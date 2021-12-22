#ifndef GEOMETRY_AABB2D_H_INCLUDED_
#define GEOMETRY_AABB2D_H_INCLUDED_

#include "vector2d.h"
#include "triangle2d.h"
#include "aabb.h"

#include <cmath>

namespace Geometry
{
    //
    // Interface
    //
    template <typename Scalar>
    class AxisAlignedBoundingBox2d : public AxisAlignedBoundingBox< Vector2d<Scalar> >
    {
        public:
            typedef typename AxisAlignedBoundingBox< Vector2d<Scalar> >::VectorType VectorType;
            typedef typename VectorType::BaseType VectorBase;
            
            AxisAlignedBoundingBox2d(const VectorBase& minBound, const VectorBase& maxBound)
                : AxisAlignedBoundingBox< VectorType >( minBound, maxBound ) 
            { }
            
            AxisAlignedBoundingBox2d(const VectorBase& p)
                : AxisAlignedBoundingBox< VectorType >(p) 
            { }
            
            AxisAlignedBoundingBox2d(Scalar x1, Scalar y1, Scalar x2, Scalar y2)
                : AxisAlignedBoundingBox< VectorType >( VectorType(x1, y1), VectorType(x2, y2) ) 
            { }
            
            AxisAlignedBoundingBox2d(const Uninitialised& u)
                : AxisAlignedBoundingBox< VectorType >(u) 
            { }
            
            typename VectorType::ScalarType GetWidth() const
            {
                return this->GetAxisExtent(0);
            }
            
            typename VectorType::ScalarType GetHeight() const
            {
                return this->GetAxisExtent(1);
            }

            // should to create a triangle that encloses the AABB tightly
            Triangle2d<Scalar> GetEnclosingTriangle()const
            {
                typename Triangle2d<Scalar>::VertexType
                    a( this->mA.GetX() + this->GetWidth()/2, this->mA.GetY() - this->GetWidth() ), 
                    b( this->mB.GetX() + this->GetHeight(),  this->mB.GetY() ), 
                    c( this->mA.GetX() - this->GetHeight(),  this->mB.GetY() );
                return Triangle2d<Scalar>( a, b, c );
            }
        
            // should to create a triangle that encloses the AABB under rotation
            Triangle2d<Scalar> GetOuterEnclosingTriangle()const
            {
                Scalar d = this->GetDiagonal().Length();
                Scalar r = d / 2;
                Scalar l = r / (sqrt(3)/6);
                typename Triangle2d<Scalar>::VertexType
                    a( this->mA.GetX() + this->GetWidth()/2, this->mA.GetY() - d ), 
                    b( this->mB.GetX() + l/2,  this->mB.GetY()+r ), 
                    c( this->mA.GetX() - l/2,  this->mB.GetY()+r );
                return Triangle2d<Scalar>( a, b, c );
            }
            
    };
}    

#endif//GEOMETRY_AABB2D_H_INCLUDED_