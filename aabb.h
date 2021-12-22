#ifndef GEOMETRY_AABB_H_INCLUDED_
#define GEOMETRY_AABB_H_INCLUDED_

#include <cmath>
#include <algorithm>
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
            typedef typename VectorType::BaseType VectorBase;
            
            AxisAlignedBoundingBox(const VectorBase& minBound, const VectorBase& maxBound);
            AxisAlignedBoundingBox(const VectorBase& p);
            AxisAlignedBoundingBox(const Uninitialised&); 
        
            const VectorType& GetMinBound() const;
            const VectorType& GetMaxBound() const;
            void SetMinBound(const VectorBase&);
            void SetMaxBound(const VectorBase&);
            
            VectorType GetCenter() const;
            bool Contains( const VectorBase& p ) const;
            bool Contains( const AxisAlignedBoundingBox& rhs ) const;
            void ExpandToContain( const VectorBase& p );
            void ExpandToContain( const AxisAlignedBoundingBox& rhs );
            
            typename VectorType::ScalarType GetAxisExtent(size_t axis) const;
            VectorType GetDiagonal() const;
            
            void Move(const VectorBase& direction);
            void MoveMinBound(const VectorBase& direction);
            void MoveMaxBound(const VectorBase& direction);
        
            typename VectorType::ScalarType GetInradius()const;
            typename VectorType::ScalarType GetArea()const;
            typename VectorType::ScalarType GetVolume()const{return GetArea();}
            VectorType GetIncenter()const;
            typename VectorType::ScalarType GetCircumradius()const;
        
            bool Overlaps(const AxisAlignedBoundingBox& rhs)const;
            
            template<typename RhsAabbType>
            RhsAabbType Overlap(const RhsAabbType& rhs)const;
            
            typename VectorType::ScalarType Distance(const VectorType c) const;
            
        protected:
            VectorType mA,mB;
    };

    //
    // Class Implementation
    // (in header as is a template)
    //
    
    template<typename T>
    AxisAlignedBoundingBox<T>::AxisAlignedBoundingBox(const VectorBase& minBound, const VectorBase& maxBound)
        : mA(minBound), mB(maxBound) 
    { 
    }
    
    template<typename T>
    AxisAlignedBoundingBox<T>::AxisAlignedBoundingBox(const VectorBase& p)
        : mA(p), mB(p)
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
    }
    
    template<typename T>
    void AxisAlignedBoundingBox<T>::SetMaxBound(const VectorBase& p)
    {
        mB = p;
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
            if ((mA[d] > p[d]) || (mB[d] <= p[d])) return false;
        }

        return true;
    }
    
    template<typename T>
    bool AxisAlignedBoundingBox<T>::Contains( const AxisAlignedBoundingBox& rhs ) const
    {
        return Contains(rhs.GetMinBound()) && Contains(rhs.GetMaxBound());
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
    void AxisAlignedBoundingBox<T>::ExpandToContain( const AxisAlignedBoundingBox& rhs )
    {
        ExpandToContain(rhs.GetMinBound());
        ExpandToContain(rhs.GetMaxBound());
    }

    template<typename T>
    typename AxisAlignedBoundingBox<T>::VectorType::ScalarType AxisAlignedBoundingBox<T>::GetAxisExtent(size_t axis) const
    {
        return mB[axis] - mA[axis];
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
    typename AxisAlignedBoundingBox<T>::VectorType::ScalarType AxisAlignedBoundingBox<T>::GetCircumradius()const
    {
        return this->GetDiagonal().Length() / 2;
    }

    template<typename T>
    typename AxisAlignedBoundingBox<T>::VectorType::ScalarType AxisAlignedBoundingBox<T>::GetInradius()const
    {
        typename AxisAlignedBoundingBox<T>::VectorType::ScalarType a,b;
        a = GetAxisExtent(0); 
        for (size_t d = 1; d!=VectorBase::sDimensions; ++d)
        {
            b = GetAxisExtent(d);
            if (b<a) a=b;			
        }
        return a/2;
    }
    
    template<typename T>
    typename AxisAlignedBoundingBox<T>::VectorType::ScalarType AxisAlignedBoundingBox<T>::GetArea()const
    {
        typename AxisAlignedBoundingBox<T>::VectorType::ScalarType a,b;
        a = GetAxisExtent(0); 
        for (size_t d = 1; d!=VectorBase::sDimensions; ++d)
        {
            b = GetAxisExtent(d);
            a=a*b;
        }
        return a;
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
            if (mA[d] >= rhs.mB[d] || mB[d] <= rhs.mA[d])
                return false;
        }
        return true;
    }
    
    template<typename T>
    template<typename RhsAabbType>
    RhsAabbType AxisAlignedBoundingBox<T>::Overlap(const RhsAabbType& rhs)const
    {
        RhsAabbType result(Geometry::uninitialised);
        for (size_t d = 0; d!=VectorBase::sDimensions; ++d)
        {
            result.mA[d] = std::max(mA[d],rhs.mA[d]);
            result.mB[d] = std::min(mB[d],rhs.mB[d]);
        }
        return result;
    }
    
    template<typename T>
    typename AxisAlignedBoundingBox<T>::VectorType::ScalarType AxisAlignedBoundingBox<T>::Distance(const VectorType p) const
    {
        VectorType d(Geometry::uninitialised);
        typename VectorType::ScalarType a=0,b,c;
        for (size_t n = 0; n!=VectorBase::sDimensions; ++n)
        {
            b=mA[n] - p[n];
            c=p[n] - mB[n];
            d[n]=std::max(a,std::max(b,c));
        }
        return d.Length();
    }
}

#endif//GEOMETRY_AABB_H_INCLUDED_