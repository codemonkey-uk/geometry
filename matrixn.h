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
                
        // explictly uninitialised construction
        explicit MatrixN(const Uninitialised&)
        	: BaseType(uninitialised)
        { }
        
        explicit MatrixN(const Scalar data[N*N])
        	: BaseType(data)
		{ }
		
		MatrixN(std::initializer_list<Scalar> data) 
			: BaseType(data)
		{ }
        
        //type conversion constructor
        template< typename OtherScalar >
		MatrixN<Scalar,N>( const MatrixN<OtherScalar, N>& rhs )
			: BaseType(rhs)
		{ }
                
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