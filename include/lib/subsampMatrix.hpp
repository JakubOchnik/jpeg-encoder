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
    SubMatrix();
    ~SubMatrix();

    // Constructs an empty Subsampled Matrix (to be filled in the future)
    SubMatrix(const BaseMatrix<T>&, SubsamplingType);

    // Copy constructor
    SubMatrix(const SubMatrix<T>& other);

    // Constructor used mainly for testing purposes
    SubMatrix(const std::vector<std::vector<T>>& srcArr, SubsamplingType _type, const uint8_t channels);

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

    // Pixel group getter with channel selector (usual range for 4-pixel group: 0-6 (Y Y Y Y Cb Cr))
    T& getGrComp(size_t x, size_t y, uint8_t comp);

    // REAL pixel operator with channel selector
    T& operator()(size_t x, size_t y, uint8_t ch);
    // This function returns a vector of REFERENCES to components of a pixel group
    const std::tuple<std::vector<ref_wrap<T>>, ref_wrap<T>, ref_wrap<T>> getGroup(size_t x, size_t y);
    // This function returns a vector of REFERENCES to Y, Cb, Cr channels of a REAL pixel
    const std::vector<ref_wrap<T>> getPixel(size_t x, size_t y);

    // Prints a matrix (cout)
    void printMatrix() override;

    // Duplicates the last row N times
    void duplicateLastRow(const int N);

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
        this->width = ceil(this->orig_width / gs_x);
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
inline T& SubMatrix<T>::getGrComp(size_t x, size_t y, uint8_t comp)
{
    if(x > this->width - 1 || y > this->height - 1)
    {
        throw std::runtime_error("Index out of bounds");
    }
    size_t base_idx{};
    // TODO: CONSIDER 422 AND 420
    if(this->type == SubsamplingType::s411)
    {   
        // Group_length = number of Y components (group pixels) + Cb + Cr
        if(comp > this->gs_x + 2 - 1) 
        {
            throw std::runtime_error("Channel number out of range");
        }
        base_idx = (x + y * this->width) * (this->gs_x + 2);
        return this->raw_data[base_idx + comp];
    }
    return this->raw_data[base_idx + comp];
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
inline T& SubMatrix<T>::operator()(size_t x, size_t y, uint8_t ch)
{
    if(x > this->orig_width - 1 || y > this->height - 1)
    {
        throw std::runtime_error("Coordinate(s) out of bounds");
    }
    if(ch > this->channels)
    {
        throw std::runtime_error("Pixel channel out of bounds");
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
        // As x_idx is intentionally truncated, calculate the offset using
        // the original index.
        if(ch == 0)
        {
            auto offset = x % 4;
            return getGrComp(x_idx, y, offset);
        }
        else
        {
            return getGrComp(x_idx, y, 3 + ch);
        }
    }
}

template<typename T>
inline const std::vector<ref_wrap<T>> SubMatrix<T>::getPixel(size_t x, size_t y)
{
    if(x > this->orig_width - 1 || y > this->height - 1)
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
        // As x_idx is intentionally truncated, calculate the offset using
        // the original index.
        auto offset = x % 4;
        auto Y = ref_wrap<T>(getGrComp(x_idx, y, offset));
        auto Cb = ref_wrap<T>(getGrComp(x_idx, y, 4));
        auto Cr = ref_wrap<T>(getGrComp(x_idx, y, 5));

        std::vector<ref_wrap<T>> out = {Y, Cb, Cr};
        return out;
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

template<typename T>
inline void SubMatrix<T>::duplicateLastRow(const int N)
{
    size_t new_length = this->length + this->f_width * N;
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
    for(int i{0}; i<N; ++i)
    {
        memcpy(&raw_new[dst_idx], &this->raw_data[idx], this->f_width * sizeof(T));
        dst_idx += this->f_width;
    }
    
    // Assume, that current data is allocated on the heap.
    if(this->memoryManaged)
        delete [] this->raw_data;

    this->memoryManaged = true;
    this->raw_data = raw_new;
    this->height += N;
    this->length += N * this->f_width;
}

template<typename T>
SubMatrix<T>::SubMatrix(const std::vector<std::vector<T>>& srcArr, SubsamplingType _type, const uint8_t channels)
{
    this->channels = channels;

    // If 4:1:1
    this->gs_x = 4;
    this->gs_y = 1;

    this->f_width = srcArr[0].size();
    this->width = this->f_width / (gs_x + 2);
    this->height = srcArr.size();

    this->orig_height = this->height;
    this->orig_width =  this->width * gs_x;

    this->length = srcArr.size() * srcArr[0].size();
    this->raw_data = new T[this->length];
    this->type = _type;
    for(int i{0}; i < srcArr.size(); ++i)
    {
        memcpy(&this->raw_data[srcArr[0].size() * i], srcArr[i].data(), srcArr[0].size()*sizeof(T));
    }
    this->memoryManaged = true;
}
