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