#ifndef MATRIXN_H_INCLUDED
#define MATRIXN_H_INCLUDED

#include "matrixnm.h"

#include <cassert>
#include <cmath>

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
        //    : BaseType(rhs)
        //    { }

        static MatrixN Identity();
        void BecomeIdentity();

        void BecomeScale(const VectorN<Scalar, N>& s);
        void BecomeScale(VectorN<Scalar, N-1> s);
        void BecomeScale(Scalar s);

        void Transpose();

        MatrixN Pow(int i) const;

        static MatrixN Translation(const VectorN<Scalar, N-1>& t);
        void BecomeTranslation(const VectorN<Scalar, N-1>& t);
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

    //
    // Member Functions
    //

    //static
    template<typename Scalar, size_t N>
    MatrixN<Scalar, N> MatrixN<Scalar, N>::Identity()
    {
        MatrixN<Scalar, N> result(uninitialised);
        result.BecomeIdentity();
        return result;
    }

    template<typename Scalar, size_t N>
    void MatrixN<Scalar, N>::BecomeIdentity()
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

    template<typename Scalar, size_t N>
    void MatrixN<Scalar, N>::BecomeScale(const VectorN<Scalar, N>& s)
    {
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
    }

    template<typename Scalar, size_t N>
    void MatrixN<Scalar, N>::BecomeScale(VectorN<Scalar, N-1> s)
    {
        BecomeScale(VectorN<Scalar, N>(s, 1));
    }

    template<typename Scalar, size_t N>
    void MatrixN<Scalar, N>::BecomeScale(Scalar s)
    {
        BecomeScale(VectorN<Scalar, N-1>(s));
    }

    template<typename Scalar, size_t N>
    void MatrixN<Scalar, N>::Transpose()
    {
        for (int n=0;n!=N;++n)
        {
            for (int m=n+1;m!=N;++m)
            {
                std::swap( (*this)[n][m], (*this)[m][n] );
            }
        }
    }

    template<typename Scalar, size_t N>
    MatrixN<Scalar, N> MatrixN<Scalar, N>::Pow(int i) const
    {
        MatrixN a; //identity
        MatrixN m = *this;
        while(i>0){
            if (i%2) a = a * m;
            i = i / 2;
            m = m * m;
        }
        return a;
    }

    // static
    template<typename Scalar, size_t N>
    MatrixN<Scalar, N> MatrixN<Scalar, N>::Translation(const VectorN<Scalar, N-1>& t)
    {
        MatrixN result(uninitialised);
        result.BecomeTranslation(t);
        return result;
    }

    template<typename Scalar, size_t N>
    void MatrixN<Scalar, N>::BecomeTranslation(const VectorN<Scalar, N-1>& t)
    {
        size_t i=0;
        this->mData[i++]=1;
        while(i<N*(N-1)-1)
        {
            for(size_t j=0;j!=N;++j)
                this->mData[i++]=0;
            this->mData[i++]=1;
        }

        this->mData[i++]=0;
        for(size_t j=0;j!=N-1;++j)
            this->mData[i++]=t.Get(j);
        this->mData[i++]=1;
    }

}//namespace Geometry

#endif