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

/** @brief Handles image pub/sub and painting circles (demo of a in-source class)
*/
class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  //image_transport::Subscriber image_sub_;
  //image_transport::Publisher image_pub_;
  image_transport::Subscriber kinect_img_sub;

public:
  int i = 0;

  ImageConverter()
    : it_(nh_)
  {
    //image_pub_ = it_.advertise("out", 1);
    //image_sub_ = it_.subscribe("usb_cam/image_raw", 1, &ImageConverter::imageCb, this);
   
    kinect_img_sub = it_.subscribe("/kinect2/hd/image_color", 1, &ImageConverter::imageCb, this);
    

  }

  ~ImageConverter()
  {
    ;
  }
  /* @brief Subscriber callback for an image
  */
  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    //cv_bridge::CvImagePtr cv_ptr;
    cv_bridge::CvImagePtr kinect_color_raw;

 
    try
    {
      //cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
      kinect_color_raw = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }


    //do_really_cool_stuff(cv_ptr);
    do_really_cool_stuff(kinect_color_raw);
    //image_pub_.publish(cv_ptr->toImageMsg());
     
  }

  /** @brief paints colored circles onto input image
   *  The function draws circles along the diagional from upper left
   *  to lower right.  The color is hardcoded.
   *  @param cv_ptr Image to paint circles over
   */        
  void do_really_cool_stuff(cv_bridge::CvImagePtr kinect_color_raw)
  {

    
    i=i+1;

//save images

    std::string name = std::to_string(i); 
    name = "hard"+ name + ".jpg";
	  cv::imwrite(name, kinect_color_raw->image);
    std::cout<<i<<std::endl;
    // End Do really cool stuff ********************************************
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  
  while(ros::ok())
    {
        ros::Duration(10).sleep();
        ros::spinOnce();
    }
    
  
  return 0;
}
