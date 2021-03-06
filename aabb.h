#ifndef GEOMETRY_AABB_H_INCLUDED_
#define GEOMETRY_AABB_H_INCLUDED_

#include <math.h>
#include <algorithm> // for max/min
#include "geometry_uninitialised.h"

namespace Geometry
{
    //
    // Interface
    //
    template<typename T>
    class AxisAlignedBoundingBox
    {
        public:
            typedef T VectorType;
            typedef typename VectorType::ScalarType ScalarType;
            typedef typename VectorType::BaseType VectorBase;

            AxisAlignedBoundingBox(const VectorType& minBound, const VectorType& maxBound);
            AxisAlignedBoundingBox(const VectorType& origin);
            AxisAlignedBoundingBox(const Uninitialised&);

            const VectorType& GetMinBound() const;
            const VectorType& GetMaxBound() const;
            void SetMinBound(const VectorBase&);
            void SetMaxBound(const VectorBase&);

            VectorType GetCenter() const;
            bool Contains( const VectorBase& p ) const;
            bool Contains(const AxisAlignedBoundingBox& rhs)const;
            void ExpandToContain( const VectorBase& p );
            void ExpandToContain( const AxisAlignedBoundingBox& rhs);

            ScalarType GetAxisExtent(size_t axis) const;
            VectorType GetDiagonal() const;

            ScalarType GetVolume()const;

            void Move(const VectorBase& direction);
            void MoveMinBound(const VectorBase& direction);
            void MoveMaxBound(const VectorBase& direction);

            ScalarType GetInradius()const;
            VectorType GetIncenter()const;

            ScalarType GetCircumradius()const;

            bool Overlaps(const AxisAlignedBoundingBox& rhs)const;

            //compare sizes of AABBs
            bool CanContain( const AxisAlignedBoundingBox& rhs )const;

        protected:
            VectorType mA,mB;
    };

    //
    // Free-functions
    //

    // Intersection (like a set intersection),
    // returns an AABB that covers the overlap of a and b
    template< typename VectorType >
    Geometry::AxisAlignedBoundingBox< VectorType > Intersection(
        const Geometry::AxisAlignedBoundingBox< VectorType >& a,
        const Geometry::AxisAlignedBoundingBox< VectorType >& b
    )
    {
        VectorType c(Geometry::uninitialised),d(Geometry::uninitialised);
        for (int i=0;i!=VectorType::sDimensions;++i)
        {
            c[i] = std::max(a.GetMinBound()[i],b.GetMinBound()[i]);
            d[i] = std::min(a.GetMaxBound()[i],b.GetMaxBound()[i]);
        }

        return Geometry::AxisAlignedBoundingBox< VectorType >(c,d);
    }

    //
    // Class Implementation
    // (in header as is a template)
    //

    template<typename T>
    AxisAlignedBoundingBox<T>::AxisAlignedBoundingBox(const VectorType& minBound, const VectorType& maxBound)
        : mA(minBound), mB(maxBound)
    {
        //assert(mA <= mB);
    }

    template<typename T>
    AxisAlignedBoundingBox<T>::AxisAlignedBoundingBox(const VectorType& o)
        : mA(o), mB(o)
    {
    }

    template<typename T>
    AxisAlignedBoundingBox<T>::AxisAlignedBoundingBox(const Uninitialised&)
        : mA(uninitialised), mB(uninitialised)
    {
    }

    template<typename T>
    const typename AxisAlignedBoundingBox<T>::VectorType& AxisAlignedBoundingBox<T>::GetMinBound() const
    {
        return mA;
    }

    template<typename T>
    const typename AxisAlignedBoundingBox<T>::VectorType& AxisAlignedBoundingBox<T>::GetMaxBound() const
    {
        return mB;
    }

    template<typename T>
    void AxisAlignedBoundingBox<T>::SetMinBound(const VectorBase& p)
    {
        mA = p;
        //assert(mA <= mB);
    }

    template<typename T>
    void AxisAlignedBoundingBox<T>::SetMaxBound(const VectorBase& p)
    {
        mB = p;
        //assert(mA <= mB);
    }

    template<typename T>
    typename AxisAlignedBoundingBox<T>::VectorType AxisAlignedBoundingBox<T>::GetCenter() const
    {
        VectorType result(uninitialised);
        ComputeMidpoint(mA,mB,&result);
        return result;
    }

    template<typename T>
    bool AxisAlignedBoundingBox<T>::Contains( const VectorBase& p ) const
    {
        for (size_t d = 0; d!=VectorBase::sDimensions; ++d)
        {
            if ((mA[d] > p[d]) || (mB[d] < p[d])) return false;
        }

        return true;
    }

    template<typename T>
    bool AxisAlignedBoundingBox<T>::Contains( const AxisAlignedBoundingBox<T>& rhs ) const
    {
        return Contains(rhs.mA) && Contains(rhs.mB);
    }

    template<typename T>
    void AxisAlignedBoundingBox<T>::ExpandToContain( const VectorBase& p )
    {
        for (size_t d = 0; d!=VectorBase::sDimensions; ++d)
        {
            if (mA[d] > p[d]) mA[d] = p[d];
            if (mB[d] < p[d]) mB[d] = p[d];
        }
    }

    template<typename T>
    void AxisAlignedBoundingBox<T>::ExpandToContain( const AxisAlignedBoundingBox<T>& rhs )
    {
        ExpandToContain(rhs.mA);
        ExpandToContain(rhs.mB);
    }

    template<typename T>
    typename AxisAlignedBoundingBox<T>::ScalarType AxisAlignedBoundingBox<T>::GetAxisExtent(size_t axis) const
    {
        return mB[axis] - mA[axis];
    }

    template<typename T>
    typename AxisAlignedBoundingBox<T>::ScalarType AxisAlignedBoundingBox<T>::GetVolume()const
    {
        ScalarType result = GetAxisExtent(0);
        for (size_t d = 1; d!=VectorBase::sDimensions; ++d)
        {
            result *= GetAxisExtent(d);
        }
        return result;
    }

    template<typename T>
    typename AxisAlignedBoundingBox<T>::VectorType AxisAlignedBoundingBox<T>::GetDiagonal() const
    {
        return typename AxisAlignedBoundingBox<T>::VectorType(mB - mA);
    }

    template<typename T>
    void AxisAlignedBoundingBox<T>::Move(const VectorBase& direction)
    {
        mA += direction;
        mB += direction;
    }

    template<typename T>
    void AxisAlignedBoundingBox<T>::MoveMinBound(const VectorBase& direction)
    {
            mA += direction;
    }

    template<typename T>
    void AxisAlignedBoundingBox<T>::MoveMaxBound(const VectorBase& direction)
    {
            mB += direction;
    }

    template<typename T>
    typename AxisAlignedBoundingBox<T>::ScalarType AxisAlignedBoundingBox<T>::GetInradius()const
    {
        ScalarType a,b;
        a = GetAxisExtent(0);
        for (size_t d = 1; d!=VectorBase::sDimensions; ++d)
        {
            b = GetAxisExtent(d);
            if (b<a) a=b;
        }
        return a/2;
    }

    template<typename T>
    typename AxisAlignedBoundingBox<T>::ScalarType AxisAlignedBoundingBox<T>::GetCircumradius()const
    {
        return this->GetDiagonal().Length() / 2;
    }

    template<typename T>
    typename AxisAlignedBoundingBox<T>::VectorType AxisAlignedBoundingBox<T>::GetIncenter()const
    {
        return GetCenter();
    }

    template<typename T>
    bool AxisAlignedBoundingBox<T>::Overlaps(const AxisAlignedBoundingBox<T>& rhs)const
    {
        for (size_t d = 0; d!=VectorBase::sDimensions; ++d)
        {
            if (mA[d] > rhs.mB[d] || mB[d] < rhs.mA[d])
                return false;
        }
        return true;
    }

    template<typename T>
    bool AxisAlignedBoundingBox<T>::CanContain(const AxisAlignedBoundingBox<T>& rhs)const
    {
        for (size_t d = 0; d!=VectorBase::sDimensions; ++d)
        {
            if (GetAxisExtent(d)<rhs.GetAxisExtent(d))
                return false;
        }
        return true;
    }
}

#endif//GEOMETRY_AABB_H_INCLUDED_