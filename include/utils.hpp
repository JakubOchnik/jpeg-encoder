#pragma once
#include <cmath>
#include <vector>
#include <iostream>
#include <lib/subsampMatrix.hpp>

namespace Debug
{
    template<typename T>
    void printSliceArray(T arr, int x, int y, int width, int channels)
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
            for(int x{0}; x < src.getOrigHeight(); ++x)
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
    inline unsigned char s_round(double num)
    {
        int out = round(num);
        if(out > 255)
        {
            return 255;
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
            raw[i] = s_round(0.299 * r + 0.587 * g + 0.114 * b);
            if(raw[i] )
            //Cb
            raw[i+1] = s_round(128 - 0.168736 * r - 0.331264 * g + 0.5 * b);
            //Cr
            raw[i+2] = s_round(128 + 0.5 * r - 0.418688 * g - 0.081312 * b);
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
            raw[i] = s_round(y + 1.402 * (cr - 128));
            //G
            raw[i+1] = s_round( y - 0.34414 * (cb - 128) - 0.71414 * (cr - 128));
            //B
            raw[i+2] = s_round(y + 1.772 * (cb - 128));
        }
    }
}