#include <lib/dct.hpp>
#include <lib/subsampMatrix.hpp>
#include <smartAssert.hpp>

#include <vector>
#include <array>
#include <iostream>


class InternalVars
{
public:
    const std::vector<std::vector<unsigned char>> rawSubsampled = 
    {
        {140,144,147,140,123,177,140,155,179,175,121,83},
        {144,152,140,147,173,138,140,148,167,179,100,89},
        {152,155,136,167,88,54,163,162,152,172,136,134},
        {168,145,156,160,122,126,152,155,136,160,173,73},
        {162,148,156,148,93,122,140,136,147,162,95,83},
        {147,167,140,155,90,100,155,140,136,162,168,101},
        {136,156,123,167,63,80,162,144,140,147,197,154},
        {148,155,136,155,92,124,152,147,147,136,126,135}
    };

    const std::vector<std::vector<float>> cosLookup = 
    {
        {1,0.98078528,0.923879533,0.831469612,0.707106781,0.555570233,0.382683432,0.195090322},
        {1,0.831469612,0.382683432,-0.195090322,-0.707106781,-0.98078528,-0.923879533,-0.555570233},
        {1,0.555570233,-0.382683432,-0.98078528,-0.707106781,0.195090322,0.923879533,0.831469612},
        {1,0.195090322,-0.923879533,-0.555570233,0.707106781,0.831469612,-0.382683432,-0.98078528},
        {1,-0.195090322,-0.923879533,0.555570233,0.707106781,-0.831469612,-0.382683432,0.98078528},
        {1,-0.555570233,-0.382683432,0.98078528,-0.707106781,-0.195090322,0.923879533,-0.831469612},
        {1,-0.831469612,0.382683432,0.195090322,-0.707106781,0.98078528,-0.923879533,0.555570233},
        {1,-0.98078528,0.923879533,-0.831469612,0.707106781,-0.555570233,0.382683432,-0.195090322}

    };

    const std::vector<std::vector<unsigned int>> yBlock =
    {
        {140,144,147,140,140,155,179,175},
        {144,152,140,147,140,148,167,179},
        {152,155,136,167,163,162,152,172},
        {168,145,156,160,152,155,136,160},
        {162,148,156,148,140,136,147,162},
        {147,167,140,155,155,140,136,162},
        {136,156,123,167,162,144,140,147},
        {148,155,136,155,152,147,147,136}
    };
    
    const std::vector<std::vector<int16_t>> dct = 
    {
        {1210,-18,15,-9,23,-9,-14,-19},
        {21,-34,26,-9,-11,11,14,7},
        {-10,-24,-2,6,-18,3,-20,-1},
        {-8,-5,14,-15,-8,-3,-3,8},
        {-3,10,8,1,-11,18,18,15},
        {4,-2,-18,8,8,-4,1,-7},
        {9,1,-3,4,-1,-7,-1,-2},
        {0,-8,-2,2,1,4,-6,0}
    };

};

template<typename T, typename U>
bool matricesEqualFloat(T first, U second)
{
    if(first.size() != second.size() || first[0].size() != second[0].size())
    {
        return false;
    }

    for(int y{0}; y < first.size(); ++y)
    {
        for(int x{0}; x < first[0].size(); ++x)
        {
            if(first[y][x] != second[y][x] && !(abs(first[y][x] - second[y][x]) < 0.001f))
            {
                return false;
            }
        }
    }
    return true;
}

template<typename T, typename U>
bool matricesEqual(T first, U second)
{
    if(first.size() != second.size() || first[0].size() != second[0].size())
    {
        return false;
    }

    for(int y{0}; y < first.size(); ++y)
    {
        for(int x{0}; x < first[0].size(); ++x)
        {
            if(first[y][x] != second[y][x])
            {
                return false;
            }
        }
    }
    return true;
}

template<typename T>
void printMatrix(const T& mat)
{
    for(int y{0}; y < mat.size(); ++y)
    {
        for(int x{0}; x < mat[0].size(); ++x)
        {
            std::cout << (int)mat[y][x] << ",";
        }
        std::cout << "\n";
    }
}

int main()
{
    InternalVars correctVal;
    SubMatrix inputMat(correctVal.rawSubsampled, SubsamplingType::s411);
    DCTprocess dstDCT(inputMat);

    M_Assert(matricesEqualFloat(correctVal.cosLookup, dstDCT.getCosLookup(8)), "DCTprocess::getCosLookup(size_t N): Incorrect cos lookup table");

    std::array<std::array<uint8_t,8>,8> yBlock;
    dstDCT.getYBlock411(yBlock,0,0);
    M_Assert(matricesEqual(correctVal.yBlock, yBlock), "getYBlock411(std::array<std::array<uint8_t,8>,8>& matrix, const size_t x, const size_t y): Incorrect block");

    std::array<std::array<int16_t,8>,8> dctArray;
    dstDCT.calculateDCT(yBlock, dctArray, dstDCT.getCosLookup(8), 8);
    printMatrix(dctArray);
    M_Assert(matricesEqual(correctVal.dct, dctArray), "calculateDCT(blockArray& srcBlock, DCTarray& dstDCT, const cosineLookup& cosLookup, const uint8_t N);: Incorrect DCT output");
}
