#pragma once
#include <cstdint>
#include <exception>
#include <vector>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <cstring>

template<typename T>
class BaseMatrix
{
protected:
    T* raw_data;

    // Pixel dimensions
    uint16_t width;
    uint16_t height;
    // Real dimensions (f_height = height, f_width = width*channels)
    uint16_t f_width;
    uint16_t f_height;
    // Number of channels in an image (usually 3)
    uint8_t channels;
    // Real length of data in the array
    size_t length;
    // Flag which indicates whether a raw_data array
    // is memory managed within the class.
    bool memoryManaged;
public:

    BaseMatrix(uint16_t w, uint16_t h, uint8_t ch, bool mem);
    BaseMatrix();

    // Pixel imensions of an image
    uint16_t getWidth() const;
    uint16_t getHeight() const;
    // F_width is a 'real' width of matrix (multiplied by number of channels)
    uint16_t getF_width() const;
    uint16_t getF_height() const;
    uint8_t getChannels() const;

    // Real size getter
    size_t size() const;

    // Prints a matrix (cout)
    virtual void printMatrix();

};

template<typename T>
inline BaseMatrix<T>::BaseMatrix(uint16_t w, uint16_t h, uint8_t ch, bool mem): width(w), height(h), channels(ch), memoryManaged(mem)
{
    f_width = width * channels;
    f_height = height;
    length = f_width * height;
}

template<typename T>
inline BaseMatrix<T>::BaseMatrix()
{
    f_width = f_height = height = width = length = channels = 0;
    raw_data = nullptr;
    memoryManaged = false;
}

template<typename T>
inline uint16_t BaseMatrix<T>::getWidth() const
{
    return width;
}

template<typename T>
inline uint16_t BaseMatrix<T>::getHeight() const
{
    return height;
}

template<typename T>
inline uint16_t BaseMatrix<T>::getF_width() const
{
    return f_width;
}
template<typename T>
inline uint16_t BaseMatrix<T>::getF_height() const
{
    return f_height;
}

template<typename T>
inline uint8_t BaseMatrix<T>::getChannels() const
{
    return channels;
}

template<typename T>
size_t BaseMatrix<T>::size() const
{
    return length;
}

template<typename T>
inline void BaseMatrix<T>::printMatrix()
{
    for(int i{0}; i<height; ++i)
    {
        for(int j{0}; j<f_width; ++j)
        {
            std::cout << static_cast<int>(raw_data[j + f_width*j]) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}