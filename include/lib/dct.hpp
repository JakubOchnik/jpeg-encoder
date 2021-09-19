#include <lib/subsampMatrix.hpp>
#include <consts/consts.hpp>
#include <array>
#include <cmath>
#include <utility>
#include <vector>
#include <cmath>

using cosineLookup = std::array<std::array<float,8>,8>;
using DCTarray = std::array<std::array<int16_t,8>,8>;
using blockArray = std::array<std::array<uint8_t,8>,8>;

class DCTprocess
{
    const std::array<std::array<uint16_t, 8>, 8> quantizationMatrix = 
    {{
        {16,11,10,16,24,40,51,61},
        {12,12,14,19,26,58,60,55},
        {14,13,16,24,40,57,69,56},
        {14,17,22,29,51,87,80,62},
        {18,22,37,56,68,109,103,77},
        {24,35,55,63,81,104,113,92},
        {49,63,78,87,103,121,120,101},
        {72,92,95,98,112,100,103,99}
    }};
    const double PI = 3.14159265358979323846;

    SubMatrix<unsigned char>& srcMat;
    SubsamplingType type;
public:
    DCTprocess(SubMatrix<unsigned char>& src);
    // Get Cosines lookup table
    cosineLookup getCosLookup(size_t N);
    // Get all values of the Y 8x8 block channel: {0 -> Y, 1 -> Cb, 2 -> Cr}
    void getBlock411(std::array<std::array<uint8_t,8>,8>& matrix, const size_t x, const size_t y, const int8_t channel);
    // Get all values of the Cb/Cr 8x8 block. channel: {0 -> Cb, 1 -> Cr}
    //void getColorBlock411(std::array<std::array<uint8_t,8>,8>& matrix, const size_t startX, const size_t startY, const int8_t channel);
    /*
    Type-II DCT:
    - Process each channel (Y, Cb, Cr) separatedly
    - Divide matrix of each channel into 8x8 sub-matrices (repeat edges if the width/length is not dividable by 8)
    For every block:
    1) Normalize the values - subtract 128 (currently skipped)
    2) Take the 2D DCT which is given by formula: https://en.wikipedia.org/wiki/JPEG#Block_splitting (Discrete cosine transform)
    [Test on the values from https://cs.stanford.edu/people/eroberts/courses/soco/projects/data-compression/lossy/jpeg/dct.htm, except
    DC coeff and 19 in the upper right corner]
    3) Quantize the matrix
    */

    // Calculate DCT for the block
    void calculateDCT(blockArray& srcBlock, DCTarray& dstDCT, const cosineLookup& cosLookup, const uint8_t N);
    // Quantize the block
    void quantizeBlock(DCTarray& dct, const uint8_t N);
    // Calculate the quantized DCT for the block
    void calculateAndQuantize(blockArray& srcBlock, DCTarray& dstDCT, const cosineLookup& cosLookup, const uint8_t N);

    // Perform DCT on the whole image
    void executeDCT();    
};