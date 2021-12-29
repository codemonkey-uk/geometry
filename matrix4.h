#ifndef MATRIX4_H_INCLUDED
#define MATRIX4_H_INCLUDED

#include "matrixn.h"
#include "vector3d.h"
#include "base_maths.h"

namespace Geometry
{
    //
    // Interface
    //

    template< typename Scalar>
    class Matrix4 : public MatrixN<Scalar, 4>
    {
    public:
        const static size_t sColumns = 4;
        const static size_t sRows = 4;

        typedef Scalar ScalarType;
        typedef Matrix4<Scalar> MatrixType;
        typedef MatrixN<Scalar,4> BaseType;

        Matrix4()
            : BaseType(uninitialised)
        {
            this->BecomeIdentity();
        }

        // explictly uninitialised construction
        explicit Matrix4(const Uninitialised&)
            : BaseType(uninitialised)
        { }

        Matrix4(const typename BaseType::BaseType& rhs)
            : BaseType(rhs)
        { }

        explicit Matrix4(const Scalar data[sRows*sColumns])
            : BaseType(data)
        { }

        Matrix4(std::initializer_list<Scalar> data)
            : BaseType(data)
        { }
        
        Matrix4( 
            const VectorN<Scalar,3>& x, 
            const VectorN<Scalar,3>& y, 
            const VectorN<Scalar,3>& z);

        static Matrix4 RotationAroundX(Scalar r);
        void BecomeRotationAroundX(Scalar r);

        static Matrix4 RotationAroundY(Scalar r);
        void BecomeRotationAroundY(Scalar r);

        static Matrix4 RotationAroundZ(Scalar r);
        void BecomeRotationAroundZ(Scalar r);

        static Matrix4 RotationFromEuler(const VectorN<Scalar,3>& r);
        void BecomeRotationFromEuler(const VectorN<Scalar,3>& r);

        static Matrix4 RotationAround(const VectorN<Scalar,3>& axis, Scalar r);
        void BecomeRotationAround(const VectorN<Scalar,3>& axis, Scalar r);
    
        static Matrix4 RotationAlign(const Vector3d<Scalar>& v1, const Vector3d<Scalar>& v2);
        void BecomeRotationAlign( const Vector3d<Scalar>& v1, const Vector3d<Scalar>& v2);
    };
    
    //
    // Free-functions
    //

    // ---

    //
    // Member Functions
    //

    // basis vectors
    template< typename Scalar>
    Matrix4<Scalar>::Matrix4( 
        const VectorN<Scalar,3>& x, 
        const VectorN<Scalar,3>& y, 
        const VectorN<Scalar,3>& z)
    {
        this->mData[ 0] = x[0];
        this->mData[ 1] = x[1];
        this->mData[ 2] = x[2];
        this->mData[ 3] = 0;

        this->mData[ 4] = y[0];
        this->mData[ 5] = y[1];
        this->mData[ 6] = y[2];
        this->mData[ 7] = 0;

        this->mData[ 8] = z[0];
        this->mData[ 9] = z[1];
        this->mData[10] = z[2];
        this->mData[11] = 0;

        this->mData[12] = 0;
        this->mData[13] = 0;
        this->mData[14] = 0;
        this->mData[15] = 1;
    }
        
    // static
    template< typename Scalar>
    Matrix4<Scalar> Matrix4<Scalar>::RotationAroundX(Scalar r)
    {
        Matrix4<Scalar> result(uninitialised);
        result.BecomeRotationAroundX(r);
        return result;
    }

    template< typename Scalar>
    void Matrix4<Scalar>::BecomeRotationAroundX(Scalar r)
    {
        Scalar cr = Cos(r);
        Scalar sr = Sin(r);

        // |  1  0       0       0 |
        // |  0  cos(A) -sin(A)  0 |
        // |  0  sin(A)  cos(A)  0 |
        // |  0  0       0       1 |

        this->mData[ 0] = 1;
        this->mData[ 1] = 0;
        this->mData[ 2] = 0;
        this->mData[ 3] = 0;

        this->mData[ 4] = 0;
        this->mData[ 5] = cr;
        this->mData[ 6] = -sr;
        this->mData[ 7] = 0;

        this->mData[ 8] = 0;
        this->mData[ 9] = sr;
        this->mData[10] = cr;
        this->mData[11] = 0;

        this->mData[12] = 0;
        this->mData[13] = 0;
        this->mData[14] = 0;
        this->mData[15] = 1;
    }

    // static
    template< typename Scalar>
    Matrix4<Scalar> Matrix4<Scalar>::RotationAroundY(Scalar r)
    {
        Matrix4<Scalar> result(uninitialised);
        result.BecomeRotationAroundY(r);
        return result;
    }

    template< typename Scalar>
    void Matrix4<Scalar>::BecomeRotationAroundY(Scalar r)
    {
        Scalar cr = Cos(r);
        Scalar sr = Sin(r);

        // |  cos(A)  0   sin(A)  0 |
        // |  0       1   0       0 |
        // | -sin(A)  0   cos(A)  0 |
        // |  0       0   0       1 |

        this->mData[ 0] = cr;
        this->mData[ 1] = 0;
        this->mData[ 2] = sr;
        this->mData[ 3] = 0;

        this->mData[ 4] = 0;
        this->mData[ 5] = 1;
        this->mData[ 6] = 0;
        this->mData[ 7] = 0;

        this->mData[ 8] = -sr;
        this->mData[ 9] = 0;
        this->mData[10] = cr;
        this->mData[11] = 0;

        this->mData[12] = 0;
        this->mData[13] = 0;
        this->mData[14] = 0;
        this->mData[15] = 1;
    }

    // static
    template< typename Scalar>
    Matrix4<Scalar> Matrix4<Scalar>::RotationAroundZ(Scalar r)
    {
        Matrix4<Scalar> result(uninitialised);
        result.BecomeRotationAroundZ(r);
        return result;
    }

    template< typename Scalar>
    void Matrix4<Scalar>::BecomeRotationAroundZ(Scalar r)
    {
        Scalar cr = Cos(r);
        Scalar sr = Sin(r);

        // cos(spin), -sin(spin), 0, 0,
        this->mData[ 0] = cr;
        this->mData[ 1] = -sr;
        this->mData[ 2] = 0;
        this->mData[ 3] = 0;

        // sin(spin), cos(spin), 0, 0,
        this->mData[ 4] = sr;
        this->mData[ 5] = cr;
        this->mData[ 6] = 0;
        this->mData[ 7] = 0;

        // 0, 0, 1, 0,
        this->mData[ 8] = 0;
        this->mData[ 9] = 0;
        this->mData[10] = 1;
        this->mData[11] = 0;

        // 0, 0, 0, 1
        this->mData[12] = 0;
        this->mData[13] = 0;
        this->mData[14] = 0;
        this->mData[15] = 1;
    }

    // static
    template< typename Scalar>
    Matrix4<Scalar> Matrix4<Scalar>::RotationFromEuler(const VectorN<Scalar,3>& r)
    {
        Matrix4<Scalar> result(uninitialised);
        result.RotationFromEuler(r);
        return result;
    }

    template< typename Scalar>
    void Matrix4<Scalar>::BecomeRotationFromEuler(const VectorN<Scalar,3>& r)
    {
        Scalar A = Cos(r.Get(0));
        Scalar B = Sin(r.Get(0));
        Scalar C = Cos(r.Get(1));
        Scalar D = Sin(r.Get(1));
        Scalar E = Cos(r.Get(2));
        Scalar F = Sin(r.Get(2));

        Scalar AD      =   A * D;
        Scalar BD      =   B * D;

        this->mData[0]  =   C * E;
        this->mData[1]  =  -C * F;
        this->mData[2]  =   D;
        this->mData[4]  =  BD * E + A * F;
        this->mData[5]  = -BD * F + A * E;
        this->mData[6]  =  -B * C;
        this->mData[8]  = -AD * E + B * F;
        this->mData[9]  =  AD * F + B * E;
        this->mData[10] =   A * C;

        this->mData[3]  =  this->mData[7] = this->mData[11] = this->mData[12] = this->mData[13] = this->mData[14] = 0;
        this->mData[15] =  1;
    }

    // static
    template< typename Scalar>
    Matrix4<Scalar> Matrix4<Scalar>::RotationAround(const VectorN<Scalar,3>& axis, Scalar r)
    {
        Matrix4<Scalar> result(uninitialised);
        result.BecomeRotationAround(axis, r);
        return result;
    }

    template< typename Scalar>
    void Matrix4<Scalar>::BecomeRotationAround(const VectorN<Scalar,3>& axis, Scalar r)
    {
        const Scalar rcos = Cos(r);
        const Scalar rsin = Sin(r);
        const Scalar rcosr = 1-rcos;
        (*this)[0][0] =            rcos + axis[0]*axis[0]*rcosr;
        (*this)[1][0] =  axis[2] * rsin + axis[1]*axis[0]*rcosr;
        //(*this)[2][0] = -axis[1] * rsin + axis[2]*axis[0]*rcosr;
        (*this)[2][0] = 0;
        (*this)[0][1] = -axis[2] * rsin + axis[0]*axis[1]*rcosr;
        (*this)[1][1] =            rcos + axis[1]*axis[1]*rcosr;
        //(*this)[2][1] =  axis[0] * rsin + axis[2]*axis[1]*rcosr;
        (*this)[2][1] = 0;
        (*this)[0][2] =  axis[1] * rsin + axis[0]*axis[2]*rcosr;
        (*this)[1][2] = -axis[0] * rsin + axis[1]*axis[2]*rcosr;
        (*this)[2][2] =            rcos + axis[2]*axis[2]*rcosr;
        (*this)[2][3] = 0;
        (*this)[0][3] = 0;
        (*this)[1][3] = 0;
        (*this)[2][3] = 0;
        (*this)[3][3] = 1;
    }
    
    // thanks to the incredible work here:
    // https://gist.github.com/kevinmoran/b45980723e53edeb8a5a43c49f134724
    
    // static
    template< typename Scalar>
    Matrix4<Scalar> Matrix4<Scalar>::RotationAlign(
        const Vector3d<Scalar>& v1, 
        const Vector3d<Scalar>& v2)
    {
        Matrix4<Scalar> result(uninitialised);
        result.BecomeRotationAlign(v1, v2);
        return result;
    }

    template< typename Scalar>
    void Matrix4<Scalar>::BecomeRotationAlign(
        const Vector3d<Scalar>& v1, 
        const Vector3d<Scalar>& v2)
    {
        // vec3 axis = cross( v1, v2 );
        auto axis = CrossProduct( v1, v2 );

        // const float cosA = dot( v1, v2 );
        auto cosA = Vector3d<Scalar>::DotProduct( v1, v2 );
        
        // const float k = 1.0f / (1.0f + cosA);
        const double k = 1.0 / (1.0 + cosA);

        const int x=0; const int y=1; const int z=2;    
        (*this)[0][0] = (axis[x] * axis[x] * k) + cosA;
        (*this)[0][1] = (axis[y] * axis[x] * k) - axis[z]; 
        (*this)[0][2] = (axis[z] * axis[x] * k) + axis[y];
        (*this)[0][3] = 0;
        (*this)[1][0] = (axis[x] * axis[y] * k) + axis[z];
        (*this)[1][1] = (axis[y] * axis[y] * k) + cosA;
        (*this)[1][2] = (axis[z] * axis[y] * k) - axis[x];
        (*this)[1][3] = 0;
        (*this)[2][0] = (axis[x] * axis[z] * k) - axis[y];
        (*this)[2][1] = (axis[y] * axis[z] * k) + axis[x];
        (*this)[2][2] = (axis[z] * axis[z] * k) + cosA;
        (*this)[2][3] = 0;
        (*this)[3][0] = 0;
        (*this)[3][1] = 0;
        (*this)[3][2] = 0;
        (*this)[3][3] = 1;
    }

}//namespace Geometry

#endif