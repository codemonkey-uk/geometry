#ifndef MATRIXN_H_INCLUDED
#define MATRIXN_H_INCLUDED

// notes:  use of double for certain maths functionality
// - may want to go via arbitary precision maths codepath

#include "geometry_uninitialised.h"
#include "base_maths.h"
#include "vectorn.h"
#include "vector2d.h"

#include <assert.h>
#include <math.h>

namespace Geometry
{
    //
    // Interface
    //
    
    template< typename Scalar, size_t N, size_t M >
	class MatrixNM
    {
    public:
        const static size_t sColumns = N;
        const static size_t sRows = M;
        
        typedef Scalar ScalarType;
        typedef MatrixNM<Scalar,N, M> VectorType;
        typedef MatrixNM<Scalar,N, M> BaseType;
        
        //get the number of elements / dimentions for this type
        static
        Vector2d<Scalar> GetSize();
        
        // explictly uninitialised construction
        explicit MatrixNM(const Uninitialised&)
        { }
        
        //type conversion constructor
        template< typename OtherScalar >
		MatrixNM<Scalar,N, M>( const MatrixNM<OtherScalar, N, M>& rhs ) 
		{
				for (size_t i=0;i<N*M;++i)
				{
					mData[i] = Scalar(rhs[i]);
				}
		}
        
        // simple accessors
        void Set (size_t n, size_t m, Scalar value);
        // Scalar& operator[] (size_t offset);
        Scalar Get (size_t n, size_t m) const;

        // const Scalar& operator[] (size_t offset)  const;
        
        // distance and length
        // todo !!! not using get/compute naming convention
        //Scalar LengthSquare() const;
        //Scalar Length() const;
        //Scalar DistanceSquare(const VectorN& rhs) const;
        //Scalar Distance(const VectorN& rhs) const;
        
        // binary operators
        MatrixNM& operator = (const MatrixNM& rhs);
        MatrixNM& operator += (const MatrixNM& rhs);
        MatrixNM& operator -= (const MatrixNM& rhs);
        MatrixNM& operator /= (const MatrixNM rhs);
        MatrixNM& operator *= (const MatrixNM rhs);
        bool operator == (const MatrixNM& rhs) const;
        bool operator != (const MatrixNM& rhs) const;
        
        //void Normalise();
        
        //static 
        //Scalar DotProduct( const VectorN& lhs, const VectorN& rhs );
        
    // private:
            //Scalar mData[N*M];
            VectorN<Scalar,N*M> mData;
    };

    //
    // Free-functions
    //

    template<typename Scalar, size_t N, size_t M>
    MatrixNM<Scalar, N, M> operator+ (MatrixNM<Scalar, N, M> lhs, const MatrixNM<Scalar, N, M>& rhs)
    {
        lhs += rhs;
        return lhs;
    }
    
    template<typename Scalar, size_t N, size_t M>
    MatrixNM<Scalar, N, M> operator- (MatrixNM<Scalar, N, M> lhs, const MatrixNM<Scalar, N, M>& rhs)
    {
        lhs -= rhs;
        return lhs;
    }
    
    template<typename Scalar, size_t N, size_t M>
    MatrixNM<Scalar, N, M> operator/ (MatrixNM<Scalar, N, M> lhs, const Scalar rhs)
    {
        lhs /= rhs;
        return lhs;
    }

    template<typename Scalar, size_t N, size_t M>
    MatrixNM<Scalar, N, M> operator* (MatrixNM<Scalar, N, M> lhs, const Scalar rhs)
    {
        lhs *= rhs;
        return lhs;
    }
    
    template<typename Scalar, size_t N, size_t M>
    MatrixNM<Scalar, N, M> operator- (MatrixNM<Scalar, N, M> arg)
    {
    	arg.mData = -arg.mData;
		return arg;
    }

    //
    // Class Implementation
    // (in header as is a template)
    //
	/*    
    template< typename Scalar, size_t N, size_t M >
    template< typename OtherScalar >
    MatrixNM<Scalar, N, M>::MatrixNM<Scalar, N, M>( const VectorN<OtherScalar, N>& rhs ) 
    {
            for (size_t i=0;i<N;++i)
            {
                mData[i] = Scalar(rhs[i]);
            }
    }
	*/
    
    template< typename Scalar, size_t N, size_t M >
    Vector2d<Scalar> MatrixNM<Scalar, N, M>::GetSize() 
    {
        return Vector2d<Scalar>(N,M);
    }
    
    template< typename Scalar, size_t N, size_t M >
    void MatrixNM<Scalar, N, M>::Set(size_t n, size_t m, Scalar value)
    {
        assert( n<N );
        assert( m<M );
        mData[n*M+m]=value;
    }
    
    //template< typename Scalar, size_t N, size_t M >
    //Scalar& MatrixNM<Scalar, N, M>::operator[] (size_t offset) 
    //{
    //    assert( offset<N );
    //    return mData[offset];
    //}

    template< typename Scalar, size_t N, size_t M >
    Scalar MatrixNM<Scalar, N, M>::Get(size_t n, size_t m) const 
    {
        assert( n<N );
        assert( m<M );
        return mData[n*M+m];
    }
    
    //template< typename Scalar, size_t N, size_t M >
    //const Scalar& MatrixNM<Scalar, N, M>::operator[] (size_t offset)  const 
    //{
    //    assert( n<N );
    //    assert( m<M );
	//	return  mData[n*M+m];
    //}
    
    template< typename Scalar, size_t N, size_t M >
    MatrixNM<Scalar, N, M>& MatrixNM<Scalar, N, M>::operator = (const MatrixNM& rhs) 
    {
        mData = rhs.mData;
    }
    
    template< typename Scalar, size_t N, size_t M >
    MatrixNM<Scalar, N, M>& MatrixNM<Scalar, N, M>::operator += (const MatrixNM& rhs) 
    {
        mData += rhs.mData;
        return *this;
    }
    
    template< typename Scalar, size_t N, size_t M >
    MatrixNM<Scalar, N, M>& MatrixNM<Scalar, N, M>::operator -= (const MatrixNM& rhs) 
    {
        mData -= rhs.mData;
        return *this;
    }
    
    //template< typename Scalar, size_t N, size_t M >
    //MatrixNM<Scalar, N, M>& MatrixNM<Scalar, N, M>::operator /= (const Scalar rhs) 
    //{
    //    for (size_t i=0;i!=N;++i)
    //        mData[i] /= rhs;
    //    return *this;
    //}
    
    //template< typename Scalar, size_t N, size_t M >
    //MatrixNM<Scalar, N, M>& MatrixNM<Scalar, N, M>::operator *= (const Scalar rhs)
    //{
    //    for (size_t i=0;i!=N;++i)
    //        mData[i] *= rhs;
    //    return *this;
    //}
    
    template< typename Scalar, size_t N, size_t M >
    bool MatrixNM<Scalar, N, M>::operator == (const MatrixNM& rhs) const
    {
        return mData == rhs.mData;
    }
    
    template< typename Scalar, size_t N, size_t M >
    bool MatrixNM<Scalar, N, M>::operator != (const MatrixNM& rhs) const
    {
        return !(*this==rhs);
    }


}//namespace Geometry

#endif