/*
 * utils.h
 *
 *  Created on: Jun 30, 2015
 *      Author: jpolin
 */

#ifndef TESTFUNCTIONS_H_
#define TESTFUNCTIONS_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <sstream>
#include <fstream>



void run_test(std::map<int, double> &results, std::function<void(const std::string& msg)> publish,
				std::mutex &m, bool &msg_rec, std::condition_variable &cv);
void dummy_callback(std::mutex &m, bool &msg_rec, std::condition_variable &cv);
void log_test(std::map<int, double> &results, std::string test_name);

#endif /* TESTFUNCTIONS_H_ */
