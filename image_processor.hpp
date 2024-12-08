#ifndef IMAGE_PROCESSOR_HPP
#define IMAGE_PROCESSOR_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "eigen_processor.hpp"

class ImageProcessor {
public:
    // 构造函数
    ImageProcessor(const std::string& imagePath);
    
    // 处理并显示所有图像
    void processAndDisplayAll();

private:
    // 成员变量
    cv::Mat originalImage;
    cv::Mat grayImage;
    cv::Mat cvMeanImage;
    cv::Mat eigenMeanImage;
    
    // 私有方法
    void loadImage(const std::string& path);
    void convertToGray();
    void applyCvMeanFilter();
    void applyEigenMeanFilter();
    void displayImage(const std::string& windowName, const cv::Mat& image);
};

#endif