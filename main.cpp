#include <encoder.hpp>
#include <opencv2/opencv.hpp>
int main(int argc, char* argv[])
{
    using namespace cv;
    Mat img = imread("../../Test_Image.png");
    Encoder enc(img.data, img.cols, img.rows);
    enc.encodeImg();
    return 0;
}