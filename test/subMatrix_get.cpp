#include <lib/imgMatrix.hpp>
#include <lib/subsampMatrix.hpp>
#include <consts/consts.hpp>
#include <smartAssert.hpp>
#include <functional>
#include <vector>

SubMatrix<unsigned char> prepareData(unsigned char* arr)
{
    for(int i{1}; i < 48; ++i)
    {
        arr[i-1] = i;
    }
    ImgMatrix mat = ImgMatrix<unsigned char>(arr, 8, 2, 3);
    mat.printMatrix();
    SubMatrix subMat(mat, SubsamplingType::s411);
    /*
        (1 4 7 10 7 8) (13 16 19 22 19 20)
        (25 28 31 34 31 32) (37 40 43 46 43 44)
    */
    std::vector<unsigned char> values = {1,4,7,10,7,8, 13,16,19,22,19,20, 25,28,31,34,31,32, 37,40,43,46,43,44};
    for(int i{0}; i < subMat.size(); ++i)
    {
        subMat[i] = values.at(i);
    }
    subMat.printMatrix();
    return subMat;
}

int main()
{
    unsigned char* arr = new unsigned char[48];
    auto subMat = prepareData(arr);

    // General
    M_Assert(subMat.size() == 24, "size_t size(): New matrix size is incorrect");
    M_Assert(subMat.getOrigWidth() == 8, "uint16_t getOrigWidth(): Incorrect value");
    M_Assert(subMat.getOrigHeight() == 2, "uint16_t getOrigHeight(): Incorrect value");
    M_Assert(subMat.getWidth() == 2, "uint16_t getWidth(): Incorrect value");
    M_Assert(subMat.getHeight() == 2, "uint16_t getHeight(): Incorrect value");
    M_Assert(subMat.getF_height() == 2, "uint16_t getF_height(): Incorrect value");
    M_Assert(subMat.getF_width() == 12, "uint16_t getF_width(): Incorrect value");
    M_Assert(subMat.getChannels() == 3, "uint8_t getChannels(): Incorrect value");
    M_Assert(subMat.getGsX() == 4, "uint8_t getGsX(): Incorrect value");
    M_Assert(subMat.getGsY() == 1, "uint8_t getGsY(): Incorrect value");
    M_Assert(subMat.getType() == SubsamplingType::s411, "SubsamplingType getType(): Incorrect value");

    /*
        (1 4 7 10 7 8) (13 16 19 22 19 20)
        (25 28 31 34 31 32) (37 40 43 46 43 44)
    */
    subMat.printMatrix();
    // T& operator[](size_t index);
    M_Assert(subMat[6] == 13, "T& operator[](size_t index) [get]: returned incorrect value");
    subMat[6] = 3;
    M_Assert(subMat[6] == 3, "T& operator[](size_t index) [set]: set incorrect value");
    /*
        (1 4 7 10 7 8) (3 16 19 22 19 20)
        (25 28 31 34 31 32) (37 40 43 46 43 44)
    */

    // T& operator()(size_t index);
    M_Assert(subMat(12) == 25, "T& operator()(size_t index) [get]: returned incorrect value");
    subMat(12) = 0;
    M_Assert(subMat(12) == 0, "T& operator()(size_t index) [set]: set incorrect value");
    /*
        (1 4 7 10 7 8) (3 16 19 22 19 20)
        (0 28 31 34 31 32) (37 40 43 46 43 44)
    */

    // T& operator()(size_t x, size_t y);
    M_Assert(subMat(11,1) == 44, "T& operator()(size_t x, size_t y) [get]: returned incorrect value");
    subMat(11,1) = 9;
    M_Assert(subMat(11,1) == 9, "T& operator()(size_t x, size_t y) [set]: set incorrect value");
    /*
        (1 4 7 10 7 8) (3 16 19 22 19 20)
        (0 28 31 34 31 32) (37 40 43 46 43 9)
    */

    // T& operator()(size_t x, size_t y, uint8_t ch);
    M_Assert(subMat(1,1,2) == 43, "T& operator()(size_t x, size_t y, uint8_t ch) [get]: returned incorrect value");
    subMat(1,1,2) = 6;
    M_Assert(subMat(1,1,2) == 6, "T& operator()(size_t x, size_t y, uint8_t ch) [get]: set incorrect value");
    /*
        (1 4 7 10 7 8) (3 16 19 22 19 20)
        (0 28 31 34 31 32) (37 40 6 46 43 9)
    */

    // const std::vector<T&> getGroup(size_t x, size_t y);
    auto group = subMat.getGroup(0,1);
    M_Assert(std::tuple_size<decltype(group)>::value == 3, "std::vector<T&> getGroup(size_t x, size_t y): returned vector with incorrect size");
    auto Y = std::get<0>(group);
    auto Cb = std::get<1>(group);
    auto Cr = std::get<2>(group);
    // Test Y
    M_Assert(Y.size() == 4, "std::vector<T&> getPixel(size_t x, size_t y): returned Y vector with incorrect size");
    M_Assert(Y[0] == 0 && Y[1] == 28 && Y[2] == 31 && Y[3] == 34, "std::vector<T&> getPixel(size_t x, size_t y): returned Y vector with an incorrect value");
    Y[1].get() = 10;
    M_Assert(subMat(0,1,1) == 10, "std::vector<T&> getPixel(size_t x, size_t y): Y reference error");
    /*
        (1 4 7 10 7 8) (3 16 19 22 19 20)
        (0 10 31 34 31 32) (37 40 6 46 43 9)
    */
    // Test Cb
    M_Assert(Cb == 31, "std::vector<T&> getPixel(size_t x, size_t y): returned incorrect Cb value");
    Cb.get() = 255;
    M_Assert(subMat(0,1,4) == 255, "std::vector<T&> getPixel(size_t x, size_t y): Cb reference error");
    /*
        (1 4 7 10 7 8) (3 16 19 22 19 20)
        (0 10 31 34 255 32) (37 40 6 46 43 9)
    */
    // Test Cr
    M_Assert(Cr == 32, "std::vector<T&> getPixel(size_t x, size_t y): returned incorrect Cb value");
    Cr.get() = 0;
    M_Assert(subMat(0,1,5) == 0, "std::vector<T&> getPixel(size_t x, size_t y): Cb reference error");
    /*
        (1 4 7 10 7 8) (3 16 19 22 19 20)
        (0 10 31 34 255 0) (37 40 6 46 43 9)
    */

    //const std::tuple<ref_wrap<T>, ref_wrap<T>, ref_wrap<T>> getPixel(size_t x, size_t y);
    auto pixel = subMat.getPixel(6,1);
    auto Y_p = std::get<0>(pixel);
    auto Cb_p = std::get<1>(pixel);
    auto Cr_p = std::get<2>(pixel);
    M_Assert(Y_p == 6, "std::tuple<ref_wrap<T>, ref_wrap<T>, ref_wrap<T>> getPixel(size_t x, size_t y) : returned incorrect Y value");
    M_Assert(Cb_p == 43, "std::tuple<ref_wrap<T>, ref_wrap<T>, ref_wrap<T>> getPixel(size_t x, size_t y) : returned incorrect Cb value");
    M_Assert(Cr_p == 9, "std::tuple<ref_wrap<T>, ref_wrap<T>, ref_wrap<T>> getPixel(size_t x, size_t y) : returned incorrect Cb value");

    delete[] arr;
    return 0;
}