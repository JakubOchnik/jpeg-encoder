#include <lib/dct.hpp>

using cosineLookup = std::array<std::array<float,8>,8>;
using DCTarray = std::array<std::array<int16_t,8>,8>;
using blockArray = std::array<std::array<uint8_t,8>,8>;

cosineLookup DCTprocess::getCosLookup(size_t N)
{
    cosineLookup cosines;
    for(int x{0}; x < N; ++x)
    {
        for(int i{0}; i < N; ++i)
        {
            cosines[x][i] = cosf((2 * x + 1) * i * PI / (2 * N));
        }
    }
    return cosines;
}

void DCTprocess::getBlock411(std::array<std::array<uint8_t,8>,8>& matrix, const size_t startX, const size_t startY, const int8_t channel)
{
    for(size_t y{startY}; y < startY + 8; ++y)
    {
        for(size_t x{startX}; x < startX + 8; ++x)
        {
            matrix[y-startY][x-startX] = srcMat(x, y, channel);
        }
    }
}

void DCTprocess::calculateDCT(blockArray& srcBlock, DCTarray& dstDCT, const cosineLookup& cosLookup, const uint8_t N)
{
    // ! VERY INEFFICIENT !
    // TODO: USE EXTERNAL FFT LIBRARY (fftw)
    for(uint8_t i{0}; i < N; ++i)
    {
        for(uint8_t j{0}; j < N; ++j)
        {
            float temp{};
            for(uint8_t y{0}; y < N; ++y)
            {
                for(uint8_t x{0}; x < N; ++x)
                {
                    temp += cosLookup[x][i] * cosLookup[y][j] * srcBlock[x][y];
                }
            }
            float coeff{(i > 0 ? 1.0f : 1.0f/sqrtf(2)) * (j > 0 ? 1.0f : 1.0f/sqrtf(2))};
            temp *= 1/sqrtf(2 * N) * coeff;
            dstDCT[i][j] = roundf(temp);
        }
    }
}

void DCTprocess::quantizeBlock(DCTarray& dct, const uint8_t N)
{
    for(int y{0}; y < N; ++y)
    {
        for(int x{0}; x < N; ++x)
        {
            dct[y][x] = roundf(dct[y][x] / quantizationMatrix[y][x]);
        }
    }
}

void DCTprocess::calculateAndQuantize(blockArray& srcBlock, DCTarray& dstDCT, const cosineLookup& cosLookup, const uint8_t N)
{
    for(uint8_t i{0}; i < N; ++i)
    {
        for(uint8_t j{0}; j < N; ++j)
        {
            float temp{};
            for(uint8_t y{0}; y < N; ++y)
            {
                for(uint8_t x{0}; x < N; ++x)
                {
                    temp += cosLookup[x][i] * cosLookup[y][j] * srcBlock[x][y];
                }
            }
            float coeff{(i > 0 ? 1.0f : 1.0f/sqrtf(2)) * (j > 0 ? 1.0f : 1.0f/sqrtf(2))};
            temp *= 1/sqrtf(2 * N) * coeff;
            dstDCT[i][j] = roundf(temp);
            // Quantization
            dstDCT[i][j] = roundf(dstDCT[i][j] / quantizationMatrix[i][j]);
        }
    }
}

void DCTprocess::executeDCT()
{
    // There is a large chance that this will not be needed
    if(srcMat.getF_width() % 8 != 0)
    {
        // duplicate cols
    }

    if(srcMat.getHeight() % 8 != 0)
    {
        // duplicate rows
        srcMat.duplicateLastRow(8 - srcMat.getHeight() % 8);
    }

    const auto cosLookup = getCosLookup(8);
    // Pre-allocate space for 8x8 matrices
    blockArray Y_block;
    blockArray Cb_block;
    blockArray Cr_block;

    // Perform DCT for each block
    if(type == SubsamplingType::s411)
    {
        // Y 
        for(size_t y{0}; y < srcMat.getOrigHeight(); y+=8)
        {
            for(size_t x{0}; x < srcMat.getOrigWidth(); x+=8)
            {
                // Get the Y
                getBlock411(Y_block, x, y, 0);
                DCTarray output_DCT;
                calculateDCT(Y_block, output_DCT, cosLookup, 8);
            }
            // Y
        }
    }
}

DCTprocess::DCTprocess(SubMatrix<unsigned char>& src): srcMat(src), type(src.getType())
{

}