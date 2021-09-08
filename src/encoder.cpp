#include <encoder.hpp>

Encoder::Encoder(unsigned char* img, int w, int h): raw_img(img), width(w), height(h)
{
    image = ImgMatrix(img, w, h, 3, true);
}

void Encoder::chromaSubsample(std::vector<uint8_t>& subsampled, int pixelsToSample)
{
    size_t N = 3 * width * height;
    // 1 2 3 4 5
    // 6 7 8 9 10
    //     x y
    // idx(0,0 [1]) = x + y * width = 0 + 0 * 5 = 0
    // idx(1,0 [2]) = x + y * width = 1 + 0 * 5 = 1
    // idx(0,1 [6]) = x + y * width = 0 + 1 * 5 = 5
    // idx(1,1 [7]) = x + y * width = 1 + 1 * 5 = 6

    // Take every four pixels
    int offset = pixelsToSample*3;
    for(int trail{0}; trail<N; trail+=offset)
    {
        int sum_cb{0};
        int sum_cr{0};
        for(int i{0}; i<pixelsToSample; ++i)
        {
            subsampled.push_back(raw_img[trail + i*3]);
            sum_cb += raw_img[trail + i*3 + 1];
            sum_cr += raw_img[trail + i*3 + 2];
        }
        uint8_t avg_cb = round(sum_cb / 4.0f);
        uint8_t avg_cr = round(sum_cr / 4.0f);
        subsampled.push_back(avg_cb);
        subsampled.push_back(avg_cr);
    }

}

void Encoder::encodeImg()
{
    using namespace cv;
    // Convert colors from RGB -> YCbCr
    // TODO in the future: Add other color spaces support (e.g. CMYK)
    size_t numOfBytes = width * height * 3;
    color::rgb2ycbcr(raw_img, numOfBytes);

    // TODO: Implement chroma subsampling
    std::vector<uint8_t> subsampled;
    chromaSubsample(subsampled, 4);
    std::cout << "Prev size:" << width * height * 3 << "\n Current size:" << subsampled.size() << "\n";
    std::cout << "\n";
    std::vector<uint8_t> desampled = Debug::subsampled2ycbcr(subsampled,4, subsampled.size());
    std::cout << "\n";
    unsigned char* raw_desampled = &desampled[0];
    std::cout << "\nDesampled\n";
    Debug::printSliceArray(raw_desampled,6,2,width, 3);
    std::cout << "\nOriginal\n";
    Debug::printSliceArray(raw_img,6,2,width, 3);
    color::ycbcr2rgb(raw_desampled,numOfBytes);
    color::ycbcr2rgb(raw_img,numOfBytes);


    Mat before_desampling(width, height, CV_8UC3, raw_img);
    cv::imshow("Before", before_desampling);
    Mat after_desampling(width, height, CV_8UC3, raw_desampled);
    cv::imshow("Test", after_desampling);
    cv::waitKey(0);

    // TODO: Implement Discrete Cosine Transform & Quantization

    // TODO: Implement Delta Encoding

    // TODO: Implement Huffmann Encoding (add a header?)

    // TODO: Save the final jpeg file
}