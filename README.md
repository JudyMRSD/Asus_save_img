save_image:
roslaunch openni2_launch openni2.launch 
rosrun image_view image_view image:=/camera/rgb/image_raw

source devel/setup.bash


rosrun mrsd_ros_tutorials subscribe_depth_node
rosrun mrsd_ros_tutorials perception_data 

1. whitesheet
2. no sheet

