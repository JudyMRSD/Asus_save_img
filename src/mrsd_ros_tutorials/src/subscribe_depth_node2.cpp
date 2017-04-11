

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

//service 
#include "mrsd_ros_tutorials/SaveData.h"


/** @brief Handles image pub/sub and painting circles (demo of a in-source class)
*/
class ImageConverter
{
  ros::NodeHandle nh;
  image_transport::ImageTransport it_;
//subscriber to rgb img and point cloud
  image_transport::Subscriber kinect_img_sub;
  ros::Subscriber kinect_pc_sub;
  image_transport::Subscriber kinect_depth_sub;
//create pointer to rgb image and point cloud and depth

  cv_bridge::CvImagePtr kinect_color_raw;
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr kinect_color_pc;
  cv_bridge::CvImagePtr kinect_depth_raw;

//PCD writer
  pcl::PCDWriter writer;

//the service is created and advertised over ROS
  ros::ServiceServer service;
public:

  ImageConverter()
    : it_(nh)
  {
    
    kinect_img_sub = it_.subscribe("/kinect2/hd/image_color_rect", 1, &ImageConverter::imageCb, this);
    //kinect_depth_sub = it_.subscribe("/kinect2/hd/image_depth_rect", 1, &ImageConverter::depthCb, this);
    kinect_pc_sub =  nh.subscribe("/kinect2/hd/points", 1, &ImageConverter::kinectPointCloudCB, this);
    kinect_color_pc = pcl::PointCloud<pcl::PointXYZRGB>::Ptr(new pcl::PointCloud<pcl::PointXYZRGB>);
    service = nh.advertiseService("ImageConverter", &ImageConverter::saveDataCB, this);
  }

  ~ImageConverter()
  {
    ;
  }
 
  void imageCb(const sensor_msgs::ImageConstPtr& msg )
  {
     kinect_color_raw = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
     ROS_INFO("[Perception] Get Kinect Image Data ");

  }

  //void depthCb(const sensor_msgs::ImageConstPtr& msg )
  //{
   //  kinect_depth_raw = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_16UC1);
    // ROS_INFO("[Perception] Get Kinect Depth Data ");

  //}

  void kinectPointCloudCB(const sensor_msgs::PointCloud2ConstPtr& cloud_msg){
     pcl::fromROSMsg(*cloud_msg, *kinect_color_pc);
     ROS_INFO("[Perception] Get Kinect Point Cloud Data");
  }


     
  //void saveDataCB(cv_bridge::CvImagePtr kinect_color_raw)
  bool saveDataCB(mrsd_ros_tutorials::SaveData::Request &req, 
    mrsd_ros_tutorials::SaveData::Response &res )
  {
/*
    SaveData.srv:
    uint16 item_number
---
uint64 status
    */  

//file name 
    std::cout << "Save Data request recieved: \n"<< 
           "item_number: "<< req.item_number <<std::endl;
//save images

    std::string name;
    name = std::to_string(req.item_number);
    std::string pc_name;
    pc_name = name;
    //std::string depth_name;
    //depth_name = name;

    name = "tote"+ name + ".jpg";
    cv::imwrite(name, kinect_color_raw->image);
    ROS_INFO("[Perception] Save image file ");

    pc_name = "tote_pc"+ pc_name +".pcd";
    writer.writeBinary(pc_name, *kinect_color_pc);
    ROS_INFO("[Perception] Save PCD file ");
	
    //depth_name = "tote_depth"+ depth_name + ".jpg";
    //cv::imwrite(name, kinect_depth_raw->image);
    //ROS_INFO("[Perception] Save image file ");

    res.status = 1;
    return true;

  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  
 
  while(ros::ok())
    {
        ros::Duration(1).sleep();
        ros::spinOnce();
   //     ic.saveDataCB("1");
    }
    
  
  return 0;
}
