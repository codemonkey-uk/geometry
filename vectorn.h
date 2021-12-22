#ifndef VECTORN_H_INCLUDED
#define VECTORN_H_INCLUDED

// notes:  use of double for certain maths functionality
// - may want to go via arbitary precision maths codepath

#include "geometry_uninitialised.h"
#include "base_maths.h"

#include <cassert>
#include <cmath>
#include <algorithm>
#include <initializer_list>

namespace Geometry
{
    //
    // Interface
    //
    
    template< typename Scalar, size_t N >
    class VectorN
    {
    public:
        const static size_t sDimensions = N;
        typedef Scalar ScalarType;
        typedef VectorN<Scalar,N> VectorType;
        typedef VectorN<Scalar,N> BaseType;
        
        //get the number of elements / dimentions for this type
        static
        size_t GetSize();
        
        // explictly uninitialised construction
        explicit VectorN(const Uninitialised&)
        { }

        explicit VectorN(Scalar d)
        {
            std::fill(mData, mData+N, d);
        }

        explicit VectorN(const Scalar data[N])
        {
            std::copy(data, data+N, mData);
        }

        VectorN(std::initializer_list<Scalar> data)
        {
            assert(data.size()<=N);
            std::copy(data.begin(), data.end(), mData);
            std::fill(mData+data.size(), mData+N, 0);
        }

        //type conversion constructor
        template< typename OtherScalar >
        VectorN<Scalar,N>( const VectorN<OtherScalar, N>& rhs ) 
        {
                for (size_t i=0;i<N;++i)
                {
                    mData[i] = Scalar(rhs[i]);
                }
        }

        //de-project (up-dimension) conversion constructor
        VectorN<Scalar,N>( const VectorN<Scalar, N-1>& rhs, Scalar extra ) 
        {
            for (int i=0;i!=N-1;++i)
                mData[i] = rhs[i];
            mData[N-1]=extra;
        }
        
        // simple accessors
        void Set (size_t offset, Scalar value);
        Scalar& operator[] (size_t offset);
        Scalar Get (size_t offset) const;

        const Scalar& operator[] (size_t offset)  const;
        
        // distance and length
        // todo !!! not using get/compute naming convention
        Scalar LengthSquare() const;
        Scalar Length() const;
        Scalar DistanceSquare(const VectorN& rhs) const;
        Scalar Distance(const VectorN& rhs) const;
        
        // binary operators
        VectorN& operator = (const VectorN& rhs);
        VectorN& operator += (const VectorN& rhs);
        VectorN& operator -= (const VectorN& rhs);
        VectorN& operator *= (const VectorN& rhs);        
        VectorN& operator /= (const Scalar rhs);
        VectorN& operator *= (const Scalar rhs);
        bool operator == (const VectorN& rhs) const;
        bool operator != (const VectorN& rhs) const;
        
        void Normalise();
        
        static 
        Scalar DotProduct( const VectorN& lhs, const VectorN& rhs );
        
        VectorN& reverse() { std::reverse(mData, mData+N); return *this; }

        VectorN Swizzle( const VectorN<int, N>& swiz ) const;
        VectorN<Scalar, N-1> Swizzle( const VectorN<int, N-1>& swiz ) const;

        static 
        VectorN Lerp( Scalar fa, const VectorN& a, const VectorN& b );
        
    private:
            Scalar mData[N];
    };

    //
    // Free-functions
    //
    
    template< typename Scalar, size_t N >
    Scalar DotProduct( const VectorN<Scalar,N>& lhs, const VectorN<Scalar,N>& rhs )
    {
        return VectorN<Scalar,N>::DotProduct( lhs, rhs );
    }

    template<typename Scalar, size_t N>
    VectorN<Scalar, N> operator+ (VectorN<Scalar, N> lhs, const VectorN<Scalar, N>& rhs)
    {
        lhs += rhs;
        return lhs;
    }
    
    template<typename Scalar, size_t N>
    VectorN<Scalar, N> operator- (VectorN<Scalar, N> lhs, const VectorN<Scalar, N>& rhs)
    {
        lhs -= rhs;
        return lhs;
    }
    
    template<typename Scalar, size_t N>
    VectorN<Scalar, N> operator/ (VectorN<Scalar, N> lhs, const Scalar rhs)
    {
        lhs /= rhs;
        return lhs;
    }

    template<typename Scalar, size_t N>
    VectorN<Scalar, N> operator- (VectorN<Scalar, N> arg)
    {
            for(size_t i=0;i!=N;++i)
            {
                arg[i] = -arg[i];   
            }
            return arg;
    }
    
    template<typename Scalar, size_t N>    
    void ComputeMidpoint(const VectorN<Scalar, N>& a, const VectorN<Scalar, N>& b, VectorN< Scalar, N>* ab )
    {
        for(size_t i=0;i!=N;++i)
            ab->Set(i, a[i] + (b[i]-a[i])/2);
    }

    template<typename Scalar, size_t N>    
    VectorN<Scalar, N> GetMidpoint(const VectorN<Scalar, N>& a, const VectorN<Scalar, N>& b)
    {
        VectorN<Scalar, N> ab( uninitialised );
        ComputeMidpoint( a, b, &ab );
        return ab;
    }

    //
    // Class Implementation
    // (in header as is a template)
    //
    
    template< typename Scalar, size_t N >
    size_t VectorN<Scalar,N>::GetSize() 
    {
        return sDimensions;
    }
    
    template< typename Scalar, size_t N >
    void VectorN<Scalar,N>::Set(size_t offset, Scalar value)
    {
        assert( offset<N );
        mData[offset]=value;
    }
    
    template< typename Scalar, size_t N >
    Scalar& VectorN<Scalar,N>::operator[] (size_t offset) 
    {
        assert( offset<N );
        return mData[offset];
    }

    template< typename Scalar, size_t N >
    Scalar VectorN<Scalar,N>::Get(size_t offset ) const 
    {
        assert( offset<N );
        return mData[offset];
    }
    
    template< typename Scalar, size_t N >
    const Scalar& VectorN<Scalar,N>::operator[] (size_t offset)  const 
    {
        assert( offset<N );
        return mData[offset];
    }
    
    template< typename Scalar, size_t N >
    Scalar VectorN<Scalar,N>::LengthSquare() const {
        Scalar l2 = 0;
        for (size_t i=0;i<N;++i)
            l2 += mData[i]*mData[i];
        return l2;
    }
    
    template< typename Scalar, size_t N >
    Scalar VectorN<Scalar,N>::Length() const {
        return Sqrt( LengthSquare() );
    }
    
    template< typename Scalar, size_t N >
    Scalar VectorN<Scalar,N>::DistanceSquare(const VectorN& rhs) const {
        Scalar l2 = 0;
        for (size_t i=0;i<N;++i)
        {
            Scalar d = rhs.mData[i] - mData[i];
            l2 += d*d;
        }
        return l2;
    }
    
    template< typename Scalar, size_t N >
    Scalar VectorN<Scalar,N>::Distance(const VectorN& rhs) const {
        return Sqrt( DistanceSquare(rhs) );
    }
    
    template< typename Scalar, size_t N >
    VectorN<Scalar,N>& VectorN<Scalar,N>::operator = (const VectorN& rhs) 
    {
        for (size_t i=0;i<N;++i)
            mData[i] = rhs.mData[i];
        return *this;
    }
    
    template< typename Scalar, size_t N >
    VectorN<Scalar,N>& VectorN<Scalar,N>::operator += (const VectorN& rhs) 
    {
        for (size_t i=0;i!=N;++i)
            mData[i] += rhs.mData[i];
        return *this;
    }
    
    template< typename Scalar, size_t N >
    VectorN<Scalar,N>& VectorN<Scalar,N>::operator -= (const VectorN& rhs) 
    {
        for (size_t i=0;i!=N;++i)
            mData[i] -= rhs.mData[i];
        return *this;
    }
    
    template< typename Scalar, size_t N >
    VectorN<Scalar,N>& VectorN<Scalar,N>::operator *= (const VectorN& rhs) 
    {
        for (size_t i=0;i!=N;++i)
            mData[i] *= rhs.mData[i];
        return *this;
    }
    
    template< typename Scalar, size_t N >
    VectorN<Scalar,N>& VectorN<Scalar,N>::operator /= (const Scalar rhs) 
    {
        for (size_t i=0;i!=N;++i)
            mData[i] /= rhs;
        return *this;
    }
    
    template< typename Scalar, size_t N >
    VectorN<Scalar,N>& VectorN<Scalar,N>::operator *= (const Scalar rhs)
    {
        for (size_t i=0;i!=N;++i)
            mData[i] *= rhs;
        return *this;
    }
    
    template< typename Scalar, size_t N >
    bool VectorN<Scalar,N>::operator == (const VectorN& rhs) const
    {
        for (size_t i=0;i<N;++i)
            if (mData[i] != rhs.mData[i]) return false;
        return true;
    }
    
    template< typename Scalar, size_t N >
    bool VectorN<Scalar,N>::operator != (const VectorN& rhs) const
    {
        return !operator==(rhs);
    }
    
    template< typename Scalar, size_t N >
    void VectorN<Scalar,N>::Normalise() 
    {
        Scalar length = Length();
        for (size_t i=0;i<N;++i)
            mData[i] /= length;
    }
    
    template< typename Scalar, size_t N >
    Scalar VectorN<Scalar,N>::DotProduct( const VectorN& lhs, const VectorN& rhs )
    {
        Scalar result = 0;
        for (size_t i=0;i<N;++i)
            result += lhs.mData[i] * rhs.mData[i];
        return result;
    }
    
    template< typename Scalar, size_t N >
    VectorN<Scalar,N> VectorN<Scalar,N>::Swizzle( const VectorN<int, N>& swiz ) const
     {
        VectorN result( uninitialised );
        for (size_t n=0;n!=N;++n)
        {
            result[n] = this->Get(swiz[n]);
        }
        return result;
    }

    template< typename Scalar, size_t N >
    VectorN<Scalar, N-1> VectorN<Scalar,N>::Swizzle( const VectorN<int, N-1>& swiz ) const
    {
        VectorN<Scalar, N-1> result( uninitialised );
        for (size_t n=0;n!=N-1;++n)
        {
            result[n] = this->Get(swiz[n]);
        }
        return result;
    }
     
    template< typename Scalar, size_t N >
    VectorN<Scalar,N> VectorN<Scalar,N>::Lerp( Scalar fa, const VectorN& a, const VectorN& b )
    {
        Scalar fb = 1.0f-fa;
        VectorN<Scalar,N> result( Geometry::uninitialised );
        for (size_t i=0;i<N;++i)
            result[i] = (a[i]*fa + b[i]*fb);
        return result;
    }

    template< typename VT >
    class LineN
    {
    public:
        typedef VT VectorType;

        LineN( const VectorType& start, const VectorType& finish )
            : mStart(start)
            , mFinish(finish)
        {
        }
        
        // bidrectional, start end end start all the same
        bool operator==(const LineN& rhs)const{
            return (mStart==rhs.mStart && mFinish==rhs.mFinish) 
            || (mStart==rhs.mFinish && mFinish==rhs.mStart)
            ;
        }

        VectorType mStart;
        VectorType mFinish;
    };

}//namespace Geometry

#endif