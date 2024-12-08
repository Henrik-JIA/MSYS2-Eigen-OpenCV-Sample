# MSYS2-Eigen-OpenCV-Sample

MSYS2安装配置并使用MinGW环境下载的OpenCV和Eigen三方库加载处理图片

## 一，MSYS2简介

面向Windows的软件分发与构建平台

[MSYS2](https://www.msys2.org/docs/what-is-msys2/)是一个工具和库的集合，提供了一个易于使用的环境，用于构建、安装和运行本机Windows软件。

## 二，软件下载
下载地址：[MSYS2](https://www.msys2.org/)
安装步骤，上述下载网页说明也非常详细，按照步骤操作即可：
![](assets/Pasted%20image%2020241207204239.png)
![](assets/Pasted%20image%2020241207204407.png)
至此，下载操作完成，下一步使用pacman工具进行配置。
## 三，pacman配置
pacman安装package的名称：
这里以MinGW64为例。
![](assets/Pasted%20image%2020241207205129.png)

这里以mingw64安装gcc。
1. 到安装目录(D:\msys64)下双击mingw64.exe打开命令行
![](assets/Pasted%20image%2020241207205436.png)

2. 测试gcc命令，发现没有该命令：
```bash
gcc -v
```
![](assets/Pasted%20image%2020241207205559.png)

3. 使用pacman安装gcc：
```bash
pacman -S mingw-w64-x86_64-gcc
```
![](assets/Pasted%20image%2020241207205654.png)
![](assets/Pasted%20image%2020241207205748.png)

4. 等待安装完成，此时打开对应安装目录，发现对应bin目录下已经生成gcc.exe
![](assets/Pasted%20image%2020241207205907.png)

至此，mingw64下的gcc命令就安装完成了。

5. 需要将mingw64/bin目录添加到系统环境变量中：
   这样操作时为了在选择编译器时可以通过扫描工具包时找到该编译器。
   ![](assets/Pasted%20image%2020241207211739.png)
   ![](assets/Pasted%20image%2020241207212236.png)
## 四，安装OpenCV库
其他安装包，和gcc安装类似，如在ming32安装opencv则对应的命令为：
```bash
pacman -S mingw-w64-x86_64-opencv
```
查看是否安装成功：（这里需要带版本4, 因为安装时 MSYS 仓库上最新版就是 opencv4）
```bash
pkg-config --cflags opencv4
```
如果能找到include/opencv4头文件所在的文件夹，就说明成功了。
![](assets/Pasted%20image%2020241207210641.png)
![](assets/Pasted%20image%2020241207210837.png)
MSYS2 下的 opencv 维护者在新的版本默认使用 QT6 作为 GUI，因此还需要安装 QT6 来支持窗口显示：
```bash
# 安装全套 Qt 
pacman -S pacman -S mingw-w64-x86_64-qt6 

# 或者精简安装，只安装基本的
pacman -S mingw-w64-x86_64-qt6-base mingw-w64-x86_64-x86_64-qt6-5compat
```
![](assets/Pasted%20image%2020241207212754.png)
以上opencv库编译完成。
```txt
头文件所在位置：D:\msys64\mingw64\include\opencv4\opencv2
静态库所在位置：D:\msys64\mingw64\lib\opencv4\3rdparty
动态库所在位置：D:\msys64\mingw64\bin
```

## 五、创建测试程序

1. 测试图片为D:/22.jpg
   ![](assets/22.jpg)

2. 首先创建一个main.cpp测试代码：

```c++
#include <iostream>
#include "image_processor.hpp"

int main()
{
    try {
        ImageProcessor processor("D:\\22.jpg");
        processor.processAndDisplayAll();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
```

3. 其它*.cpp和*.hpp文件

- image_processor.hpp：
```c++
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
```

- image_processor.cpp：
```c++
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
```

- eigen_processor.hpp：
```c++
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
```

- eigen_processor.cpp：
```c++
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
```

4. 创建一个CmakeLists.txt文件
```cmake
cmake_minimum_required(VERSION 3.5)

project(vsin01 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(OpenCV_DIR D:/Forvs/opencv4.10/x64/mingw/lib)
set(OpenCV_DIR D:/msys64/mingw64/lib) #库路径你自己换成你电脑上的

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

# 包含 Eigen 头文件目录
include_directories("D:/msys64/mingw64/include/eigen3")

# 查找项目中的所有 .cpp 源文件，排除 CMake 生成目录
file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/*.cpp
    ${CMAKE_SOURCE_DIR}/*.hpp
)

# 手动排除掉 CMake 生成的 CMakeFiles 目录
list(FILTER SOURCES EXCLUDE REGEX "/CMakeFiles/")  

# 将找到的文件添加到可执行文件中
add_executable(MYCODE ${SOURCES})

target_link_libraries(MYCODE PRIVATE ${OpenCV_LIBS})
```
这样就创建好了两个文件了，这里build是编译自行生成的文件夹。
![](assets/Pasted%20image%2020241208161723.png)

5. 之后在该目录下启动VScode。并选择D:\msys64\mingw64\bin\gcc.exe为编译工具，因为OpenCV和Eigen也是这个mingw64所编译的，两者需要一致。
![](assets/Pasted%20image%2020241207231712.png)

6. 删除缓存并重新配置后，点击生成。
![](assets/Pasted%20image%2020241207231910.png)
![](assets/Pasted%20image%2020241207232019.png)
显示生成完成，这样就证明没有问题了，编译成功了。
![](assets/Pasted%20image%2020241207232056.png)

7. 运行调试main.cpp代码，点击调试。
![](assets/Pasted%20image%2020241207232214.png) 
8. 以下为代码输出的结果：
![](assets/Pasted%20image%2020241208161143.png)

这样就完成了Msys2安装配置，并使用MinGW环境下载的OpenCV和Eigen三方库加载和处理图片。