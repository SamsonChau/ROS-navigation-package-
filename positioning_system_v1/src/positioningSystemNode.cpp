#include <ros/ros.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/Float64.h>
#include <sensor_msgs/Imu.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>
#include <cmath>

double x = 0.0;
double y = 0.0;
double th = 0.0;
double angle = 0.0;

double q[4] ;
double preangle = 0.0 ;
double R_rotation = 0.0;
double L_rotation = 0.0;
double angle_change = 0.0;

double siny_cosp = 0.0;
double cosy_cosp = 0.0;

double ax = 0.0;
double ay = 0.0;
double ath = 0.0;

double vx = 0.0;
double vy = 0.0;
double vth = 0.0;

double PI= 3.14159265358979323846;

void EncoderXCallback(const std_msgs::Float64::ConstPtr& msg)
{
   x = msg -> data;
   //ROS_INFO("x: %f, y: %f, th: %f", x, y, th);
}
void EncoderYCallback(const std_msgs::Float64::ConstPtr& msg)
{
   y = msg -> data;
   //ROS_INFO("x: %f, y: %f, th: %f", x, y, th);
}
void IMUCallback(const sensor_msgs::Imu::ConstPtr& msg)
{
   
   q[0] = msg -> orientation.w;
   q[1] = -(msg -> orientation.x);
   q[2] = -(msg -> orientation.y);
   q[3] = -(msg -> orientation.z);
   siny_cosp = 2 * (q[1] * q[2]+ q[0] * q[3]);
   cosy_cosp = q[0] * q[0]+q[1] * q[1] - q[2] * q[2]- q[3] * q[3];
   th = std::atan2(siny_cosp, cosy_cosp);
   angle = th * 180 / PI;
   if (abs(preangle - angle) > 359 && preangle < angle){
	R_rotation--;
	L_rotation++;
   }
   else if(abs(preangle - angle) > 359 && preangle > angle){
	R_rotation++;
	L_rotation--;
   }
   if (L_rotation>0){
	angle_change = angle/360 + L_rotation;
   }
   else if(R_rotation>1){
	angle_change = angle/360 + R_rotation;
   }
   else if(R_rotation == 1 && L_rotation== -1){
 	angle_change = (360-angle) / 360;
   }
   else{
	angle_change = (angle/360);
   }
   preangle = angle; 

   ax = msg -> angular_velocity.x;
   ay = msg -> angular_velocity.y;
   ath = msg -> angular_velocity.z;

   vx = msg -> linear_acceleration.x;
   vy = msg -> linear_acceleration.y;
   vth = msg -> linear_acceleration.z;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "positioning_system_v1");
  ros::NodeHandle n;

  
  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom",50);
  //ros::Subscriber EncoderX = n.subscribe("EncoderX", 10, EncoderXCallback);
  //ros::Subscriber EncoderY = n.subscribe("EncoderY", 10, EncoderYCallback);
  ros::Subscriber EncoderY = n.subscribe("EncoderY", 10, EncoderYCallback);
  ros::Subscriber IMU = n.subscribe("imu/data", 1000, IMUCallback);
  tf::TransformBroadcaster odom_broadcaster;

  ros::Time current_time, last_time;
  current_time = ros::Time::now();
  last_time = ros::Time::now();

  ros::Rate r(1.0);
  while(n.ok()){
        
	//ROS_INFO("x: %f, y: %f, th: %f", x, y, angle);
	//ROS_INFO("th: %f, L: %f, R: %f, delta: %f", angle, L_rotation, R_rotation, angle_change);
	ros::spinOnce();
	current_time = ros:: Time ::now();

	//since all odometry is 6DOF we'll need a quaternion created from yaw
	geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);

	//first, we'll publish the transform over tf
	geometry_msgs::TransformStamped odom_trans;
	odom_trans.header.stamp = current_time;
	odom_trans.header.frame_id = "odom";
	odom_trans.child_frame_id = "base_link";

	odom_trans.transform.translation.x = x;
	odom_trans.transform.translation.y = y;
	odom_trans.transform.translation.z = 0.0;
	odom_trans.transform.rotation = odom_quat;

	//send the transform
	odom_broadcaster.sendTransform(odom_trans);

	//next, we'll publish the odometry message over ROS
	nav_msgs::Odometry odom;
	odom.header.stamp = current_time;
	odom.header.frame_id = "odom";

	//set the position
	odom.pose.pose.position.x = x;
	odom.pose.pose.position.y = y;
	odom.pose.pose.position.z = 0.0;
	odom.pose.pose.orientation = odom_quat;

	//set the velocity
	odom.child_frame_id = "base_link";
	odom.twist.twist.linear.x = vx;
	odom.twist.twist.linear.y = vy;
        odom.twist.twist.linear.z = vth;

        odom.twist.twist.angular.x = ax;	
        odom.twist.twist.angular.y = ay; 
	odom.twist.twist.angular.z = ath;

	//publish the message
	odom_pub.publish(odom);
  }
  return 0;
}
