#pragma once
#include <cstdint>
#include <exception>
#include <vector>
#include <cstdlib>
#include <functional>
#include <iostream>

template<typename T>
class Matrix
{
    T* raw_data;

    uint16_t width;
    uint16_t height;
    uint16_t f_width;
    uint16_t f_height;
    uint8_t channels;

    size_t length;
    bool outsideAlloc;
public:

    Matrix(T* data, uint16_t w, uint16_t h, uint8_t ch);
    Matrix() = delete;
    ~Matrix();

    uint16_t getWidth() const;
    uint16_t getHeight() const;
    uint16_t getF_width() const;
    uint16_t getF_height() const;
    uint8_t getChannels() const;


    T& operator[](size_t index);
    T& operator()(size_t index);
    T& operator()(size_t x, size_t y);
    T& operator()(size_t x, size_t y, uint8_t ch);
    const std::vector<std::reference_wrapper<T>> getPixel(size_t x, size_t y);
    T& getPixelCh(size_t x, size_t y, uint8_t ch);

    size_t size() const;
    void printMatrix();
    // TODO: Modifying size
    // apppendRow
    void duplicateLastRow();
    // appendColumn
    void duplicateLastColumn();

    void clean();
};


template<typename T>
inline uint16_t Matrix<T>::getWidth() const
{
    return width;
}

template<typename T>
inline uint16_t Matrix<T>::getHeight() const
{
    return height;
}

template<typename T>
inline uint16_t Matrix<T>::getF_width() const
{
    return f_width;
}
template<typename T>
inline uint16_t Matrix<T>::getF_height() const
{
    return f_height;
}

template<typename T>
inline uint8_t Matrix<T>::getChannels() const
{
    return channels;
}

template<typename T>
inline Matrix<T>::Matrix(T* data, uint16_t w, uint16_t h, uint8_t ch): raw_data(data), width(w), height(h), channels(ch)
{
    f_width = width * channels;
    f_height = height;
    length = width * height * channels;
    outsideAlloc = true;
}

template<typename T>
inline T& Matrix<T>::operator()(size_t index)
{
    if(index > length - 1)
    {
        throw std::runtime_error("Array index too large");
    }
    return raw_data[index];
}

template<typename T>
inline T& Matrix<T>::operator()(size_t x, size_t y)
{
    if(x > f_width - 1 || y > height - 1)
    {
        throw std::runtime_error("Index out of bounds");
    }

    size_t idx = x + y * f_width;
    return raw_data[idx];
}

template<typename T>
inline T& Matrix<T>::operator()(size_t x, size_t y, uint8_t ch)
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
inline T& Matrix<T>::operator[](size_t index)
{
    if(index > length - 1)
    {
        throw std::runtime_error("Array index too large");
    }
    return raw_data[index];
}

template<typename T>
inline const std::vector<std::reference_wrapper<T>> Matrix<T>::getPixel(size_t x, size_t y)
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
inline size_t Matrix<T>::size() const
{
    return length;
}

template<typename T>
inline T& Matrix<T>::getPixelCh(size_t x, size_t y, uint8_t ch)
{
    // Returns a vector of REFERENCES to each particular channel
    // R, G, B; Y, Cb, Cr; etc.
    size_t base_idx = (x + y * width) * channels;
    return raw_data[base_idx + ch];
}

template<typename T>
inline void Matrix<T>::duplicateLastRow()
{
    int y = height - 1;
    size_t new_length = length + f_width;
    T* raw_new = nullptr;
    try
    {
        raw_new = new T[new_length];
    }
    catch(const std::exception& e)
    {
        throw;
    }

    // copy existing array
    memcpy(raw_new,raw_data,length*sizeof(T));
    // copy last row
    size_t idx = 0 + f_width * (height - 1);
    memcpy(&raw_new[length], &raw_data[idx], f_width*sizeof(T));
    
    // Assume, that current data is allocated on the heap.
    if(!outsideAlloc)
        delete [] raw_data;

    outsideAlloc = false;
    raw_data = raw_new;
    height += 1;
    length += f_width;
}
// TODO
template<typename T>
inline void Matrix<T>::duplicateLastColumn()
{
    int x = width - 1;
    size_t new_length = length + width * 3;
    try
    {
        T* raw_new = new T[new_length];
    }
    catch(const std::exception& e)
    {
        //std::cerr << e.what() << '\n';
    }
}

template<typename T>
inline void Matrix<T>::printMatrix()
{
    for(int i{0}; i<height; ++i)
    {
        for(int j{0}; j<f_width; ++j)
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
Matrix<T>::~Matrix()
{
    if(!outsideAlloc)
        delete [] raw_data;
}