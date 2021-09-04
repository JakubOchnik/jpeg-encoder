#include <iostream>
#include <opencv2/opencv.hpp>
#include <utils.hpp>

class Encoder
{
    cv::Mat image;
    unsigned char* raw_img;
    int width;
    int height;
    void chromaSubsample(std::vector<uint8_t>& subsampled, int pixelsToSample);
public:
    Encoder(unsigned char* img, int w, int h, cv::Mat full_img);
    void encodeImg();
};