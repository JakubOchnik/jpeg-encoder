#include <lib/matrix.hpp>
#include <smartAssert.hpp>

bool verifyNewRow(Matrix<char>& mat)
{
    for(int x{0}; x < 6; ++x)
    {
        if(mat(x,2) != mat(x,1))
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
    M_Assert(verifyNewRow(mat), "duplicateLastRow: Last row is incorrect");

    // duplicateLastColumn
    // TODO
    return 0;
}