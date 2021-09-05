#include <lib/matrix.hpp>
#include <smartAssert.hpp>

bool verifyNewRow(Matrix<char>& mat, int y)
{
    for(int x{0}; x < 6; ++x)
    {
        if(mat(x,y) != mat(x,y-1))
        {
            return false;
        }
    }
    return true;
}

bool verifyTripleNewRow(Matrix<char>& mat, int y)
{
    // y = 6
    for(int x{0}; x < 6; ++x)
    {
        if(mat(x,y) != mat(x,y-1) || mat(x,y-1) != mat(x,y-2) || mat(x,y-2) != mat(x,y-3))
        {
            return false;
        }
    }
    return true;
}

int main()
{
    // duplicateLastRow
    char arr[] = {1,2,3,4,5,6,7,8,9,10,11,12};
    Matrix mat = Matrix<char>(arr, 2, 2, 3);
    mat.duplicateLastRow();
    M_Assert(verifyNewRow(mat, 2), "duplicateLastRow (1: single): Last row is incorrect");
    // another (mem alloc test, segfault = end of test)
    mat.duplicateLastRow();
    M_Assert(verifyNewRow(mat, 3), "duplicateLastRow (2: single): Last row is incorrect");
    mat.duplicateLastRow(3);
    M_Assert(verifyTripleNewRow(mat, 6), "duplicateLastRow (3: triple): Some row is incorrect");
    // duplicateLastColumn
    // TODO
    return 0;
}