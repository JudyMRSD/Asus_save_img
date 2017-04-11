/* 
 * Tutorial code courtesy of cv_bridge: 
 * http://wiki.ros.org/cv_bridge/Tutorials/UsingCvBridgeToConvertBetweenROSImagesAndOpenCVImages#cv_bridge.2BAC8-Tutorials.2BAC8-UsingCvBridgeCppDiamondback.CA-ae53133f28c1a7955fa0e72fb63f62d02bdb25a8_1
 */

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

volatile bool g_new_image = false;
cv_bridge::CvImagePtr g_cv_ptr;

void imageCb(const sensor_msgs::ImageConstPtr& msg)
{
  try{

    g_cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);

  }catch (cv_bridge::Exception& e){

    ROS_ERROR("cv_bridge exception: %s", e.what());
    return;
  }

  g_new_image = true;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");

  ros::NodeHandle nh_;
  image_transport::ImageTransport it_(nh_);
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

  image_pub_ = it_.advertise("out", 1);
  image_sub_ = it_.subscribe("usb_cam/image_raw", 1, &imageCb);

  while(ros::ok() )
  {
    if(g_new_image)
    {

      // Do really cool stuff here *******************************************
      int rows = g_cv_ptr->image.rows;
      int cols = g_cv_ptr->image.cols;
      if( rows > 60 && cols > 60)
      {
        int min = (rows < cols) ? rows : cols ;
        for(int i = 0; i < min; i = i + 10)
        {
          cv::circle(g_cv_ptr->image, cv::Point(i, i), 10, CV_RGB(0,255,0));
        }    
      }
      // End Do really cool stuff ********************************************

      g_new_image = false;

      image_pub_.publish(g_cv_ptr->toImageMsg());

    }
    ros::Duration(.01).sleep();
    ros::spinOnce();
  }
  return 0;
}