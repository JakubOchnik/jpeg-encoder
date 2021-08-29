#pragma once
#include <cmath>

namespace color{
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