/*
 * ros_pub.cpp
 *
 *  Created on: Jul 1, 2015
 *      Author: jpolin
 */

#include "ros/ros.h"
#include "std_msgs/String.h"
#include <string>
#include <test_functions.h>

#include <thread>
#include <chrono>

using namespace std;

// Lets publisher know when subscriber is done
std::condition_variable cv;
std::mutex m;
bool msg_rec = false;

void ros_sub_callback(const std_msgs::String::ConstPtr& msg){
	//cout << "Receiving..." << endl << flush;
	//this_thread::sleep_for(chrono::milliseconds(100));
	dummy_callback(m, msg_rec, cv);
}

int main(int argc, char** argv){

	string topic = "Test_Topic";

	ros::init(argc, argv, "ros_publisher");
	ros::NodeHandle n;

	// Make Subscriber
	ros::Subscriber ros_sub = n.subscribe(topic, 2, ros_sub_callback);

	// Publisher (buffer of 2 since we're waiting for reply before sending)
	ros::Publisher ros_pub = n.advertise<std_msgs::String>(topic, 2);

	map<int, double> results;
/*
	run_test(results,[&](const string& msg){
		// Need to make it a ROS msg
		std_msgs::String ros_msg;
		ros_msg.data = msg;
		ros_pub.publish(ros_msg);
		ros::spinOnce();
		cout << "published..." << flush;

		this_thread::sleep_for(chrono::milliseconds(1000));
	}, m, msg_rec, cv);
*/

	this_thread::sleep_for(chrono::milliseconds(1000));

	ros::AsyncSpinner spinner(2); // Use 4 threads
	spinner.start();

	/******** Test Parameters **********/
		const int niters = 1000; // Per msg size
		vector<int> payload_size = {64}; // bytes
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
				std_msgs::String ros_msg;
				ros_msg.data = msg;
				ros_pub.publish(ros_msg);
//				ros::spinOnce();
				// Wait for callback to fire
				{
					unique_lock<mutex> lk(m);
					cv.wait(lk, [&]{return msg_rec;});
					msg_rec = false;
				}
			}
			results[*it] = (clock() - start_time) / (double) CLOCKS_PER_SEC;
		}
	/*
	std_msgs::String ros_msg;
	ros_msg.data = "Transmitting... \n";
	while (true) {
		ros_pub.publish(ros_msg);
	}
	*/

		log_test(results, "ROS Test");

}
