#pragma once
#include <cstdint>
#include <exception>
#include <vector>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <cstring>
#include <tuple>
#include <cmath>
#include <lib/baseMatrix.hpp>
#include <lib/imgMatrix.hpp>
#include <consts/consts.hpp>

template<typename T>
using ref_wrap = std::reference_wrapper<T>;

template<typename T>
class SubMatrix : public BaseMatrix<T>
{
    // Pixel dimensions
    uint16_t orig_width;
    uint16_t orig_height;

    SubsamplingType type;
    uint8_t gs_x;
    uint8_t gs_y;


public:

    SubMatrix(const BaseMatrix<T>&, SubsamplingType);
    SubMatrix();
    ~SubMatrix();

    SubMatrix(const SubMatrix<T>& other);

    // Getters
    uint8_t getGsX() const;
    uint8_t getGsY() const;
    SubsamplingType getType() const;
    uint16_t getOrigWidth() const;
    uint16_t getOrigHeight() const;

    // Raw, "flat" data index operators
    virtual T& operator[](size_t index);
    virtual T& operator()(size_t index);
    // Raw, 2D operator (x is counted with channels, y is the same as pixel height)
    virtual T& operator()(size_t x, size_t y);

    // Pixel index operator with channel selector (range: 0-channels)
    T& operator()(size_t x, size_t y, uint8_t ch);

    // This function returns a vector of REFERENCES to components of a pixel group
    const std::tuple<std::vector<ref_wrap<T>>, ref_wrap<T>, ref_wrap<T>> getGroup(size_t x, size_t y);
    // This function returns a vector of REFERENCES to Y, Cb, Cr channels of a REAL pixel
    const std::tuple<ref_wrap<T>, ref_wrap<T>, ref_wrap<T>> getPixel(size_t x, size_t y);
    T& getPixelComponent(size_t x, size_t y, uint8_t comp);

    // Prints a matrix (cout)
    void printMatrix() override;

};

template<typename T>
inline SubMatrix<T>::SubMatrix(): BaseMatrix<T>()
{
}

template<typename T>
inline SubMatrix<T>::SubMatrix(const BaseMatrix<T>& img, SubsamplingType type): BaseMatrix<T>(img.getWidth(), img.getHeight(), img.getChannels(), true)
{
    this->type = type;
    if(type == SubsamplingType::s411)
    {
        this->gs_x = 4;
        this->gs_y = 1;
        this->orig_height = img.getHeight();
        this->orig_width = img.getWidth();
        this->width = ceil(this->orig_width / 4);
        this->length = ceil(this->width * this->height * (gs_x + 2));
        this->f_width = ceil(this->width * (gs_x + 2));
        this->raw_data = new T[this->length];
        this->memoryManaged = true;
    }
}

template<typename T>
inline SubMatrix<T>::SubMatrix(const SubMatrix<T>& other)
{
    this->orig_height = other.getOrigHeight();
    this->orig_width = other.getOrigWidth();
    this->width = other.getWidth();
    this->height = other.getHeight();
    this->length = other.size();
    this->f_width = other.getF_width();
    this->f_height = other.getF_height();
    this->channels = other.channels;
    this->gs_x = other.getGsX();
    this->gs_y = other.getGsY();
    this->type = other.getType();

    this->raw_data = new T[this->length];
    // explicitly copy data
    memcpy(this->raw_data, other.getRawPointer(), this->length*sizeof(T));
    this->memoryManaged = true;
}


template<typename T>
inline T& SubMatrix<T>::operator()(size_t index)
{
    if(index > this->length - 1)
    {
        throw std::runtime_error("Array index too large");
    }
    return this->raw_data[index];
}

template<typename T>
inline T& SubMatrix<T>::operator()(size_t x, size_t y)
{
    if(x > this->f_width - 1 || y > this->height - 1)
    {
        throw std::runtime_error("Index out of bounds");
    }

    size_t idx = x + y * this->f_width;
    return this->raw_data[idx];
}

template<typename T>
inline T& SubMatrix<T>::operator[](size_t index)
{
    if(index > this->length - 1)
    {
        throw std::runtime_error("Array index too large");
    }
    return this->raw_data[index];
}

template<typename T>
inline T& SubMatrix<T>::operator()(size_t x, size_t y, uint8_t comp)
{
    if(x > this->width - 1 || y > this->height - 1)
    {
        throw std::runtime_error("Index out of bounds");
    }
    // TODO: CONSIDER 422 AND 420
    if(this->type == SubsamplingType::s411)
    {   
        // Pixel_length = number of Y components (group pixels) + Cb + Cr
        if(comp > this->gs_x + 2 - 1) 
        {
            throw std::runtime_error("Channel number out of range");
        }
    }
    return getPixelComponent(x,y,comp);
}

template<typename T>
inline T& SubMatrix<T>::getPixelComponent(size_t x, size_t y, uint8_t comp)
{
    // Returns a vector of REFERENCES to each particular component
    // R, G, B; Y, Cb, Cr; etc.
    // TODO: CONSIDER 422 AND 420
    if(this->type == SubsamplingType::s411)
    {
        size_t base_idx = (x + y * this->width) * (this->gs_x + 2);
        return this->raw_data[base_idx + comp];
    }
}

template<typename T>
inline const std::tuple<std::vector<ref_wrap<T>>, ref_wrap<T>, ref_wrap<T>> SubMatrix<T>::getGroup(size_t x, size_t y)
{
    if(x > this->width - 1 || y > this->height - 1)
    {
        throw std::runtime_error("Index out of bounds");
    }
    // Returns a vector of REFERENCES to Y channels and references to Cb and Cr
    std::vector<ref_wrap<T>> y_refs;
    if(this->type == SubsamplingType::s411)
    {
        size_t base_idx = (x + y * this->width) * (this->gs_x + 2);
        for(int i{0}; i < this->gs_x; ++i)
        {
            y_refs.emplace_back(this->raw_data[base_idx + i]);
        }
        const auto Cb = ref_wrap<T>(this->raw_data[base_idx + this->gs_x]);
        const auto Cr = ref_wrap<T>(this->raw_data[base_idx + this->gs_x + 1]);
        return std::tuple(y_refs, Cb, Cr);
    }
    throw std::runtime_error("Something went wrong. Dirty workaround, will fix in the future.");
    return std::tuple(y_refs, ref_wrap<T>(this->raw_data[0]), ref_wrap<T>(this->raw_data[0]));
}

template<typename T>
inline const std::tuple<ref_wrap<T>, ref_wrap<T>, ref_wrap<T>> SubMatrix<T>::getPixel(size_t x, size_t y)
{
    if(x > this->orig_width - 1 || y > this->orig_height - 1)
    {
        throw std::runtime_error("Coordinate(s) out of bounds");
    }
    // TODO: Other modes
    if(this->type == SubsamplingType::s411)
    {
        // At first: localize the proper group
            /*
            0  1 2  3       4   5  6  7 
            (1 4 7 10 7 8) (13 16 19 22 19 20)
            8  9  10 11        12  13 14 15  
            (25 28 31 34 31 32) (37 40 43 46 43 44)
            */

        auto x_idx = static_cast<int>(x / 4);
        // (x_idx, y)
        auto offset = x_idx % 4;
        auto Y = ref_wrap<T>((*this)(x_idx, y, offset));
        auto Cb = ref_wrap<T>((*this)(x_idx, y, 4));
        auto Cr = ref_wrap<T>((*this)(x_idx, y, 5));
        return std::tuple(Y, Cb, Cr);
    }
}

template<typename T>
inline void SubMatrix<T>::printMatrix()
{
    // WARNING: works only with 411
    for(int i{0}; i < this->height; ++i)
    {
        for(int j{0}; j < this->f_width; ++j)
        {
            if(j > 0 && j%6==0)
                std::cout << "|";
            std::cout << static_cast<int>((*this)(j,i)) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

template<typename T>
inline SubMatrix<T>::~SubMatrix()
{
    if(this->memoryManaged)
        delete [] this->raw_data;
}

template<typename T>
uint8_t SubMatrix<T>::getGsX() const
{
    return this->gs_x;
}

template<typename T>
uint8_t SubMatrix<T>::getGsY() const
{
    return this->gs_y;
}

template<typename T>
SubsamplingType SubMatrix<T>::getType() const
{
    return this->type;
}

template<typename T>
uint16_t SubMatrix<T>::getOrigWidth() const
{
    return this->orig_width;
}

template<typename T>
uint16_t SubMatrix<T>::getOrigHeight() const
{
    return this->orig_height;
}
