#include "EngineMath.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"
#include "Quat.h"

using namespace Matrix::Math;

float MathInstance::GetFastSin(unsigned int i)
{
    return FastSin[i];
}
float MathInstance::GetFastCos(unsigned int i)
{
    return FastCos[i];
}

void MathInstance::InitCRCTable()
{
#define POLYNOMIAL 0x04c11db7L
    int i, j;
    unsigned int crc_accum;
    for (i = 0; i < 256; i++)
    {
        crc_accum = ((unsigned int)i << 24);
        for (j = 0; j < 8; j++)
        {
            if (crc_accum & 0x80000000L)
                crc_accum = (crc_accum << 1) ^ POLYNOMIAL;
            else
                crc_accum = (crc_accum << 1);
        }
        CRCTable[i] = crc_accum;
    }
}
unsigned int MathInstance::CRC32Compute(const void *pData, unsigned int uiDataSize)
{
    unsigned int uiResult;

    if (uiDataSize == 0)
        return 0;

    const unsigned char *pDataTemp = (const unsigned char *)pData;
    uiResult = *pDataTemp++ << 24;
    if (uiDataSize > 1)
    {
        uiResult |= *pDataTemp++ << 16;
        if (uiDataSize > 2)
        {
            uiResult |= *pDataTemp++ << 8;
            if (uiDataSize > 3)
                uiResult |= *pDataTemp++;
        }
    }
    uiResult = ~uiResult;

    for (unsigned int i = 4; i < uiDataSize; i++)
    {
        uiResult = (uiResult << 8 | *pDataTemp++) ^ CRCTable[uiResult >> 24];
    }

    return ~uiResult;
}
MathInstance::MathInstance()
{
    InitCRCTable();
    for (unsigned int i = 0; i <= 360; i++)
    {
        float iRadian = AngleToRadian(float(i));
        FastSin[i] = SIN(iRadian);
        FastCos[i] = COS(iRadian);
    }
}
MathInstance::~MathInstance()
{
}

/*----------------------------------------------------------------*/

bool Matrix::Math::InitMath()
{
    MathInstance::GetMathInstance();
    return 1;
}

bool Matrix::Math::IsUniformScale(float fScale)
{
    if (ABS(fScale - 1.0f) < EPSILON_E4)
    {
        return true;
    }
    return false;
}
bool Matrix::Math::IsZeroTranslate(const Vector3 &Translate)
{
    if (Translate.GetSqrLength() < EPSILON_E4)
    {
        return true;
    }
    return false;
}
bool Matrix::Math::IsIdentityRotate(const Matrix3 &Rotate)
{
    for (unsigned int i = 0; i < 3; i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            if (i != j)
            {
                if (ABS(Rotate.M[i][j]) > EPSILON_E4)
                {
                    return false;
                }
            }
            else
            {
                if (ABS(Rotate.M[i][j] - 1.0f) > EPSILON_E4)
                {
                    return false;
                }
            }
        }
    }
    return true;
}
bool Matrix::Math::IsIdentityRotate(const Quat &Rotate)
{
    Vector3 R(Rotate.x, Rotate.y, Rotate.z);
    if (R.GetSqrLength() < EPSILON_E4)
    {
        return true;
    }
    return false;
}
float Matrix::Math::LineInterpolation(float t1, float t2, float t)
{
    return t1 + (t2 - t1) * t;
}
Vector3 Matrix::Math::LineInterpolation(const Vector3 &t1, const Vector3 &t2, float t)
{
    return t1 + (t2 - t1) * t;
}
Quat Matrix::Math::LineInterpolation(const Quat &t1, const Quat &t2, float t)
{
    float fCos = t2.Dot(t1);
    Quat Temp = t1;
    if (fCos < 0.0f)
    {
        Temp = t1 * (-1.0f);
    }
    return Temp + (t2 - Temp) * t;
}
Vector4 Matrix::Math::LineInterpolation(const Vector4 &t1, const Vector4 &t2, float t)
{
    return t1 + (t2 - t1) * t;
}