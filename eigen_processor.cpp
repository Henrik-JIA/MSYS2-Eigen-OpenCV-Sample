#include "eigen_processor.hpp"

void EigenProcessor::applyMeanFilter(const cv::Mat& input, cv::Mat& output, int kernelSize) {
    // 转换为Eigen矩阵
    Eigen::MatrixXf eigenMat = cvMatToEigen(input);
    
    // 创建输出矩阵
    Eigen::MatrixXf resultMat = Eigen::MatrixXf::Zero(input.rows, input.cols);
    
    // 进行均值滤波
    int offset = kernelSize / 2;
    
    for(int i = offset; i < input.rows - offset; i++) {
        for(int j = offset; j < input.cols - offset; j++) {
            float sum = 0;
            for(int ki = -offset; ki <= offset; ki++) {
                for(int kj = -offset; kj <= offset; kj++) {
                    sum += eigenMat(i + ki, j + kj);
                }
            }
            resultMat(i,j) = sum / (kernelSize * kernelSize);
        }
    }
    
    // 转回OpenCV Mat
    eigenToCvMat(resultMat, output);
}

Eigen::MatrixXf EigenProcessor::cvMatToEigen(const cv::Mat& input) {
    Eigen::MatrixXf eigenMat(input.rows, input.cols);
    for(int i = 0; i < input.rows; i++) {
        for(int j = 0; j < input.cols; j++) {
            eigenMat(i,j) = input.at<uchar>(i,j);
        }
    }
    return eigenMat;
}

void EigenProcessor::eigenToCvMat(const Eigen::MatrixXf& eigenMat, cv::Mat& output) {
    output = cv::Mat(eigenMat.rows(), eigenMat.cols(), CV_8UC1);
    for(int i = 0; i < output.rows; i++) {
        for(int j = 0; j < output.cols; j++) {
            output.at<uchar>(i,j) = static_cast<uchar>(eigenMat(i,j));
        }
    }
}