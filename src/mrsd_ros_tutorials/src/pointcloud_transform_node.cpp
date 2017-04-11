#include <iostream>

#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>

#include <pcl/common/common.h>
#include <pcl/common/transforms.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl_ros/point_cloud.h>

/** @brief Handles Pub/Sub for pointcloud as an illustrative example
*/
class CloudPublisher
{
public:
  ros::NodeHandle nh_;
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr in_cloud_;
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr out_cloud_;
  ros::Publisher cloud_publisher_;
  ros::Subscriber cloud_subscriber_;

  CloudPublisher():
    in_cloud_(new pcl::PointCloud<pcl::PointXYZRGB>),
    out_cloud_(new pcl::PointCloud<pcl::PointXYZRGB>)
  {
    ROS_INFO("Initialized CloudPublisher Class");
    cloud_publisher_ = nh_.advertise<sensor_msgs::PointCloud2>("/out_cloud", 1);
    cloud_subscriber_ = nh_.subscribe("/camera/depth_registered/points_throttle", 1, &CloudPublisher::cloudCallback, this);
  }

  void cloudCallback(const sensor_msgs::PointCloud2ConstPtr& msg)
  {
    pcl::fromROSMsg(*msg, *in_cloud_);

    std::string frame_name = msg->header.frame_id;

    transformCloud(in_cloud_, out_cloud_);

    removeColor(out_cloud_);

    publishCloud(out_cloud_, frame_name);

  }
  /** @brief Simple method to show how to iterate over contents of a pointcloud
  */
  void removeColor(pcl::PointCloud<pcl::PointXYZRGB>::Ptr src)
  {
    for(auto it = (*src).begin(); it != src->end(); it++)
    {
      it->r = 255;
      it->b = 0;
      it->g = 0;
    }
  }

  /* @brief Simple method to show how to transform pointcloud using PCL
  */
  void transformCloud(pcl::PointCloud<pcl::PointXYZRGB>::Ptr src,
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr dst)
  {

    Eigen::Matrix4f M;
    M <<  1,  0,  0 , 0,
          0,  1,  0,  0,
          0,  0,  1,  0,
          0,  0,  0,  1;

    pcl::transformPointCloud (*src, *dst, M);
  }

  /* @brief Simple illustrative Publish example
  */
  void publishCloud(pcl::PointCloud<pcl::PointXYZRGB>::Ptr src, std::string frame_name)
  {         
    sensor_msgs::PointCloud2 cloud_msg;
    pcl::toROSMsg(*src, cloud_msg);
    cloud_msg.header.stamp = ros::Time::now();
    cloud_msg.header.frame_id = frame_name;
    cloud_publisher_.publish(cloud_msg);
  }


}; // Class Definition


int main(int argc, char** argv)
{
  ros::init(argc, argv, "pointcloud_transform_node");

  CloudPublisher CP;

  while(ros::ok() )
  {
    ros::Duration(.1).sleep();
    ros::spinOnce();
  }
  return 0;
}