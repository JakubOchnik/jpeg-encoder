#include <iostream>
#include <opencv2/opencv.hpp>
#include <utils.hpp>
#include <lib/matrix.hpp>
#include <lib/subsampler.hpp>

class Encoder
{
    Matrix<unsigned char> image;
    unsigned char* raw_img;
    int width;
    int height;
    void chromaSubsample(std::vector<uint8_t>& subsampled, int pixelsToSample);
public:
    Encoder(unsigned char* img, int w, int h);
    void encodeImg();
};