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
    ImgMatrix(uint16_t w, uint16_t h, uint8_t ch);
    ImgMatrix(const ImgMatrix<T>& other);
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
    std::vector<std::reference_wrapper<T>> getPixel(size_t x, size_t y);
    T& getPixelCh(size_t x, size_t y, uint8_t ch);

    // Prints a matrix (cout)
    void printMatrix() override;
    // Functions which append a duplicate of last row/column n times
    void duplicateLastRow(int n = 1);
    void duplicateLastColumn(int n = 1);

};

template<typename T>
inline ImgMatrix<T>::ImgMatrix(T* data, uint16_t w, uint16_t h, uint8_t ch, bool copy): BaseMatrix<T>(w, h, ch, copy)
{
    if(copy)
    {
        this->raw_data = new T[this->length];
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
inline ImgMatrix<T>::ImgMatrix(const ImgMatrix<T>& other)
{
    this->width = other.getWidth();
    this->height = other.getHeight();
    this->f_width = other.getF_width();
    this->f_height = other.getF_height();
    this->length = other.size();
    this->channels = other.getChannels();

    this->raw_data = new T[this->length];
    // explicitly copy data
    memcpy(this->raw_data, other.getRawPointer(), this->length*sizeof(T));
    this->memoryManaged = true;
}


template<typename T>
inline ImgMatrix<T>::ImgMatrix(uint16_t w, uint16_t h, uint8_t ch): BaseMatrix<T>(w, h, ch, true)
{
    this->raw_data = new T[this->length];
}

template<typename T>
inline ImgMatrix<T>::ImgMatrix(): BaseMatrix<T>()
{
}


template<typename T>
inline T& ImgMatrix<T>::operator()(size_t index)
{
    if(index > this->length - 1)
    {
        throw std::runtime_error("Array index too large");
    }
    return this->raw_data[index];
}

template<typename T>
inline T& ImgMatrix<T>::operator()(size_t x, size_t y)
{
    if(x > this->f_width - 1 || y > this->height - 1)
    {
        throw std::runtime_error("Index out of bounds");
    }

    size_t idx = x + y * this->f_width;
    return this->raw_data[idx];
}

template<typename T>
inline T& ImgMatrix<T>::operator[](size_t index)
{
    if(index > this->length - 1)
    {
        throw std::runtime_error("Array index too large");
    }
    return this->raw_data[index];
}

template<typename T>
inline T& ImgMatrix<T>::operator()(size_t x, size_t y, uint8_t ch)
{
    if(x > this->width - 1 || y > this->height - 1)
    {
        throw std::runtime_error("Index out of bounds");
    }
    if(ch > this->channels)
    {
        throw std::runtime_error("Channel number out of range");
    }
    return getPixelCh(x,y,ch);
}

template<typename T>
inline std::vector<std::reference_wrapper<T>> ImgMatrix<T>::getPixel(size_t x, size_t y)
{
    if(x > this->width - 1 || y > this->height - 1)
    {
        throw std::runtime_error("Index out of bounds");
    }
    // Returns a vector of REFERENCES to each particular channel
    // R, G, B; Y, Cb, Cr; etc.
    size_t base_idx = (x + y * this->width) * this->channels;
    std::vector<std::reference_wrapper<T>> refs;
    for(int i{0}; i < this->channels; ++i)
    {
        refs.emplace_back(this->raw_data[base_idx + i]);
    }
    return refs;
}

template<typename T>
inline T& ImgMatrix<T>::getPixelCh(size_t x, size_t y, uint8_t ch)
{
    // Returns a vector of REFERENCES to each particular channel
    // R, G, B; Y, Cb, Cr; etc.
    size_t base_idx = (x + y * this->width) * this->channels;
    return this->raw_data[base_idx + ch];
}

template<typename T>
inline void ImgMatrix<T>::duplicateLastRow(int n)
{
    size_t new_length = this->length + this->f_width * n;
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
    memcpy(raw_new, this->raw_data, this->length*sizeof(T));
    // copy last row
    size_t idx = 0 + this->f_width * (this->height - 1);
    size_t dst_idx = this->length;
    for(int i{0}; i<n; ++i)
    {
        memcpy(&raw_new[dst_idx], &this->raw_data[idx], this->f_width * sizeof(T));
        dst_idx += this->f_width;
    }
    
    // Assume, that current data is allocated on the heap.
    if(this->memoryManaged)
        delete [] this->raw_data;

    this->memoryManaged = true;
    this->raw_data = raw_new;
    this->height += n;
    this->length += n * this->f_width;
}

template<typename T>
inline void ImgMatrix<T>::duplicateLastColumn(int n)
{
    size_t new_length = this->length + n * this->height * this->channels;
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
    for(int y{0}; y < this->height; ++y)
    {
        // Append existing row
        memcpy(&raw_new[dst_idx], &this->raw_data[src_idx], this->f_width * sizeof(T));
        dst_idx += this->f_width;
        src_idx += this->f_width;
        // Append columns
        for(int i{0}; i < n; ++i)
        {
            memcpy(&raw_new[dst_idx], &this->raw_data[src_idx - this->channels], this->channels * sizeof(T));
            dst_idx += this->channels;
        }
    }

        // Assume, that current data is allocated on the heap.
    if(this->memoryManaged)
        delete [] this->raw_data;

    this->memoryManaged = true;
    this->raw_data = raw_new;
    this->width += n;
    this->f_width += n * this->channels;
    this->length += n * this->height * this->channels;
}

template<typename T>
inline void ImgMatrix<T>::printMatrix()
{
    for(int i{0}; i < this->height; ++i)
    {
        for(int j{0}; j < this->f_width; ++j)
        {
            if(j > 0 && j%3==0)
                std::cout << "|";
            std::cout << static_cast<int>((*this)(j,i)) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

template<typename T>
inline ImgMatrix<T>::~ImgMatrix()
{
    if(this->memoryManaged)
        delete [] this->raw_data;
}