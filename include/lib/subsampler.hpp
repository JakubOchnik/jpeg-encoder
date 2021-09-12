#include <cmath>
#include <imgMatrix.hpp>
#include <subsampMatrix.hpp>
#include <consts/consts.hpp>

class Subsampler 
{
public:
    // 4:1:1 implementation
    // TODO: 4:2:0 (most popular) and 4:2:2
    static SubMatrix<unsigned char> chromaSubsample411(ImgMatrix<unsigned char>& img);
};