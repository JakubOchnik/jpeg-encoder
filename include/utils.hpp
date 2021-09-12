#pragma once
#include <cmath>
#include <vector>
#include <iostream>
#include <lib/subsampMatrix.hpp>

namespace Debug
{
    template<typename T>
    void printSliceArray(T* arr, int x, int y, int width, int channels)
    {
        int new_x = x * channels;
        for(int i{0}; i < y; ++i)
        {
            for(int j{0}; j < new_x; ++j)
            {
                std::cout << (int)arr[j + i * width * channels] << " ";
            }
            std::cout << "\n";
        }
    }

    template<typename T>
    ImgMatrix<T> subsampled2ycbcr(SubMatrix<T>& src)
    {
        ImgMatrix<T> decoded(src.getOrigWidth(), src.getOrigHeight(), src.getChannels());

        for(int y{0}; y < src.getOrigHeight(); ++y)
        {
            for(int x{0}; x < src.getOrigWidth(); ++x)
            {
                auto srcPixel = src.getPixel(x, y);
                auto newPixel = decoded.getPixel(x, y);

                for(int i{0}; i < src.getChannels(); ++i)
                {
                    newPixel[i].get() = srcPixel[i];
                }
            }
        }
        return decoded;
    }
}

namespace color
{
    inline unsigned char round_uchar(double num)
    {
        // Safe round function
        // It checks whether the value is in the uchar range
        int out = static_cast<int>(round(num));
        if(out > 255)
        {
            return 255;
        }
        else if(out < 0)
        {
            return 0;
        }
        else
        {
            return out;
        }
    }

    template<typename T>
    void rgb2ycbcr(T* raw, size_t N)
    {
        for(int i{0}; i < N; i+=3)
        {
            // R G B, but it might be different with OpenCV
            unsigned char r, g, b;
            r = raw[i];
            g = raw[i+1];
            b = raw[i+2];
            //Y
            raw[i] = round_uchar(0.299 * r + 0.587 * g + 0.114 * b);
            //Cb
            raw[i+1] = round_uchar(128.0 - 0.168736 * r - 0.331264 * g + 0.5 * b);
            //Cr
            raw[i+2] = round_uchar(128.0 + 0.5 * r - 0.418688 * g - 0.081312 * b);
        }
    }

    template<typename T>
    void ycbcr2rgb(T* raw, size_t N)
    {
        for(int i{0}; i < N; i+=3)
        {
            // R G B, but it might be different with OpenCV
            unsigned char y, cb, cr;
            y = raw[i];
            cb = raw[i+1];
            cr = raw[i+2];
            //R
            raw[i] = round_uchar(y + 1.402 * (cr - 128));
            //G
            raw[i+1] = round_uchar(y - 0.34414 * (cb - 128) - 0.71414 * (cr - 128));
            //B
            raw[i+2] = round_uchar(y + 1.772 * (cb - 128));
        }
    }
}