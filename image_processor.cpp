#include "image_processor.hpp"

ImageProcessor::ImageProcessor(const std::string& imagePath) {
    loadImage(imagePath);
}

void ImageProcessor::loadImage(const std::string& path) {
    originalImage = cv::imread(path);
    if(originalImage.empty()) {
        throw std::runtime_error("Failed to load image: " + path);
    }
}

void ImageProcessor::convertToGray() {
    cv::cvtColor(originalImage, grayImage, cv::COLOR_BGR2GRAY);
}

void ImageProcessor::applyCvMeanFilter() {
    cv::blur(grayImage, cvMeanImage, cv::Size(5, 5));
}

void ImageProcessor::applyEigenMeanFilter() {
    EigenProcessor::applyMeanFilter(grayImage, eigenMeanImage);
}

void ImageProcessor::displayImage(const std::string& windowName, const cv::Mat& image) {
    cv::namedWindow(windowName);
    cv::imshow(windowName, image);
}

void ImageProcessor::processAndDisplayAll() {
    // 显示原图
    displayImage("Original Image", originalImage);
    
    // 处理并显示灰度图
    convertToGray();
    displayImage("Gray Image", grayImage);
    
    // 处理并显示OpenCV均值滤波结果
    applyCvMeanFilter();
    displayImage("OpenCV Mean Image", cvMeanImage);
    
    // 处理并显示Eigen均值滤波结果
    applyEigenMeanFilter();
    displayImage("Eigen Mean Image", eigenMeanImage);
    
    cv::waitKey(0);
}