
/*
 * Author: Joe Polin
 * Date: June 2015
 * Purpose: Make a simple redis application so that we can figure out how fast it runs.
 */

#include <stdio.h>
#include <iostream>
#include <redox.hpp>
#include <map>
#include <string>
#include <functional>

#include <thread>
#include <chrono>
#include <test_functions.h>

// Use different thread for subscriber?
#define USETHREAD

using namespace std;

void create_subscriber(redox::Subscriber &rdx_sub, string topic, mutex &m, bool &msg_rec, condition_variable &cv,
																mutex &m_test_over, bool &test_over, condition_variable &cv_test_over){
	// Subscriber just triggers conditional variable
	//cout << "**Creating subscriber** \n";
	rdx_sub.subscribe(topic, [&](const string& topic, const string& msg) {
		//cout << "Message Received" << endl;
		//cout << msg << endl;
		dummy_callback(m, msg_rec, cv);
		});

	unique_lock<mutex> lk(m_test_over);
	cv_test_over.wait(lk, [&]{return test_over;});

	//cout << "Subscriber CV signaled--subscriber thread returning \n";
}

int main(int argc, char* argv[]) {

	condition_variable cv, cv_test_over;
	mutex m, m_test_over;
	bool msg_rec = false;
	bool test_over = false;

	// For each framework, map of: payload size (bytes) : time for niters
	map<int, double> redis_results;

	// ---------- Redis (REDOX) ---------- //


	// Initialize and connect
	redox::Redox rdx_pub;
	redox::Subscriber rdx_sub;
	if(!rdx_pub.connect() || !rdx_sub.connect()) {
		cout << "\n Redis: Could not connect publisher/subscriber. Exiting... \n";
		return 1;
	}

	string topic = "Test Topic";

	// TODO: Try using nowait
	rdx_pub.noWait(true);

#ifdef USETHREAD
	thread subscriber_thread(create_subscriber, ref(rdx_sub), ref(topic), ref(m), ref(msg_rec), ref(cv),
																		  ref(m_test_over), ref(test_over), ref(cv_test_over));
#else
	// Subscriber just triggers conditional variable
	rdx_sub.subscribe(topic, [&](const string& topic, const string& msg) {
		//cout << "Message Received" << endl;
		//cout << msg << endl;
		dummy_callback(m, msg_rec, cv);
		});
#endif

	// Subscriber not ready right away--is there a better way to do this?
	this_thread::sleep_for(chrono::milliseconds(100));

	run_test(redis_results, [&](const string& msg){
		//cout << "Publishing Message\n";
		rdx_pub.publish(topic, msg);
	}, m, msg_rec, cv);

#ifdef USETHREAD
	// Tell subscriber thread to wrap up
	{
	unique_lock<mutex> lk(m_test_over);
	test_over = true;
	cv_test_over.notify_all();
	}
#endif

	// Clean up Redis
	//cout << "Trying to disconnect subscriber... \n";
	rdx_sub.disconnect();
	//cout << "Trying to disconnect publisher... \n";
	rdx_pub.disconnect();
	//cout << "Trying to join the subscriber thread...\n";
#ifdef USETHREAD
	subscriber_thread.join();
#endif
	//cout << "Trying to log results... \n";
	log_test(redis_results, "Redis times");


	return 0;

}
