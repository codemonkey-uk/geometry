#ifndef VECTOR2D_H_INCLUDED
#define VECTOR2D_H_INCLUDED

#include "geometry_uninitialised.h"
#include "vectorn.h"

namespace Geometry
{
    //
    // Interface
    //
    
    template<typename Scalar>
    class Vector2d : public VectorN<Scalar, 2>
    {
        public:
            typedef Vector2d<Scalar> VectorType;
            typedef VectorN<Scalar,2> BaseType;
            
            // simple construction
            Vector2d( Scalar a, Scalar b );

            //explicity uninitialised construction
            Vector2d(const Uninitialised&);
                        
            //upcast constructor
            explicit Vector2d( const VectorN<Scalar, 2>& rhs );
            Vector2d& operator = (const VectorN<Scalar, 2>& rhs);
            
            //simple accessor functions
            //note !!! don't like non-const Scalar & Get but required by shaded_relief
            Scalar& GetX();
            const Scalar& GetX() const;
            void SetX( Scalar v );
            Scalar& GetY();
            const Scalar& GetY() const;
            void SetY( Scalar v );
            
            VectorType R90() const;
    };
    
    //
    // Class Implementation
    // (in header as is a template)
    //
    
    template<typename Scalar>
    Vector2d<Scalar>::Vector2d( Scalar a, Scalar b ) 
        : VectorN<Scalar, 2>(uninitialised) 
    { 
        (*this)[0]=a;
        (*this)[1]=b; 
    }

    template<typename Scalar>
    Vector2d<Scalar>::Vector2d( const VectorN<Scalar, 2>& rhs )
        : VectorN<Scalar, 2>(rhs)
    {
        // nothing to do here
    }
    
    template<typename Scalar>
    Vector2d<Scalar>::Vector2d(const Uninitialised&)
        : VectorN<Scalar, 2>(uninitialised) 
    {
        // nothing to do here
    }
    
    template<typename Scalar>
    Vector2d<Scalar>& Vector2d<Scalar>::operator = (const VectorN<Scalar, 2>& rhs)
    {
        this->BaseType::operator=(rhs);
        return *this;
    }


    template<typename Scalar>
    Scalar& Vector2d<Scalar>::GetX() 
    {
        return (*this)[0];
    }

    template<typename Scalar>
    const Scalar& Vector2d<Scalar>::GetX() const 
    {
        return (*this)[0];
    }
    
    template<typename Scalar>
    void Vector2d<Scalar>::SetX( Scalar x ) 
    {
        (*this)[0] = x;
    }
    
    template<typename Scalar>
    Scalar& Vector2d<Scalar>::GetY() 
    {
        return (*this)[1];
    }
    
    template<typename Scalar>
    const Scalar& Vector2d<Scalar>::GetY() const 
    {
        return (*this)[1];
    }
    
    template<typename Scalar>
    void Vector2d<Scalar>::SetY( Scalar y ) 
    {
        (*this)[1] = y;
    }

    template<typename Scalar>
    Vector2d<Scalar> Vector2d<Scalar>::R90() const
    {
        return VectorType{-GetY(),GetX()};
    }

	template< typename Scalar >
	class Line2d : public LineN< Vector2d<Scalar> >
	{
	public:
		typedef Vector2d<Scalar> VectorType;
		typedef Scalar ScalarType;

        Line2d( const VectorType& start, const VectorType& finish )
            : LineN<Vector2d<Scalar> >(start, finish)
        {
        }
        
        Line2d( 
            const typename VectorType::BaseType& start, 
            const typename VectorType::BaseType& finish )
            : LineN<Vector2d<Scalar> >( VectorType(start), VectorType(finish))
        {
        }

		VectorType Normal()const
		{
			VectorType result(this->mFinish);
			result -= this->mStart;
			result.Normalise();
			ScalarType temp = result.Get(0);
			result.Set(0, -result.Get(1));
			result.Set(1, temp);
			return result;
		}

		ScalarType SignedDistance(const VectorType& p)const
		{
            VectorType v(p - this->mStart);
			return DotProduct(v, Normal());
		}
		
        ScalarType SegmentDistance(const VectorType& p) const
        {
            // Return minimum distance between line segment vw and point p
            const float l2 = this->mStart.DistanceSquare(this->mFinish);  // i.e. |w-v|^2 -  avoid a sqrt
            if (l2 == 0.0) return p.Distance(this->mStart);   // v == w case
            // Consider the line extending the segment, parameterized as v + t (w - v).
            // We find projection of point p onto the line. 
            // It falls where t = [(p-v) . (w-v)] / |w-v|^2
            // We clamp t from [0,1] to handle points outside the segment vw.
            float t = DotProduct(p - this->mStart, this->mFinish - this->mStart) / l2;
            if (t>1) t=1;
            if (t<0) t=0;
            // const VectorType projection = this->mStart + (this->mFinish - this->mStart) * t;  // Projection falls on the segment
            auto projection = (this->mFinish - this->mStart);
            projection *= t;
            projection += this->mStart;
            return p.Distance(projection);
        }
        
        ScalarType Side(const VectorType& p)const
        {
            VectorType ab(this->mFinish - this->mStart);
            VectorType ap(p - this->mStart);
            return (ab.GetX()*ap.GetY())-(ab.GetY()*ap.GetX());
        }
        
        // from Andre LeMothe's "Tricks of the Windows Game Programming Gurus"
        // Returns 1 if the lines intersect, otherwise 0. In addition, if the lines 
        // intersect the intersection point may be stored in the floats i_x and i_y.
        bool Intersection(const Line2d& other, VectorType* result)
        {
            float p0_x = this->mStart.GetX();
            float p0_y = this->mStart.GetY(); 
            float p1_x = this->mFinish.GetX();
            float p1_y = this->mFinish.GetY();
            float p2_x = other.mStart.GetX();
            float p2_y = other.mStart.GetY(); 
            float p3_x = other.mFinish.GetX();
            float p3_y = other.mFinish.GetY();
            
            float s1_x, s1_y, s2_x, s2_y;
            s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
            s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

            float s, t;
            s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
            t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

            if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
            {
                // Collision detected
                if (result != NULL)
                {
                    result->SetX( p0_x + (t * s1_x) );
                    result->SetY( p0_y + (t * s1_y) );
                }
                return true;
            }

            return false; // No collision
        }
    };
}

#endif