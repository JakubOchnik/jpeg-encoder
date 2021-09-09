#include <cmath>
#include <imgMatrix.hpp>
#include <subsampMatrix.hpp>
#include <consts/consts.hpp>

class Subsampler 
{
    static SubMatrix<unsigned char> chromaSubsample(ImgMatrix<unsigned char>& img);
};