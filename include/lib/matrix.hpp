#include <cstdint>
#include <exception>
#include <vector>

template<typename T>
class Matrix
{
    T* raw_data;

    uint16_t width;
    uint16_t height;
    uint16_t f_width;
    uint16_t f_height;
    uint8_t channels;

    long long length;



public:

    Matrix(T* data, uint16_t w, uint16_t h, uint8_t ch);
    Matrix() = delete;
    T& operator[](size_t index);
    T& operator()(size_t index);
    T& operator()(size_t x, size_t y);
    T& operator()(size_t x, size_t y, uint8_t ch);
    const std::vector<T&> getPixel(size_t x, size_t y);
    T& getPixelCh(size_t x, size_t y, uint8_t ch);

    size_t size() const;

    // Modifying size
    // apppendRow
    void appendRow();
    // appendColumn
    void appendColumn();
};

template<typename T>
inline Matrix<T>::Matrix(T* data, uint16_t w, uint16_t h, uint8_t ch): raw_data(data), width(w), height(h), channels(ch)
{
    f_width = width * channels;
    f_height = height;
    length = width * height * channels;
}

template<typename T>
inline T& Matrix<T>::operator()(size_t index)
{
    if(index > length - 1)
    {
        throw std::exception("Array index too large");
    }
    return raw_data[index];
}

template<typename T>
inline T& Matrix<T>::operator()(size_t x, size_t y)
{
    if(x > f_width - 1 || y > height - 1)
    {
        throw std::exception("Index out of bounds");
    }

    size_t idx = x + y * width;
    return raw_data[idx];
}

template<typename T>
inline T& Matrix<T>::operator()(size_t x, size_t y, uint8_t ch)
{
    return getPixelCh(x,y,ch);
}

template<typename T>
inline T& Matrix<T>::operator[](size_t index)
{
    if(index > length - 1)
    {
        throw std::exception("Array index too large");
    }
    return raw_data[index];
}

template<typename T>
inline const std::vector<T&> Matrix<T>::getPixel(size_t x, size_t y)
{
    if(x > width - 1 || y > height - 1)
    {
        throw std::exception("Index out of bounds");
    }
    // Returns a vector of REFERENCES to each particular channel
    // R, G, B; Y, Cb, Cr; etc.
    size_t base_idx = (x + y * width) * channels;
    std::vector<T&> refs(channels);
    for(int i{0}; i<channels; ++i)
    {
        refs.at(i) = raw_data[base_idx + i];
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
    if(x > width - 1 || y > height - 1)
    {
        throw std::exception("Index out of bounds");
    }
    if(ch > channels)
    {
        throw std::exception("Channel out of range");
    }
    // Returns a vector of REFERENCES to each particular channel
    // R, G, B; Y, Cb, Cr; etc.
    size_t base_idx = (x + y * width) * channels;
    return raw_data[base_idx + ch];
}