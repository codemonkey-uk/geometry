#ifndef MATRIXN_H_INCLUDED
#define MATRIXN_H_INCLUDED

#include "matrixnm.h"

#include <assert.h>
#include <math.h>

namespace Geometry
{
    //
    // Interface
    //
    
    template< typename Scalar, size_t N>
	class MatrixN : public MatrixNM<Scalar, N, N>
    {
    public:
        const static size_t sColumns = N;
        const static size_t sRows = N;
        
        typedef Scalar ScalarType;
        typedef MatrixN<Scalar,N> MatrixType;
        typedef MatrixNM<Scalar,N, N> BaseType;
    	
    	MatrixN()
    		: BaseType(uninitialised)
        { 
			BecomeIdentity();
        } 
        
        // explictly uninitialised construction
        explicit MatrixN(const Uninitialised&)
        	: BaseType(uninitialised)
        { }
        
        MatrixN(const BaseType& rhs)
        	: BaseType(rhs)
        { }
        
        explicit MatrixN(const Scalar data[N*N])
        	: BaseType(data)
		{ }
		
		MatrixN(std::initializer_list<Scalar> data) 
			: BaseType(data)
		{ }
        
        //type conversion constructor
        //template< typename OtherScalar >
		//MatrixN<Scalar,N>( const MatrixN<OtherScalar, N>& rhs )
		//	: BaseType(rhs)
		//	{ }
        
        void BecomeIdentity()
        {
        	int i=0;
        	this->mData[i++]=1;
        	while(i<N*N)
        	{
        		for(int j=0;j!=N;++j)
        			this->mData[i++]=0;
        		this->mData[i++]=1;
        	}
        }
        
        void Scale(VectorN<Scalar, N> s)
        {
        	int i=0, a=0;
        	this->mData[i++]=s[a++];
        	while(i<N*N)
        	{
        		for(int j=0;j!=N;++j)
        			this->mData[i++]=0;
        		this->mData[i++]=s[a++];
        	}
        } 
        
        void Scale(VectorN<Scalar, N-1> s)
        {
			Scale(VectorN<Scalar, N>(s, 1));
        } 
        
        void Scale(Scalar s)
        {
			Scale(VectorN<Scalar, N-1>(s));
        }
        
        void Transpose()
        {
        	for (int n=0;n!=N;++n)
        	{
				for (int m=n+1;m!=N;++m)
				{
					std::swap( (*this)[n][m], (*this)[m][n] );
				}
        	}
        }
        /*
        void Pow(int i)
        {
			MatrixN a;//identity
			while(i>0){
				if(i%2) a = a * *this;
				i=i/2;
				m=m*m;
			}        
        }
        */
    };

    //
    // Free-functions
    //

	template<typename Scalar, size_t N>
    VectorN<Scalar, N> operator* (const MatrixN<Scalar, N>& lhs, const VectorN<Scalar, N>& rhs)
    {
		VectorN<Scalar, N> r(uninitialised);
        for (int n=0;n!=N;++n)
        {
        	r[n]=lhs[0][n]*rhs[0];
        	for (int m=1;m!=N;++m)
        		r[n] += lhs[m][n]*rhs[m];
        }
        return r;
    }

}//namespace Geometry

#endif