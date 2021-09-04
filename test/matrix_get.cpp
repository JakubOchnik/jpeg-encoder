#include <lib/matrix.hpp>
#include <smartAssert.hpp>
#include <functional>
#include <vector>


int main()
{
    char arr[] = {1,2,3,4,5,6,7,8,9,10,11,12};
    Matrix mat = Matrix<char>(arr, 2, 2, 3);

    // General
    M_Assert(mat.size() == 12, "size_t size(): New matrix size is incorrect");
    M_Assert(mat.getWidth() == 2, "uint16_t getWidth(): Incorrect value");
    M_Assert(mat.getHeight() == 2, "uint16_t getHeight(): Incorrect value");
    M_Assert(mat.getF_height() == 2, "uint16_t getF_height(): Incorrect value");
    M_Assert(mat.getF_width() == 6, "uint16_t getF_width(): Incorrect value");
    M_Assert(mat.getChannels() == 3, "uint8_t getChannels(): Incorrect value");
    // T& operator[](size_t index);
    M_Assert(mat[3] == 4, "T& operator[](size_t index) [get]: returned incorrect value");
    mat[3] = 3;
    M_Assert(mat[3] == 3, "T& operator[](size_t index) [set]: set incorrect value");
    // T& operator()(size_t index);
    M_Assert(mat(0) == 1, "T& operator()(size_t index) [get]: returned incorrect value");
    mat(0) = 0;
    M_Assert(mat(0) == 0, "T& operator()(size_t index) [set]: set incorrect value");
    // T& operator()(size_t x, size_t y);
    M_Assert(mat(3,1) == 10, "T& operator()(size_t x, size_t y) [get]: returned incorrect value");
    mat(3,1) = 9;
    M_Assert(mat(3,1) == 9, "T& operator()(size_t x, size_t y) [set]: set incorrect value");
    // T& operator()(size_t x, size_t y, uint8_t ch);
    M_Assert(mat(0,1,0) == 7, "T& operator()(size_t x, size_t y, uint8_t ch) [get]: returned incorrect value");
    mat(0,1,0) = 6;
    M_Assert(mat(0,1,0) == 6, "T& operator()(size_t x, size_t y, uint8_t ch) [get]: set incorrect value");

    // const std::vector<T&> getPixel(size_t x, size_t y);
    auto pixel = mat.getPixel(1,1);
    M_Assert(pixel.size() == 3, "std::vector<T&> getPixel(size_t x, size_t y): returned vector with incorrect size");
    M_Assert(pixel[1] == 11, "std::vector<T&> getPixel(size_t x, size_t y): returned vector with incorrect value");
    pixel[1].get() = 10;
    M_Assert(mat(1,1,1) == 10, "std::vector<T&> getPixel(size_t x, size_t y): reference error");
    return 0;
}