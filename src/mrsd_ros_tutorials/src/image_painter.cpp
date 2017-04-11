/* 
 * Tutorial code courtesy of cv_bridge: 
 * http://wiki.ros.org/cv_bridge/Tutorials/UsingCvBridgeToConvertBetweenROSImagesAndOpenCVImages#cv_bridge.2BAC8-Tutorials.2BAC8-UsingCvBridgeCppDiamondback.CA-ae53133f28c1a7955fa0e72fb63f62d02bdb25a8_1
 */

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <mrsd_ros_tutorials/image_painter.h>


void mrsd::ImagePainter::do_really_cool_stuff(cv_bridge::CvImagePtr cv_ptr)
{
  // Do really cool stuff here *******************************************
  int rows = cv_ptr->image.rows;
  int cols = cv_ptr->image.cols;
  if( rows > 60 && cols > 60)
  {
    int min = (rows < cols) ? rows : cols ;
    for(int i = 0; i < min; i = i + 10)
    {
      cv::circle(cv_ptr->image, cv::Point(i, i), 10, CV_RGB(0,0,255));
    }    
  }
  // End Do really cool stuff ********************************************
}