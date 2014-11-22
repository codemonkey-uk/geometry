#ifndef BASIC_MATHS_HEADER_INCLUDED
#define BASIC_MATHS_HEADER_INCLUDED

#include <limits>
#include <math.h>
#include <stdlib.h>

namespace Geometry
{
    inline float Sqrt( float f )
    {
        return sqrtf( f );
    }

    inline double Sqrt( double f )
    {
        return sqrt( f );
    }

    // http://www.codecodex.com/wiki/index.php?title=Calculate_an_integer_square_root
    inline int Sqrt( int x )
    {
        unsigned long op, res, one;
        op = x;
        res = 0;

        one = 1 << 30;
        while (one > op) one >>= 2;
        while (one != 0) {
            if (op >= res + one) {
                op = op - (res + one);
                res = res +  2 * one;
            }
            res >>= 1;
            one >>= 2;
        }
        return(res);
    }

    inline float Sin( float f )
    {
        return sinf(f);
    }

    inline float Cos( float f )
    {
        return cosf(f);
    }

    inline double Sin( double d )
    {
        return sin(d);
    }

    inline double Cos( double d )
    {
        return cos(d);
    }

    inline int Abs( int i )
    {
        return abs(i);
    }

    inline float Abs( float f )
    {
        return fabs(f);
    }

    inline double Abs( double d )
    {
        return fabs(d);
    }

    inline double Pow( double b, double e )
    {
        return pow(b,e);
    }

    inline float Pow( float b, float e )
    {
        return pow(b,e);
    }

    inline int iPow( int b, int e )
    {
        int result = 1;
        while (e)
        {
            if (e & 1)
                result *= b;
            e >>= 1;
            b *= b;
        }
        return result;
    }

    inline int Pow( int b, int e )
    {
        return iPow(b,e);
    }
}


#endif //BASIC_MATHS_HEADER_INCLUDED