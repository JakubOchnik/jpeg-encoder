#include <subsampler.hpp>

void Subsampler::chromaSubsample(Matrix<unsigned char>& img)
{
    // 4:1:1 implementation
    // TODO: 4:2:0 (most popular) and 4:2:2
    std::vector<unsigned char> subsampled(img.size()/2);

    if(img.getWidth() % 4 != 0)
    {
        int padding = img.getWidth() % 4;
        img.duplicateLastColumn(padding);
    }
    for(int y{0}; y < img.getHeight(); ++y)
    {
        for(int x{0}; x < img.getWidth(); x+=4)
        {
            int sum_cb{0};
            int sum_cr{0};
            for(int i{0}; i<4; ++i)
            {
                subsampled.push_back(img(x + i,y, 0));
                sum_cb += img(x + i, y, 1);
                sum_cr += img(x + i, y, 2);
            }
        uint8_t avg_cb = std::round(sum_cb / 4.0f);
        uint8_t avg_cr = std::round(sum_cr / 4.0f);
        subsampled.push_back(avg_cb);
        subsampled.push_back(avg_cr);
        }
    }

    // TODO: Return it into an interface-like container:
    // something like matrix, but for subsampled image.
    return;
}

