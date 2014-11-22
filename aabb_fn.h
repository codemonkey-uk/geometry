#ifndef GEOMETRY_AABBFN_H_INCLUDED_
#define GEOMETRY_AABBFN_H_INCLUDED_

#include "aabb.h"
#include <vector>

namespace Geometry
{
    // difference
    // outputs the set of AABBs that cover the space in B not already covered by A
    template <typename AABB, typename insertion_iterator>
    void AABB_Difference(
        AABB a, AABB b,
        insertion_iterator& ii
    )
    {
        // for each dimention that this AABB exists in
        AABB bb = b;
        for(size_t d=0;d!=AABB::VectorType::sDimensions;++d)
        {
            if (b.GetMinBound().Get(d) < a.GetMinBound().Get(d))
            {
                typename AABB::VectorType newMax( b.GetMaxBound() );
                newMax[d] = a.GetMinBound().Get(d);
                AABB result( bb.GetMinBound(), newMax );
                *ii++ = result;
                typename AABB::VectorType newMin( bb.GetMinBound() );
                newMin[d] = newMax[d];
                bb.SetMinBound( newMin );
            }
        }
        //assert( a.GetMinBound()==bb.GetMinBound() );
        for(size_t d=0;d!=AABB::VectorType::sDimensions;++d)
        {
            if (b.GetMaxBound().Get(d) > a.GetMaxBound().Get(d))
            {
                typename AABB::VectorType newMin( a.GetMinBound() );
                newMin[d] = a.GetMaxBound().Get(d);
                *ii++ = AABB( newMin, bb.GetMaxBound() );
                typename AABB::VectorType newMax( bb.GetMaxBound() );
                newMax[d] = newMin[d];
                bb.SetMaxBound( newMax );
            }
        }
        //assert( a==bb );
    }

    // edges
    // outputs the set of edges that enclose the space defined by the AABB
    template <typename AABB, typename insertion_iterator>
    void AABB_GatherEdges(
        AABB box,
        insertion_iterator& ii
    )
    {
        typedef typename AABB::VectorType Point;
        typedef std::pair<int, int> Edge;

        // create temporary storage for the edges
        std::vector< Point > corners;
        corners.reserve( iPow( 2, Point::sDimensions ) );

        std::vector< Edge > edges;

        const Point e( box.GetMaxBound() );
        const Point o( box.GetMinBound() );
        Point p = o;
        p[0] = e[0];

        corners.push_back(o);
        corners.push_back(p);
        edges.push_back( Edge(0,1) );

        for (int d=1;d!=Point::sDimensions;++d)
        {
            int cc = corners.size();
            int ec = edges.size();
            for (int c=0;c!=cc;++c)
            {
                Point a = corners[c];
                a[d]=e[d];
                corners.push_back(a);
                edges.push_back( Edge(c, c+cc) );
            }
            for (int e=0;e!=ec;++e)
                edges.push_back( Edge( edges[e].first+cc, edges[e].second+cc) );
        }

        // write all the edges to the output iterator
        for (int e=0;e!=edges.size();++e)
            *ii++ = LineN< Point >( corners[edges[e].first], corners[edges[e].second] );
    }
}

#endif