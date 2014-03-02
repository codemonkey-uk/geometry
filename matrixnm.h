#ifndef MATRIXNM_H_INCLUDED
#define MATRIXNM_H_INCLUDED

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
        const static size_t sColumns = M;
        const static size_t sRows = N;
        
        typedef Scalar ScalarType;
        typedef MatrixNM<Scalar,N, M> MatrixType;
        typedef MatrixNM<Scalar,N, M> BaseType;
        
        //get the number of elements / dimentions for this type
        static
        Vector2d<int> GetSize();
        
        // explictly uninitialised construction
        explicit MatrixNM(const Uninitialised&)
        	: mData(uninitialised)
        { }
        
        explicit MatrixNM(const Scalar data[N*M])
        	: mData(data)
		{ }
		
		MatrixNM(std::initializer_list<Scalar> data) 
			: mData(data)
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
        Scalar* operator[] (size_t n);
        
        Scalar Get (size_t n, size_t m) const;
        const Scalar* operator[] (size_t n)  const;
        
        // binary operators
        MatrixNM& operator = (const MatrixNM& rhs);
        MatrixNM& operator += (const MatrixNM& rhs);
        MatrixNM& operator -= (const MatrixNM& rhs);
        //MatrixNM& operator /= (const Scalar rhs);
        //MatrixNM& operator *= (const Scalar rhs);
        bool operator == (const MatrixNM& rhs) const;
        bool operator != (const MatrixNM& rhs) const;

        
    // private:
            // Scalar mData[N*M];
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
    MatrixNM<Scalar, N, M> operator- (MatrixNM<Scalar, N, M> arg)
    {
    	arg.mData = -arg.mData;
		return arg;
    }

	// generic matrix multiply
	// MatrixNM[n][m] / [row][column]
	// N= Row Width, M= Column Height
	// L = lhs = left hand side, 
	// R = rhs = right hand side
	// lhs * rhs, 
	// LN == RM == LN_RM
	// Result of the multiplication has the same matrix dimensions as RHS
    template<typename Scalar, size_t LN, size_t LM_RN, size_t RM>
    MatrixNM<Scalar, LM_RN, RM> operator* (
    	const MatrixNM<Scalar, LN, LM_RN>& lhs, 
    	const MatrixNM<Scalar, LM_RN, RM>& rhs)
    {
    	MatrixNM<Scalar, LM_RN, RM> r(uninitialised);
    	for (int rn=0;rn!=LM_RN;++rn)
        {
        	for (int rm=0;rm!=RM;++rm)
        	{
        		// multiply rows into columns
        		r[rn][rm] = lhs[rn][0]*rhs[0][rm];
        		for(int nm=1;nm!=LM_RN;++nm)
        		{
        			r[rn][rm] += lhs[rn][nm]*rhs[nm][rm];
        		}
        	}
        }
        return r;
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
    Vector2d<int> MatrixNM<Scalar, N, M>::GetSize() 
    {
        return Vector2d<int>(N,M);
    }
    
    template< typename Scalar, size_t N, size_t M >
    void MatrixNM<Scalar, N, M>::Set(size_t n, size_t m, Scalar value)
    {
        assert( n<N );
        assert( m<M );
        mData[n*M+m]=value;
    }
    
    template< typename Scalar, size_t N, size_t M >
    Scalar* MatrixNM<Scalar, N, M>::operator[] (size_t n) 
    {
        assert( n<N );
        return &mData[n*M];
    }

    template< typename Scalar, size_t N, size_t M >
    Scalar MatrixNM<Scalar, N, M>::Get(size_t n, size_t m) const 
    {
        assert( n<N );
        assert( m<M );
        return mData[n*M+m];
    }
    
    template< typename Scalar, size_t N, size_t M >
    const Scalar* MatrixNM<Scalar, N, M>::operator[] (size_t n)  const 
    {
        assert( n<N );
		return &mData[n*M];
    }
    
    template< typename Scalar, size_t N, size_t M >
    MatrixNM<Scalar, N, M>& MatrixNM<Scalar, N, M>::operator = (const MatrixNM& rhs) 
    {
        mData = rhs.mData;
        return *this;
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