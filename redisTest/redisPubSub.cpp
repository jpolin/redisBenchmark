
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

#include <thread>
#include <chrono>
#include <test_functions.h>

using namespace std;

int main(int argc, char* argv[]) {

	std::condition_variable cv;
	std::mutex m;
	bool msg_rec = false;

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

	// Subscriber just triggers conditional variable
	rdx_sub.subscribe(topic, [&](const string& topic, const string& msg) {
		//cout << "Message Received" << endl;
		//cout << msg << endl;
		dummy_callback(m, msg_rec, cv);
		});

	// Subscriber not ready right away--is there a better way to do this?
	this_thread::sleep_for(chrono::milliseconds(100));

	run_test(redis_results, [&](const string& msg){
		rdx_pub.publish(topic, msg);
	}, m, msg_rec, cv);

	// Clean up Redis
	rdx_pub.disconnect();
	rdx_sub.disconnect();

	log_test(redis_results, "Redis times");


	return 0;

}
