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

bool verifyNewColumn(Matrix<char>& mat, int x)
{
    for(int y{0}; y < mat.getHeight(); ++y)
    {
        for(int i{0}; i < mat.getChannels(); ++i)
        {
            if(mat(x + i,y) != mat(x - 3 + i, y))
            {
                return false;
            }
        }
    }
    return true;
}

bool verifyDoubleColumn(Matrix<char>& mat, int x)
{
    auto ch = mat.getChannels();
    auto h = mat.getHeight();
    for(int y{0}; y < h; ++y)
    {
        for(int i{0}; i < ch * 2; ++i)
        {
            if(mat(x + i,y) != mat(x - ch + i, y))
            {
                return false;
            }
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
    M_Assert(mat.getHeight() == 3, "duplicateLastRow (1: single): Incorrect height");
    M_Assert(mat.size() == 18, "duplicateLastRow (1: single): Incorrect size");
    M_Assert(verifyNewRow(mat, 2), "duplicateLastRow (1: single): Last row is incorrect");
    mat.duplicateLastRow();
    M_Assert(mat.getHeight() == 4, "duplicateLastRow (2: single): Incorrect height");
    M_Assert(mat.size() == 24, "duplicateLastRow (2: single): Incorrect size");
    M_Assert(verifyNewRow(mat, 3), "duplicateLastRow (2: single): Last row is incorrect");
    mat.duplicateLastRow(3);
    M_Assert(mat.getHeight() == 7, "duplicateLastRow (3: triple): Incorrect height");
    M_Assert(mat.size() == 42, "duplicateLastRow (3: triple): Incorrect size");
    M_Assert(verifyTripleNewRow(mat, 6), "duplicateLastRow (3: triple): Some row is incorrect");
    // duplicateLastColumn
    Matrix mat2 = Matrix<char>(arr, 2, 2, 3);
    mat2.duplicateLastColumn();
    M_Assert(mat2.getWidth() == 3, "duplicateLastColumn (1: single): Incorrect width");
    M_Assert(mat2.getF_width() == 9, "duplicateLastColumn (1: single): Incorrect full width");
    M_Assert(mat2.size() == 18, "duplicateLastColumn (1: single): Incorrect size");
    M_Assert(verifyNewColumn(mat2, 6), "duplicateLastColumn (1: single): Last column is incorrect");
    mat2.duplicateLastColumn();
    M_Assert(mat2.getWidth() == 4, "duplicateLastColumn (2: single): Incorrect width");
    M_Assert(mat2.getF_width() == 12, "duplicateLastColumn (2: single): Incorrect full width");
    M_Assert(mat2.size() == 24, "duplicateLastColumn (2: single): Incorrect size");
    M_Assert(verifyNewColumn(mat2, 9), "duplicateLastColumn (2: single): Last column is incorrect");
    mat2.duplicateLastColumn(2);
    M_Assert(mat2.getWidth() == 6, "duplicateLastColumn (3: double): Incorrect width");
    M_Assert(mat2.getF_width() == 18, "duplicateLastColumn (3: double): Incorrect full width");
    M_Assert(mat2.size() == 36, "duplicateLastColumn (3: double): Incorrect size");
    M_Assert(verifyDoubleColumn(mat2, 12), "duplicateLastColumn (3: double): Some column is incorrect");
    // Combination of two
    Matrix mat3 = Matrix<char>(arr, 2, 2, 3);
    mat3.duplicateLastRow(3);
    mat3.duplicateLastColumn(2);
    M_Assert(mat3.getHeight() == 5, "Combination (2 x col, 3 x row): Incorrect height");
    M_Assert(mat3.getWidth() == 4, "Combination (2 x col, 3 x row): Incorrect width");
    M_Assert(mat3.getF_width() == 12, "Combination (2 x col, 3 x row): Incorrect full width");
    M_Assert(mat3.size() == 60, "Combination (2 x col, 3 x row): Incorrect size");
    M_Assert(verifyTripleNewRow(mat3, 4), "Combination (2 x col, 3 x row): Some row is incorrect");
    M_Assert(verifyDoubleColumn(mat3, 6), "Combination (2 x col, 3 x row): Some column is incorrect");
    return 0;
}