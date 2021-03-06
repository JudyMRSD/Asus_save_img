//client: process data got from subscribe_pc_node.cpp

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

//pcl
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl_ros/point_cloud.h>
#include <sensor_msgs/PointCloud2.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

#include "mrsd_ros_tutorials/SaveData.h"


int main(int argc, char **argv)
{
  ros::init(argc, argv, "data_processing_client");
  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<mrsd_ros_tutorials::SaveData>("ImageConverter");
  mrsd_ros_tutorials::SaveData srv;
  int item_number;
  std::cin>>item_number;
  
  srv.request.item_number = item_number;
  std::cout << "("<< srv.request.item_number<< ")\n";

  while (ros::ok())
  {
   std::cin>>item_number; 
  //save image when button is pressed
  if(item_number!=0){
        
        srv.request.item_number = item_number; // todo
        
        bool res = client.call(srv);
	      item_number = 0;

        ros::Duration(1.0).sleep();
      }
  //save the pcd and corresponding tf
  }




}
