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
#include <mrsd_ros_tutorials/image_painter.h>

/** @brief ROS image wrapper for ImagePainter
 *
 * The ROW wrapper feeds data to the ImagePainter class 
 * and does something with the result.  This helps keep the ImagePainter
 * class clear of Ros Pub/Sub code that would make testing of ImagePainter 
 * more difficult
 */

class ImageWrapper
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;
  mrsd::ImagePainter painter_;
  
public:
  ImageWrapper()
    : it_(nh_)
  {
    image_pub_ = it_.advertise("out", 1);
    image_sub_ = it_.subscribe("usb_cam/image_raw", 1, &ImageWrapper::imageCb, this);

  }

  ~ImageWrapper()
  {
    ;
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    painter_.do_really_cool_stuff(cv_ptr);
    
    image_pub_.publish(cv_ptr->toImageMsg());
  }

}; // Class Definition

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  
  ImageWrapper wrapperInstance;

  ros::spin();

  return 0;
}