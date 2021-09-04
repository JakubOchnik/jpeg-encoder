#pragma once
#include <cmath>
#include <vector>

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
    std::vector<uint8_t> subsampled2ycbcr(T arr, int sampledPixels, int N)
    {
        std::vector<uint8_t> decoded;
        int step = sampledPixels + 2;
        int counter = 0;
        for(int trail{0}; trail < N; trail+=step)
        {
            uint8_t cb = arr[trail + sampledPixels];
            uint8_t cr = arr[trail + sampledPixels + 1];
            for(int i{0}; i<sampledPixels; ++i)
            {
                decoded.push_back(arr[trail + i]);
                counter++;
                decoded.push_back(cb);
                counter++;
                decoded.push_back(cr);
                counter++;
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