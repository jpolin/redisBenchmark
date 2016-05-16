**Author:** Joe Polin
**Date:** Summer 2015
**Description:** Compare latency for publish/subscribe functionality in Redis, ROS, and single executable with all modules compiled together.

## Background

The Stanford Robotics Lab works with a wide range of robots, applications, and code bases. Too often, code solutions are robot- or application-specific. Thus, code is often not portable from one robot to another, even if most of the functionality is robot-agnostic. In particular, Dr. Khatib's work features task and operational space controllers that can be designed without tailoring to a specific robot. To address this gap, a more modular and flexible framework is being developed. This project investigates different options for communication between different code modules:

1. Robot Operating System (ROS) Indigo: http://www.ros.org/)
2. Redis 3.0.2: http://redis.io/
3. Single executable with all modules compiled together

Options 1 and 2 are appealing since they can run in a distributed environment. However, most projects in the lab don't have this requirement, and may benefit from connecting the modules at compile time.

## Goal

To understand the tradeoffs of each option, canonical publisher/subsriber applications were written using each design and timing information was collected.

## Results

Ranking from fastest to slowest:
- Single executable (expected)
- Redis
- ROS

TODO: Include quantitative data
