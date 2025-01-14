#include <ros/ros.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>

#include "clf_object_recognition_msgs/LoadPointCloudMsg.h"
#include "load_point_cloud_provider.h"


bool loadPointCloud(clf_object_recognition_msgs::LoadPointCloudMsg::Request& req,
                    clf_object_recognition_msgs::LoadPointCloudMsg::Response& res)
{
    // Load the PCD file from disk
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
    if (pcl::io::loadPCDFile<pcl::PointXYZ> (req.filename, *cloud) == -1)
    {
        ROS_ERROR_STREAM("Failed to load point cloud from file: " << req.filename);
        return false;
    }

    // Convert the PCL point cloud to a ROS message
    sensor_msgs::PointCloud2 msg;
    pcl::toROSMsg(*cloud, msg);
    msg.header.frame_id = req.frame_id;

    // Populate the response with the ROS message
    res.pointcloud = msg;

    return true;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "load_point_cloud_server");
    ros::NodeHandle nh;

    ros::ServiceServer service = nh.advertiseService("load_point_cloud", loadPointCloud);

    ROS_INFO("Load Point Cloud service ready.");
    ros::spin();

    return 0;
}
