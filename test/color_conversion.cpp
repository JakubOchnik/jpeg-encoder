#include <utils.hpp>
#include <cassert>
#include <cmath>

bool validate_fwd(unsigned char* out)
{
    unsigned char should_be[] = {105,213,54,255,128,128};
    for(int i{0}; i < 6; ++i)
    {
        if(out[i] != should_be[i])
        {
            return false;
        }
    }
    return true;
}

bool validate_rwd(unsigned char* out)
{
    unsigned char orig[] = {1,128,255,255,255,255};
    for(int i{0}; i < 6; ++i)
    {
        if(abs(orig[i] - out[i]) > 2)
        {
            return false;
        }
    }
    return true;
}

int main()
{
    unsigned char arr[] = {1,128,255,255,255,255};
    color::rgb2ycbcr<unsigned char>(arr,6);
    assert(validate_fwd(arr));
    color::ycbcr2rgb<unsigned char>(arr,6);
    assert(validate_rwd(arr));
    return 0;
}