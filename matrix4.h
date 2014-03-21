#ifndef MATRIX4_H_INCLUDED
#define MATRIX4_H_INCLUDED

#include "matrixn.h"
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
        
        Matrix4(const BaseType& rhs)
        	: BaseType(rhs)
        { }
        
        explicit Matrix4(const Scalar data[sRows*sColumns])
        	: BaseType(data)
		{ }
		
		Matrix4(std::initializer_list<Scalar> data) 
			: BaseType(data)
		{ }
        
        static Matrix4 RotationAroundX(Scalar r);
        void BecomeRotationAroundX(Scalar r);
        
        static Matrix4 RotationAroundY(Scalar r);
        void BecomeRotationAroundY(Scalar r);

        static Matrix4 RotationAroundZ(Scalar r);
        void BecomeRotationAroundZ(Scalar r);
    };

    //
    // Free-functions
    //

	// --- 
	
	//
	// Member Functions
	//
    
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
		
}//namespace Geometry

#endif