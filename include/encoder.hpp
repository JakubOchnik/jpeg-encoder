#include <iostream>
#include <opencv2/opencv.hpp>
#include <utils.hpp>

class Encoder
{
    unsigned char* raw_img;
    int width;
    int height;
    void chromSubsample();
public:
    Encoder(unsigned char* img, int w, int h);
    void encodeImg();
};