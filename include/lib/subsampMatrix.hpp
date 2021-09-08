/*
#pragma once
#include <cstdint>
#include <exception>
#include <vector>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <cstring>
#include <lib/baseMatrix.hpp>
#include <lib/imgMatrix.hpp>
#include <consts/consts.hpp>

template<typename T>
class SubMatrix : public BaseMatrix<T>
{

public:

    SubMatrix(T* data, uint16_t w, uint16_t h, uint8_t ch, bool copy = false);
    SubMatrix(const BaseMatrix<T>&);
    SubMatrix();
    ~SubMatrix();

    // Raw, "flat" data index operators
    virtual T& operator[](size_t index);
    virtual T& operator()(size_t index);
    // Raw, 2D operator (x is counted with channels, y is the same as pixel height)
    virtual T& operator()(size_t x, size_t y);

    // Pixel index operator with channel selector (range: 0-channels)
    T& operator()(size_t x, size_t y, uint8_t ch);

    // This function returns a vector of REFERENCES to components of pixel
    const std::vector<std::reference_wrapper<T>> getPixel(size_t x, size_t y);
    T& getPixelCh(size_t x, size_t y, uint8_t ch);

    // Prints a matrix (cout)
    void printMatrix() override;

};

template<typename T>
inline SubMatrix<T>::SubMatrix(T* data, uint16_t w, uint16_t h, uint8_t ch, bool copy): BaseMatrix<T>(w,h,ch)
{
    if(copy)
    {
        raw_data = new T[length];
        memcpy(this->raw_data, data, this->length * sizeof(T));
        this->memoryManaged = true;
    }
    else
    {
        this->raw_data = data;
        this->memoryManaged = false;
    }
}

template<typename T>
inline SubMatrix<T>::SubMatrix(): BaseMatrix<T>()
{
}

template<typename T>
inline SubMatrix<T>::SubMatrix(const BaseMatrix<T>& img): BaseMatrix<T>(img.getWidth(), img.getHeight(), img.getChannels())
{
    // Assume it's an image

}

template<typename T>
inline T& SubMatrix<T>::operator()(size_t index)
{
    if(index > length - 1)
    {
        throw std::runtime_error("Array index too large");
    }
    return raw_data[index];
}

template<typename T>
inline T& SubMatrix<T>::operator()(size_t x, size_t y)
{
    if(x > f_width - 1 || y > height - 1)
    {
        throw std::runtime_error("Index out of bounds");
    }

    size_t idx = x + y * f_width;
    return raw_data[idx];
}

template<typename T>
inline T& SubMatrix<T>::operator[](size_t index)
{
    if(index > length - 1)
    {
        throw std::runtime_error("Array index too large");
    }
    return raw_data[index];
}

template<typename T>
inline T& SubMatrix<T>::operator()(size_t x, size_t y, uint8_t ch)
{
    if(x > width - 1 || y > height - 1)
    {
        throw std::runtime_error("Index out of bounds");
    }
    if(ch > channels)
    {
        throw std::runtime_error("Channel number out of range");
    }
    return getPixelCh(x,y,ch);
}

template<typename T>
inline const std::vector<std::reference_wrapper<T>> SubMatrix<T>::getPixel(size_t x, size_t y)
{
    if(x > width - 1 || y > height - 1)
    {
        throw std::runtime_error("Index out of bounds");
    }
    // Returns a vector of REFERENCES to each particular channel
    // R, G, B; Y, Cb, Cr; etc.
    size_t base_idx = (x + y * width) * channels;
    std::vector<std::reference_wrapper<T>> refs;
    for(int i{0}; i<channels; ++i)
    {
        refs.emplace_back(raw_data[base_idx + i]);
    }
    return refs;
}

template<typename T>
inline T& SubMatrix<T>::getPixelCh(size_t x, size_t y, uint8_t ch)
{
    // Returns a vector of REFERENCES to each particular channel
    // R, G, B; Y, Cb, Cr; etc.
    size_t base_idx = (x + y * width) * channels;
    return raw_data[base_idx + ch];
}

template<typename T>
inline void SubMatrix<T>::printMatrix()
{
    for(int i{0}; i<height; ++i)
    {
        for(int j{0}; j<f_width; ++j)
        {
            if(j > 0 && j%3==0)
                std::cout << "|";
            //std::cout << static_cast<int>((*this)(j,i)) << " ";
            getChannels();
            operator[](0);
            //operator()(j,i);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

template<typename T>
inline SubMatrix<T>::~SubMatrix()
{
    if(memoryManaged)
        delete [] raw_data;
}
*/