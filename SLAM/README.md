# What is SLAM?

SLAM stands for Simultaneous Localization And Mapping.

* SLAM is concerned with the problem of building a map of an unknown environment by a mobile robot while at the same time navigating the environment using the map.
* SLAM is applicable for both 2D and 3D motion.
  * 2D SLAM
  * 3D SLAM



SLAM can be implemented in many ways.

* There is a huge amount of different hardware that can be used
* SLAM is more like a concept than a single algorithm



SLAM consists of multiple parts; Landmark extraction, data association, state estimation, state update and landmark update. 

There are many ways to solve each of the smaller parts.



An EKF (Extended Kalman Filter) is the heart of the SLAM process.

* The EKF keeps track of an estimate of the uncertainty in the robots position and also the uncertainty in these landmarks it has seen in the environment.



## Landmarks

- Landmarks should be easily re-observable.
- Individual landmarks should be distinguishable from each other.
- Landmarks should be plentiful in the environment.
- Landmarks should be stationary.



## Extended Kalman Filter







# Using SLAM on ROS

> implementing own SLAM is a research level stuff.
>
> Just use what SLAM the talented people have implemented.
>
> If you implement your own SLAM you can probably write a paper.



2D SLAM:

* cartographer
* hectorslam



3D SLAM:

* rtabmap

