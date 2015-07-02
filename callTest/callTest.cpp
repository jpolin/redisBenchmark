
/*
 * Author: Joe Polin
 * Date: June 2015
 * Purpose: Make a simple redis application so that we can figure out how fast it runs.
 */

#include <stdio.h>
#include <iostream>
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
	map<int, double> results;

	run_test(results, [&](const string& msg){
		dummy_callback(m, msg_rec, cv);
	}, m, msg_rec, cv);

	log_test(results, "Call times");


	return 0;

}
