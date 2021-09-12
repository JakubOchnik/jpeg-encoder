#include <encoder.hpp>

Encoder::Encoder(unsigned char* img, int w, int h): raw_img(img), width(w), height(h)
{
    image = ImgMatrix(img, w, h, 3, false);
}

void Encoder::encodeImg()
{
    using namespace cv;
    // Convert colors from RGB -> YCbCr
    // TODO: Add other color spaces support (e.g. CMYK)
    size_t numOfBytes = width * height * 3;
    color::rgb2ycbcr(raw_img, numOfBytes);

    // Chroma subsampling
    auto subsampled = Subsampler::chromaSubsample411(image);

    std::cout << "Prev size:" << width * height * 3 << "\n Current size:" << subsampled.size() << "\n";
    std::cout << "\n";

    // SUBSAMPLING DEBUG:
    // Convert subsampled image back to YCbCr
    auto desampled = Debug::subsampled2ycbcr<unsigned char>(subsampled);
    // Convert subsampled and original image back to RGB color space
    unsigned char* raw_desampled = desampled.getRawPointer();
    color::ycbcr2rgb(raw_desampled,numOfBytes);
    color::ycbcr2rgb(raw_img,numOfBytes);

    // Preview both images
    Mat before_desampling(width, height, CV_8UC3, raw_img);
    cv::imshow("Before desampling", before_desampling);
    Mat after_desampling(width, height, CV_8UC3, raw_desampled);
    cv::imshow("Subsampled image", after_desampling);
    cv::waitKey(0);

    // TODO: Implement Discrete Cosine Transform & Quantization

    // TODO: Implement Delta Encoding

    // TODO: Implement Huffmann Encoding (add a header?)

    // TODO: Save the final jpeg file
}