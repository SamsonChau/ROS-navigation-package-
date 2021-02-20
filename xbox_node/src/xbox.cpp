#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>

class Robot {
	public:

	Robot();

	private:
	
	void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);

	ros::NodeHandle nh_;
	int linearX_, linearY_ , angular_;
	double l_scale_, a_scale_;
	ros::Publisher vel_pub_;
	ros::Subscriber joy_sub_;
};

Robot::Robot():
	linearX_(0),
	linearY_(1),
	angular_(3)
	{
	  //nh_.param("axis_linear", linearX_, linear_);
	  //nh_.param("axis_angular", angular_, angular_);
	  //nh_.param("scale_angular", a_scale_, a_scale_);
	  //nh_.param("scale_linear", l_scale_, l_scale_);
		vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
		joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &Robot::joyCallback, this);
	

}
void Robot::joyCallback(const sensor_msgs::Joy::ConstPtr& joy){
	geometry_msgs::Twist twist;	
	twist.angular.z = joy->axes[angular_];
	twist.linear.x = joy->axes[linearX_];
	twist.linear.y = joy->axes[linearY_];
	vel_pub_.publish(twist);
}

int main(int argc, char** argv){
	ros::init(argc, argv, "xbox_node");
	Robot controller;
	ros::spin();
}
