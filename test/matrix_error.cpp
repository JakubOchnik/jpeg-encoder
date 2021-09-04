#include <lib/matrix.hpp>
#include <smartAssert.hpp>
#include <functional>
#include <vector>
#include <iostream>

int main()
{
    char arr[] = {1,2,3,4,5,6,7,8,9,10,11,12};
    Matrix mat = Matrix<char>(arr, 2, 2, 3);

    // Getters:
    // T& operator[](size_t index)
    bool caught = false;
    try
    {
        mat[12] = 5;
    }
    catch(const std::exception& e)
    {
        caught = true;
        M_Assert(!strcmp(e.what(),"Array index too large"),"T& operator[](size_t index) [ex]: returned incorrect exception" );
        std::cerr << e.what() << '\n';
    }
    M_Assert(caught, "T& operator[](size_t index) [n_ex]: did not throw exception");
    caught = false;

    // T& operator()(size_t index);
    try
    {
        mat(12) = 5;
    }
    catch(const std::exception& e)
    {
        caught = true;
        M_Assert(!strcmp(e.what(), "Array index too large"),"T& operator()(size_t index) [ex]: returned incorrect exception" );
        std::cerr << e.what() << '\n';
    }
    M_Assert(caught, "T& operator()(size_t index) [n_ex]: did not throw exception");
    caught = false;

    // T& operator()(size_t x, size_t y);
    // incorrect x
    try
    {
        mat(6, 1) = 12;
    }
    catch(const std::exception& e)
    {
        caught = true;
        M_Assert(!strcmp(e.what(), "Index out of bounds"),"T& operator()(size_t x, size_t y) [ex]: returned incorrect exception (x)" );
        std::cerr << e.what() << '\n';
    }
    M_Assert(caught, "T& operator()(size_t x, size_t y) [n_ex]: did not throw exception (x)");
    caught = false;
    // incorrect y
    try
    {
        mat(1, 2) = 12;
    }
    catch(const std::exception& e)
    {
        caught = true;
        M_Assert(!strcmp(e.what(), "Index out of bounds"),"T& operator()(size_t x, size_t y) [ex]: returned incorrect exception (y)" );
        std::cerr << e.what() << '\n';
    }
    M_Assert(caught, "T& operator()(size_t x, size_t y) [n_ex]: did not throw exception (y)");
    caught = false;
    
    // T& operator()(size_t x, size_t y, uint8_t ch);
    // incorrect number of channels
    try
    {
        mat(0, 0, 4) = 12;
    }
    catch(const std::exception& e)
    {
        caught = true;
        M_Assert(!strcmp(e.what(), "Channel number out of range"),"T& operator()(size_t x, size_t y, uint8_t ch) [ex]: returned incorrect exception (channels)" );
        std::cerr << e.what() << '\n';
    }
    M_Assert(caught, "T& operator()(size_t x, size_t y, uint8_t ch) [n_ex]: did not throw exception (channels)");
    caught = false;
    // incorrect x
    try
    {
        mat(2, 1, 2) = 12;
    }
    catch(const std::exception& e)
    {
        caught = true;
        M_Assert(!strcmp(e.what(), "Index out of bounds"),"T& operator()(size_t x, size_t y, uint8_t ch) [ex]: returned incorrect exception (x)" );
        std::cerr << e.what() << '\n';
    }
    M_Assert(caught, "T& operator()(size_t x, size_t y, uint8_t ch) [n_ex]: did not throw exception (x)");
    caught = false;
    // incorrect y
    try
    {
        mat(0, 2, 2) = 12;
    }
    catch(const std::exception& e)
    {
        caught = true;
        M_Assert(!strcmp(e.what(), "Index out of bounds"),"T& operator()(size_t x, size_t y, uint8_t ch) [ex]: returned incorrect exception (y)" );
        std::cerr << e.what() << '\n';
    }
    M_Assert(caught, "T& operator()(size_t x, size_t y, uint8_t ch) [n_ex]: did not throw exception (y)");
    caught = false;

    // const std::vector<T&> getPixel(size_t x, size_t y);
    // incorrect x
    try
    {
        mat.getPixel(2,1);
    }
    catch(const std::exception& e)
    {
        caught = true;
        M_Assert(!strcmp(e.what(), "Index out of bounds"),"std::vector<T&> getPixel(size_t x, size_t y) [ex]: returned incorrect exception (x)" );
        std::cerr << e.what() << '\n';
    }
    M_Assert(caught, "std::vector<T&> getPixel(size_t x, size_t y) [n_ex]: did not throw exception (x)");
    caught = false;
    // incorrect y
    try
    {
        mat.getPixel(0,2);
    }
    catch(const std::exception& e)
    {
        caught = true;
        M_Assert(!strcmp(e.what(), "Index out of bounds"),"std::vector<T&> getPixel(size_t x, size_t y) [ex]: returned incorrect exception (y)" );
        std::cerr << e.what() << '\n';
    }
    M_Assert(caught, "std::vector<T&> getPixel(size_t x, size_t y) [n_ex]: did not throw exception (y)");

    return 0;
}