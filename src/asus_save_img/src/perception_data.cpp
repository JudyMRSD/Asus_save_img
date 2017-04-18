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

#include "asus_save_img/SaveData.h"


int main(int argc, char **argv)
{
  ros::init(argc, argv, "data_processing_client");
  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<asus_save_img::SaveData>("ImageConverter");
  asus_save_img::SaveData srv;
  std::string item_name;
  std::cin>>item_name;
  
  

  while (ros::ok())
  {
   std::cin>>item_name; 
  //save image when button is pressed
  if(item_name.length()>0){
        
        srv.request.item_name = item_name; // todo
        
        bool res = client.call(srv);
        item_name = "";

        ros::Duration(1.0).sleep();
      }
  //save the pcd and corresponding tf
  }




}