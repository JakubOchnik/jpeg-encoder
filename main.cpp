#include <encoder.hpp>
#include <opencv2/opencv.hpp>
#include <lib/matrix.hpp>
int main(int argc, char* argv[])
{
    using namespace cv;
    Mat img = imread("../../Test_Image.png");
    Encoder enc(img.data, img.cols, img.rows, img);
    enc.encodeImg();
    return 0;
}