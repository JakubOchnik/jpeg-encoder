#pragma once
#include <cstdint>
#include <exception>
#include <vector>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <cstring>
#include <lib/baseMatrix.hpp>

template<typename T>
class ImgMatrix : public BaseMatrix<T>
{
public:

    ImgMatrix(T* data, uint16_t w, uint16_t h, uint8_t ch, bool copy = false);
    ImgMatrix();
    ~ImgMatrix();

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
    // Functions which append a duplicate of last row/column n times
    void duplicateLastRow(int n = 1);
    void duplicateLastColumn(int n = 1);

};

template<typename T>
inline ImgMatrix<T>::ImgMatrix(T* data, uint16_t w, uint16_t h, uint8_t ch, bool copy): BaseMatrix<T>(w,h,ch)
{
    f_width = width * channels;
    f_height = height;
    length = width * height * channels;
    if(copy)
    {
        raw_data = new T[length];
        memcpy(raw_data, data, length * sizeof(T));
        memoryManaged = true;
    }
    else
    {
        raw_data = data;
        memoryManaged = false;
    }
}

template<typename T>
inline ImgMatrix<T>::ImgMatrix(): BaseMatrix<T>()
{
    f_width = f_height = height = width = length = channels = 0;
    raw_data = nullptr;
    memoryManaged = false;
}


template<typename T>
inline T& ImgMatrix<T>::operator()(size_t index)
{
    if(index > length - 1)
    {
        throw std::runtime_error("Array index too large");
    }
    return raw_data[index];
}

template<typename T>
inline T& ImgMatrix<T>::operator()(size_t x, size_t y)
{
    if(x > f_width - 1 || y > height - 1)
    {
        throw std::runtime_error("Index out of bounds");
    }

    size_t idx = x + y * f_width;
    return raw_data[idx];
}

template<typename T>
inline T& ImgMatrix<T>::operator[](size_t index)
{
    if(index > length - 1)
    {
        throw std::runtime_error("Array index too large");
    }
    return raw_data[index];
}

template<typename T>
inline T& ImgMatrix<T>::operator()(size_t x, size_t y, uint8_t ch)
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
inline const std::vector<std::reference_wrapper<T>> ImgMatrix<T>::getPixel(size_t x, size_t y)
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
inline T& ImgMatrix<T>::getPixelCh(size_t x, size_t y, uint8_t ch)
{
    // Returns a vector of REFERENCES to each particular channel
    // R, G, B; Y, Cb, Cr; etc.
    size_t base_idx = (x + y * width) * channels;
    return raw_data[base_idx + ch];
}

template<typename T>
inline void ImgMatrix<T>::duplicateLastRow(int n)
{
    size_t new_length = length + f_width * n;
    T* raw_new = nullptr;
    try
    {
        raw_new = new T[new_length];
    }
    catch(...)
    {
        throw;
    }

    // copy existing array
    memcpy(raw_new,raw_data,length*sizeof(T));
    // copy last row
    size_t idx = 0 + f_width * (height - 1);
    size_t dst_idx = length;
    for(int i{0}; i<n; ++i)
    {
        memcpy(&raw_new[dst_idx], &raw_data[idx], f_width * sizeof(T));
        dst_idx += f_width;
    }
    
    // Assume, that current data is allocated on the heap.
    if(memoryManaged)
        delete [] raw_data;

    memoryManaged = true;
    raw_data = raw_new;
    height += n;
    length += n * f_width;
}

template<typename T>
inline void ImgMatrix<T>::duplicateLastColumn(int n)
{
    size_t new_length = length + n * height * channels;
    T* raw_new = nullptr;
    try
    {
        raw_new = new T[new_length];
    }
    catch(...)
    {
        throw;
    }
    size_t src_idx = 0;
    size_t dst_idx = 0;
    for(int y{0}; y < height; ++y)
    {
        // Append existing row
        memcpy(&raw_new[dst_idx], &raw_data[src_idx], f_width * sizeof(T));
        dst_idx += f_width;
        src_idx += f_width;
        // Append columns
        for(int i{0}; i < n; ++i)
        {
            memcpy(&raw_new[dst_idx], &raw_data[src_idx - channels], channels * sizeof(T));
            dst_idx += channels;
        }
    }

        // Assume, that current data is allocated on the heap.
    if(memoryManaged)
        delete [] raw_data;

    memoryManaged = true;
    raw_data = raw_new;
    width += n;
    f_width += n * channels;
    length += n * height * channels;
}

template<typename T>
inline void ImgMatrix<T>::printMatrix()
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
inline ImgMatrix<T>::~ImgMatrix()
{
    if(memoryManaged)
        delete [] raw_data;
}