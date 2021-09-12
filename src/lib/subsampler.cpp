#include <subsampler.hpp>

SubMatrix<unsigned char> Subsampler::chromaSubsample411(ImgMatrix<unsigned char>& img)
{
    if(img.getWidth() % 4 != 0)
    {
        int padding = img.getWidth() % 4;
        img.duplicateLastColumn(padding);
    }
    SubMatrix<unsigned char> sub(img, SubsamplingType::s411);
    
    size_t idx = 0;
    for(int y{0}; y < img.getHeight(); ++y)
    {
        // Move the window 4 pixels to the right (width of each subsampled group)
        for(int x{0}; x < img.getWidth(); x+=4)
        {
            int sum_cb{0};
            int sum_cr{0};
            // Append each Y component to the subMatrix
            // and add each Cb and Cr component to the group sum
            for(int i{0}; i<4; ++i)
            {
                sub[idx++] = img(x + i, y, 0);
                sum_cb += img(x + i, y, 1);
                sum_cr += img(x + i, y, 2);
            }
            // Calculate the Cb and Cr average
            uint8_t avg_cb = std::round(sum_cb / 4.0f);
            uint8_t avg_cr = std::round(sum_cr / 4.0f);
            // Append the averages as the 5-th and 6-th component of the group
            sub[idx] = avg_cb;
            sub[++idx] = avg_cr;
            ++idx;
        }
    }
    return sub;
}

