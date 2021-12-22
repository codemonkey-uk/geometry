#ifndef GEOMETRY_TRIANGLE2D_H_INCLUDED_
#define GEOMETRY_TRIANGLE2D_H_INCLUDED_

#include "triangle.h"
#include "vector2d.h"

#include <math.h>

namespace Geometry
{
    //
    // Interface
    //
    template <typename Scalar>
    class Triangle2d : public Triangle< Vector2d<Scalar> >
    {
        public:
            typedef typename Triangle< Vector2d<Scalar> >::VectorType VectorType;
            typedef typename Triangle< Vector2d<Scalar> >::VertexType VertexType;
            
            Triangle2d(const VertexType& t1, const VertexType& t2, const VertexType& t3);
                        
            Scalar GetSurfaceArea() const;
            
            void ComputeCircumCenter( VectorType& result ) const;
            VectorType GetCircumCenter() const;
            Scalar GetCircumRadius() const;
    };
    
    //
    // Class Implementation
    // (in header as is a template)
    //
    
    template <typename Scalar>
    Triangle2d<Scalar>::Triangle2d(const VertexType& t1, const VertexType& t2, const VertexType& t3)
        : Triangle< Vector2d<Scalar> >(t1,t2,t3)
    {
        // nothing to do here
    }
    
    template <typename Scalar>
    Scalar Triangle2d<Scalar>::GetSurfaceArea() const
    {
        return Fabs (
          this->GetA().GetX() * ( this->GetC().GetY() - this->GetB().GetY() ) +
          this->GetB().GetX() * ( this->GetA().GetY() - this->GetC().GetY() ) + 
          this->GetC().GetX() * ( this->GetB().GetY() - this->GetA().GetY() )
        ) / 2;
    }

    template <typename Scalar>
    void Triangle2d<Scalar>::ComputeCircumCenter( typename Triangle2d<Scalar>::VectorType& result ) const
    {
        const Scalar asq = ( this->GetB().GetX() - this->GetA().GetX() ) * ( this->GetB().GetX() - this->GetA().GetX() ) + ( this->GetB().GetY() - this->GetA().GetY() ) * ( this->GetB().GetY() - this->GetA().GetY() );
        const Scalar csq = ( this->GetC().GetX() - this->GetA().GetX() ) * ( this->GetC().GetX() - this->GetA().GetX() ) + ( this->GetC().GetY() - this->GetA().GetY() ) * ( this->GetC().GetY() - this->GetA().GetY() );
        
        const Scalar top1 =  ( this->GetB().GetY() - this->GetA().GetY() ) * csq - ( this->GetC().GetY() - this->GetA().GetY() ) * asq;
        const Scalar top2 =  ( this->GetB().GetX() - this->GetA().GetX() ) * csq - ( this->GetC().GetX() - this->GetA().GetX() ) * asq;
        const Scalar bot  =  ( this->GetB().GetY() - this->GetA().GetY() ) * ( this->GetC().GetX() - this->GetA().GetX() )  - ( this->GetC().GetY() - this->GetA().GetY() ) * ( this->GetB().GetX() - this->GetA().GetX() );
        
        result.SetX( this->GetA().GetX() + 0.5E+00 * top1 / bot);
        result.SetY( this->GetA().GetY() - 0.5E+00 * top2 / bot);
    }

    template <typename Scalar>
    typename Triangle2d<Scalar>::VectorType Triangle2d<Scalar>::GetCircumCenter() const
    {
        VectorType result(uninitialised);
        ComputeCircumCenter(result);
        return result;
    }

    template <typename Scalar>
    Scalar Triangle2d<Scalar>::GetCircumRadius() const
    {
        Scalar asq = Sqrt ( ( this->GetB().GetX() - this->GetA().GetX() ) * ( this->GetB().GetX() - this->GetA().GetX() ) + ( this->GetB().GetY() - this->GetA().GetY() ) * ( this->GetB().GetY() - this->GetA().GetY() ) );
        Scalar bsq = Sqrt ( ( this->GetC().GetX() - this->GetB().GetX() ) * ( this->GetC().GetX() - this->GetB().GetX() ) + ( this->GetC().GetY() - this->GetB().GetY() ) * ( this->GetC().GetY() - this->GetB().GetY() ) );
        Scalar csq = Sqrt ( ( this->GetA().GetX() - this->GetC().GetX() ) * ( this->GetA().GetX() - this->GetC().GetX() ) + ( this->GetA().GetY() - this->GetC().GetY() ) * (this-> GetA().GetY() - this->GetC().GetY() ) );
        
        Scalar bot = ( asq + bsq + csq ) * ( - asq + bsq + csq ) * ( asq - bsq + csq ) * ( asq + bsq - csq );
        
        assert ( bot > 0 );
        
        return asq * bsq * csq / Sqrt ( bot );
    }

}

#endif