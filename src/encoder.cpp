#include <encoder.hpp>
Encoder::Encoder(unsigned char* img, int w, int h): raw_img(img), width(w), height(h)
{
    
}

void Encoder::chromSubsample()
{
    
}

void Encoder::encodeImg()
{
    // Convert colors from RGB -> YCbCr
    // TODO: Add other color spaces support (e.g. CMYK)
    size_t numOfBytes = width * height * 3;
    color::rgb2ycbcr(raw_img, numOfBytes);


}