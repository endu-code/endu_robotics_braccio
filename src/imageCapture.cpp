#include <ros/ros.h>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>
#include <functional>
#include <mutex>

void imageCaptureThread(cv::Mat* capture, std::mutex* capMutex) {
    ROS_WARN_STREAM("Started Capture Thread!");

    cv::VideoCapture cap;
    cap.open(0, cv::CAP_ANY);

    if (!cap.isOpened()) {
        ROS_ERROR_STREAM("Error opening Camera");
        exit(-1);
        }
    ROS_INFO_STREAM("Camera opened successfully");

    while (ros::ok) {
        try {
            cv::Mat newImage;
            cap.grab();
            cap.retrieve(newImage);
            capMutex->lock();
            *capture = newImage;
            capMutex->unlock();
            }
        catch (const std::exception& ex) {
            ROS_ERROR_STREAM("Image Capture threw exception: " << ex.what());
            }
        catch (const std::string& str) {
            ROS_ERROR_STREAM("Image capture threw exception: " << str);
            }
        catch (...) {
            ROS_ERROR_STREAM("Image capture threw unknown exception: ");
            }
        }
    ROS_WARN_STREAM("Exiting Capture Thread!");    
    return;
    }