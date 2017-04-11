#include <iostream>
#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/Pose.h>

#include <dynamic_reconfigure/server.h>
#include <mrsd_ros_tutorials/TransformsConfig.h>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

#include <tf/transform_broadcaster.h>
#include <tf_conversions/tf_eigen.h>

/** @brief Class to manage box marker position
 * 
 * a marker is published in /box frame.  Dynamic reconfigure parameters
 * are used to manage the position and rotation of the box
 */
class BoxDisplayer
{
public:

  ros::NodeHandle nh_;
  
  geometry_msgs::TransformStamped box_tf_;
  ros::Publisher marker_pub_;
  visualization_msgs::Marker marker_; ///< msg for displaying a marker in RVIZ
  tf::TransformBroadcaster tf_broadcaster_;

  BoxDisplayer()
  {
    marker_pub_ = nh_.advertise<visualization_msgs::Marker>("box_marker", 1);
    marker_.header.frame_id = "/box";
    //marker_.header.stamp = ros::Time::now();
    marker_.ns = "shelfbin_marker";
    marker_.id = 0;
    marker_.type = visualization_msgs::Marker::MESH_RESOURCE;
    marker_.mesh_resource = "package://mrsd_ros_tutorials/model/bin.STL";

    marker_.action = visualization_msgs::Marker::ADD;

    marker_.scale.x = 1.0;
    marker_.scale.y = 1.0;
    marker_.scale.z = 1.0;

    marker_.color.r = 0.0f;
    marker_.color.g = 1.0f;
    marker_.color.b = 0.0f;
    marker_.color.a = 1.0;

    marker_.lifetime = ros::Duration();
    geometry_msgs::Pose zero_pose;
    zero_pose.orientation.w = 1; // so inital pose is a valid pose (magnitude of Quaternion should = 1)
    marker_.pose = zero_pose;

    box_tf_.transform.rotation.w = 1;
    std::cout << "Class initialized\n";

  }

  /** @brief update box transfrom from dynamic reconfigure callback
  * Eigen is used to simplify the RPY to quaternion calculation
  */
  void reconfig_callback(mrsd_ros_tutorials::TransformsConfig &config, uint32_t level) {
  ROS_INFO("Got a callback from reconfigure");
  // Set Translation
  box_tf_.transform.translation.x = config.x;
  box_tf_.transform.translation.y = config.y;
  box_tf_.transform.translation.z = config.z;

  // Set Rotation
  Eigen::AngleAxisd roll = Eigen::AngleAxisd(config.roll, Eigen::Vector3d::UnitX());
  Eigen::AngleAxisd pitch = Eigen::AngleAxisd(config.pitch, Eigen::Vector3d::UnitY());
  Eigen::AngleAxisd yaw = Eigen::AngleAxisd(config.yaw, Eigen::Vector3d::UnitZ());

  Eigen::Quaterniond QEigen;
  QEigen = roll * pitch * yaw;
  
  box_tf_.transform.rotation.x = QEigen.x();
  box_tf_.transform.rotation.y = QEigen.y();
  box_tf_.transform.rotation.z = QEigen.z();
  box_tf_.transform.rotation.w = QEigen.w();

  }

  /** @brief heartbeat update of box transform at current time
   */
  void updateBox()
  {
    //std::cout << "updating Box\n";
    marker_.header.stamp = ros::Time::now();
    marker_pub_.publish(marker_);

    box_tf_.header.stamp = ros::Time::now();
    box_tf_.header.frame_id = "world";
    box_tf_.child_frame_id = "box";

    tf_broadcaster_.sendTransform(box_tf_);
  }


}; // Class Definition


int main( int argc, char** argv )
{
  ros::init(argc, argv, "display_box_marker");
  ros::Time::init();
  ros::Rate rate(10); // hz
  ros::NodeHandle nh;
  //BoxDisplayer *BDptr = new BoxDisplayer();
  BoxDisplayer BD;


  dynamic_reconfigure::Server<mrsd_ros_tutorials::TransformsConfig> server;
  dynamic_reconfigure::Server<mrsd_ros_tutorials::TransformsConfig>::CallbackType cbType;

  //cbType = boost::bind(&BoxDisplayer::reconfig_callback, BDptr, _1);
  cbType = boost::bind(&BoxDisplayer::reconfig_callback, &BD, _1, _2);
  server.setCallback(cbType);

  std::cout << "Initialized display_box_marker node\n";


  while (ros::ok() )
  {
    //BDptr->updateBox();
    BD.updateBox();
    ros::spinOnce();
    rate.sleep();
  }
  return 0;
}
