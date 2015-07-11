/*
 * utils.cpp
 *
 *  Created on: Jun 30, 2015
 *      Author: jpolin
 */


#include "test_functions.h"

using namespace std;

void run_test(map<int, double> &results, function<void(const string& msg)> publish,
				mutex &m, bool &msg_rec, condition_variable &cv) {

	/******** Test Parameters **********/
	const int niters = 10000; // Per msg size
	//vector<int> payload_size = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024}; // bytes
	vector<int> payload_size = {600}; // Pretty good middleground
	/***********************************/

	for (vector<int>::iterator it = payload_size.begin(); it != payload_size.end(); ++it) {

		// Generate string with nbytes
		stringstream msg_stream;
		for (int i = 0; i < *it; i++)
			msg_stream << *((char*)&i)%128; // Keep as ascii chars

		// Send this msg niters times and time it
		string msg = msg_stream.str();
		clock_t start_time = clock();
		for (int i = 0; i < niters; i++) {
			//cout << "Publishing... " << flush;
			publish(msg);
			// Wait for callback to fire
			unique_lock<mutex> lk(m);
			cv.wait(lk, [&]{return msg_rec;});
			msg_rec = false;
		}
		results[*it] = (clock() - start_time) / (double) CLOCKS_PER_SEC;
	}
}

void dummy_callback(mutex &m, bool &msg_rec, condition_variable &cv){
	unique_lock<mutex> lk(m);
	msg_rec = true;
	cv.notify_one();
}

void log_test(map<int, double> &results, string test_name){
	ofstream ofs(test_name, ofstream::out);
	for(auto it : results) {
		ofs << it.first << ", "<< it.second << endl;
		// Print to console as well
		if (true) {
			cout << test_name << ": " << it.first << " bytes, " << it.second << " secs, "
				 << it.second / 10.0 << " milliseconds per message \n";
		}
	}

	ofs.close();
}

