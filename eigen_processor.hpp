#ifndef EIGEN_PROCESSOR_HPP
#define EIGEN_PROCESSOR_HPP

#include <opencv2/core/core.hpp>
#include <Eigen/Dense>

class EigenProcessor {
public:
    // 使用Eigen进行均值滤波
    static void applyMeanFilter(const cv::Mat& input, cv::Mat& output, int kernelSize = 5);

private:
    // OpenCV Mat 转 Eigen Matrix
    static Eigen::MatrixXf cvMatToEigen(const cv::Mat& input);
    
    // Eigen Matrix 转 OpenCV Mat
    static void eigenToCvMat(const Eigen::MatrixXf& eigenMat, cv::Mat& output);
};

#endif