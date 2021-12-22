#include "../geometry_constants.h"
#include "../matrixn.h"
#include "../matrix4.h"
#include "../vector3d.h"

#include "../aabb.h"
#include "../aabb2d.h"
#include "../aabb_fn.h"

#include <vector>

using namespace Geometry;

// --- TEST HELPERS ---

int tests_passed = 0;
int tests_failed = 0;
#define TEST( condition ) if (!(condition)) { \
	printf("failed: %s\n", #condition); tests_failed++; \
	} else { tests_passed++; }

void Flush(const char* name)
{
	printf("%s:\t%i passed\n", name, tests_passed);
	tests_passed = 0;
}

void DebugPrint(const Geometry::MatrixN<int,4>& matrix)
{
	for (int n=0;n!=4;n++)
	{
		printf("%i,%i,%i,%i\n", matrix[n][0],matrix[n][1],matrix[n][2],matrix[n][3]);
	}
}

void DebugPrint(const Geometry::MatrixN<float,4>& matrix)
{
	for (int n=0;n!=4;n++)
	{
		printf("%f,%f,%f,%f\n", matrix[n][0],matrix[n][1],matrix[n][2],matrix[n][3]);
	}
}

void DebugPrint(const Geometry::MatrixN<double,4>& matrix)
{
	for (int n=0;n!=4;n++)
	{
		printf("%lf,%lf,%lf,%lf\n", matrix[n][0],matrix[n][1],matrix[n][2],matrix[n][3]);
	}
}

void DebugPrint(Geometry::AxisAlignedBoundingBox2d< int > aabb)
{
    printf("AABB: %i,%i->%i,%i\n",
        aabb.GetMinBound().GetX(),
        aabb.GetMinBound().GetY(), 
        aabb.GetMaxBound().GetX(),
        aabb.GetMaxBound().GetY()
    );
}

void DebugPrint(std::vector< Geometry::AxisAlignedBoundingBox2d< int > >& r)
{
    for (auto aabb : r)
    {
        DebugPrint(aabb);
    }
}

// --- TESTS ---

void TestLayout()
{
	Geometry::MatrixN<int,4> matrixA = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	int matrixB[4][4] = { { 0, 1, 2, 3 }, { 4, 5, 6, 7 }, { 8, 9, 10, 11 }, { 12, 13, 14, 15 } };
	Geometry::MatrixN<int,4> matrixC(matrixA);
	for (int n=0;n!=4;n++)
		for (int m=0;m!=4;m++)
			TEST( matrixB[n][m]==matrixC[n][m] );
	Flush("TestLayout");
}

void TestTranslate()
{
	int dx=1;
	int dy=2;
	int dz=4;

	// translation matrix
	MatrixN<int,4> t({
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		dx, dy, dz, 1
	});

	// position vector
	VectorN<int,4> v({8,16,32,1});

	VectorN<int,4> v2 = t * v;

	// should be translated
	TEST( v2[0]==v[0]+dx );
	TEST( v2[1]==v[1]+dy );
	TEST( v2[2]==v[2]+dz );

	// direction vector
	v[3]=0;
	v2 = t * v;

	// should be unchanged
	TEST( v2[0]==v[0] );
	TEST( v2[1]==v[1] );
	TEST( v2[2]==v[2] );

	// construct from vector:
	const Vector3d<int> tv(2,3,4);
	MatrixN<int, 4> mt = MatrixN<int, 4>::Translation(tv);
	TEST( mt[3][0] == tv[0] );
	TEST( mt[3][1] == tv[1] );
	TEST( mt[3][2] == tv[2] );

	Flush("TestTranslate");
}

void TestRotate()
{
	TEST( Matrix4<float>::RotationAroundZ(0) == Matrix4<float>::Identity() );
	TEST( Matrix4<double>::RotationAroundZ(0) == Matrix4<double>::Identity() );

	// TODO

	// Negating the rotation angle is equivalent to generating the transpose of the matrix.

  	// If a rotation matrix is multiplied with its transpose, the result is the identity matrix.

  	Flush("TestRotate");
}

void TestIdentity()
{
	// two ways to get identity matrix

	// manually, create uninitialised, then call BecomeIdentity
	MatrixN<int,4> test1(uninitialised);
	test1.BecomeIdentity();

	// or default constructor
	MatrixN<int,4> test2;

	for (int n=0;n!=4;n++)
	{
		for (int m=0;m!=4;m++)
		{
			TEST( test1[n][m]==(n==m) );
			TEST( test2[n][m]==(n==m) );
		}
	}

	// check that identity * vector leaves vector unmodified
	VectorN<int,4> v({8,16,32,1});
	TEST( test2 * v == v );

	Flush("TestIdentity");
}

void TestScale()
{
	// vector we will scale for the tests
	const VectorN<int,4> v({2,4,8,1});
	// v*2, note w should not be scaled!
	const VectorN<int,4> v2({2*2,4*2,8*2,1});
	// v*v, note w should not be scaled!
	const VectorN<int,4> v_sq({2*2,4*4,8*8,1});

	MatrixN<int,4> test1;
	test1.BecomeScale(2);
	TEST( test1 * v == v2 );

	MatrixN<int,4> test2;
	test2.BecomeScale(v);
	TEST( test2 * v == v_sq );

	Flush("TestScale");
}

void TestTranspose()
{
	const Geometry::MatrixN<int,4> matrixA = {
		0, 1, 2, 3,
		4, 5, 6, 7,
		8, 9, 10, 11,
		12, 13, 14, 15
	};
	Geometry::MatrixN<int,4> matrixB = matrixA;
	matrixB.Transpose();

	TEST( matrixA[0][0]	== matrixB[0][0] );
	TEST( matrixA[1][0]	== matrixB[0][1] );
	TEST( matrixA[2][0]	== matrixB[0][2] );
	TEST( matrixA[3][0]	== matrixB[0][3] );
	TEST( matrixA[0][1]	== matrixB[1][0] );
	TEST( matrixA[1][1]	== matrixB[1][1] );
	TEST( matrixA[2][1]	== matrixB[1][2] );
	TEST( matrixA[3][1]	== matrixB[1][3] );
	TEST( matrixA[0][2]	== matrixB[2][0] );
	TEST( matrixA[1][2]	== matrixB[2][1] );
	TEST( matrixA[2][2]	== matrixB[2][2] );
	TEST( matrixA[3][2]	== matrixB[2][3] );
	TEST( matrixA[0][3]	== matrixB[3][0] );
	TEST( matrixA[1][3]	== matrixB[3][1] );
	TEST( matrixA[2][3]	== matrixB[3][2] );
	TEST( matrixA[3][3]	== matrixB[3][3] );

	TEST( matrixB == matrixA.GetTranspose() );

	// non orthogonal transpose
	const Geometry::MatrixNM<int,2,4> matrixC = {
		0, 1, 2, 3,
		4, 5, 6, 7
	};
	Geometry::MatrixNM<int,4,2> matrixD = matrixC.GetTranspose();
	TEST( matrixD[0][0]==matrixC[0][0] );
	TEST( matrixD[1][0]==matrixC[0][1] );
	TEST( matrixD[2][0]==matrixC[0][2] );
	TEST( matrixD[3][0]==matrixC[0][3] );
	TEST( matrixD[0][1]==matrixC[1][0] );
	TEST( matrixD[1][1]==matrixC[1][1] );
	TEST( matrixD[2][1]==matrixC[1][2] );
	TEST( matrixD[3][1]==matrixC[1][3] );

	Flush("TestTranspose");
}

void TestMultiply()
 {
	int a= 0, b= 1, c= 2, d= 3,
		e= 4, f= 5, g= 6, h= 7,
		i= 8, j= 9, k=10, l=11,
		m=12, n=13;//, o=14, p=15;
	int q=20, r=21, s=22, t=23, u=24, v=25, w=26, x=27;
	const Geometry::MatrixN<int,4> matrixA = {
		0, 1, 2, 3,
		4, 5, 6, 7,
		8, 9, 10, 11,
		12, 13, 14, 15
	};

	// 4x4*4x4
	Geometry::MatrixN<int,4> matrixB = matrixA*matrixA;
	int a2 = a*a + b*e + c*i + d*m;
	TEST( matrixB[0][0] == a2 );
	int b2 = a*a + b*f + c*j + d*n;
	TEST( matrixB[0][1] == b2 );
	int j2 = i*b + j*f + k*j + l*n;
	TEST( matrixB[2][1] == j2 );

	const Geometry::MatrixNM<int,4,2> matrixC = {
		q, r,
		s, t,
		u, v,
		w, x
	};

	// 4x4 * 4x2
	Geometry::MatrixNM<int,4,2> matrixD = matrixA * matrixC;
	TEST( matrixD[0][0] == a*q + b*s + c*u + d*w );
	TEST( matrixD[0][1] == a*r + b*t + c*v + d*x );

	const Geometry::MatrixNM<int,2,4> matrixE = {
		a, b, c, d,
		e, f, g, h
	};

	Geometry::MatrixNM<int,2,4> matrixF = matrixC * matrixE;
	TEST( matrixF[0][0] == q*a + r*e );

	// 2x4 * 4x2 runtime error :(
	// matrixE * matrixC;

	// 4x2 * 4*4 does not compile:
	// matrixC * matrixA;

	Flush("TestMultiply");
}

void TestPow()
{
	const Geometry::MatrixN<int,4> matrixA = {
		0, 1, 2, 3,
		4, 5, 6, 7,
		8, 9, 10, 11,
		12, 13, 14, 15
	};
	const Geometry::MatrixN<int,4> matrixI;

	// m^0 == Identity
	TEST( matrixA.Pow(0) == matrixI );

	// m^1 == m
	TEST( matrixA.Pow(1) == matrixA );

	// m^2 == m*m
	TEST( matrixA.Pow(2) == matrixA*matrixA );

	// m^3 == m*m*m
	TEST( matrixA.Pow(3) == matrixA*matrixA*matrixA );

	// Raising the identity matrix to any power always generates the identity
	TEST( matrixI.Pow(3) == matrixI );
	TEST( matrixI.Pow(4) == matrixI );
	TEST( matrixI.Pow(5) == matrixI );

	TEST( Pow(2,3)==8 );
	TEST( Pow(5,5)==3125 );

	Flush("TestPow");
}

template< int n >
void TestAABB_GatherEdges(int expect)
{
	Geometry::AxisAlignedBoundingBox< VectorN<int,n> >
		aabb( uninitialised );

	std::vector< LineN< VectorN<int,n> > > edges;
	std::back_insert_iterator< std::vector< LineN< VectorN<int,n> > > > ii = std::back_inserter(edges);
	Geometry::AABB_GatherEdges( aabb, ii );

	TEST( edges.size()==expect );
}

void TestAABB_Difference2d( 
    Geometry::AxisAlignedBoundingBox2d< int > a,
    Geometry::AxisAlignedBoundingBox2d< int > b,
    int expected
)
{
    std::vector< Geometry::AxisAlignedBoundingBox2d< int > > r;
    auto itr = std::back_inserter(r);
    AABB_Difference( a,b,itr );
    
    TEST( r.size()==expected );
    if (r.size()!=expected)
    {
        printf("expected %i got %i:\n",expected,(int)r.size());
        DebugPrint(r);
    }
    // no overlaps in results
    for (const auto& ra:r)
    {
        // no overlaps with original
        TEST(a.Overlaps(ra)==false);
        if (a.Overlaps(ra))
        {
            printf("Unexpected overlaps against A in difference set:\n");
            DebugPrint(a);
            DebugPrint(ra);
        }
        
        for (const auto& rb:r)
        {
            if (&ra!=&rb)
            {
                TEST(ra.Overlaps(rb)==false);
                if (ra.Overlaps(rb))
                {
                    printf("Unexpected overlaps in difference set:\n");
                    DebugPrint(ra);
                    DebugPrint(rb);
                }
            }
        }
    }
        
}

void TestAABB_Difference2d_Same()
{
    Geometry::AxisAlignedBoundingBox2d< int >
        a( {0,0},{10,10} );
    Geometry::AxisAlignedBoundingBox2d< int >
        b( {0,0},{10,10} );

    TestAABB_Difference2d(a,b,0);
}

void TestAABB_Difference2d_NoOverlap()
{
    Geometry::AxisAlignedBoundingBox2d< int >
        a( {0,0},{2,2} );
    Geometry::AxisAlignedBoundingBox2d< int >
        b( {4,4},{8,8} );

    TestAABB_Difference2d(a,b,1);
}

void TestAABB_Difference2d_AContainsB()
{
    Geometry::AxisAlignedBoundingBox2d< int >
        a( {0,0},{10,10} );
    Geometry::AxisAlignedBoundingBox2d< int >
        b( {4,4},{8,8} );

    TestAABB_Difference2d(a,b,0);
}

void TestAABB_Difference2d_BExtendsA1Axis()
{
    Geometry::AxisAlignedBoundingBox2d< int >
        a( {0,0},{5,5} );
    Geometry::AxisAlignedBoundingBox2d< int >
        b( {0,0},{5,8} );

    TestAABB_Difference2d(a,b,1);
}

void TestAABB_Difference2d_BExtendsA2Axis_Max()
{
    Geometry::AxisAlignedBoundingBox2d< int >
        a( {0,0},{5,5} );
    Geometry::AxisAlignedBoundingBox2d< int >
        b( {0,0},{8,8} );

    TestAABB_Difference2d(a,b,2);
}

void TestAABB_Difference2d_BExtendsA2Axis_Min()
{
    Geometry::AxisAlignedBoundingBox2d< int >
        a( {0,0},{5,5} );
    Geometry::AxisAlignedBoundingBox2d< int >
        b( {-5,-5},{5,5} );

    TestAABB_Difference2d(a,b,2);
}

void TestAABB_Difference2d_BExtendsA2Axis()
{
    TestAABB_Difference2d_BExtendsA2Axis_Max();
    TestAABB_Difference2d_BExtendsA2Axis_Min();
}

void TestAABB_Difference2d()
{
    TestAABB_Difference2d_Same();
    TestAABB_Difference2d_NoOverlap();
    TestAABB_Difference2d_AContainsB();
    TestAABB_Difference2d_BExtendsA1Axis();
    TestAABB_Difference2d_BExtendsA2Axis();
}

void TestAABB()
{
	Geometry::AxisAlignedBoundingBox< VectorN<int,2> >
		aabb( uninitialised );

	VectorN<int,2> p1({1,1});
	aabb = p1;

    // a point volume has zero size so should contain nothing
	TEST( aabb.Contains(p1)==false );
	TEST( aabb.GetMinBound()==p1 );
	TEST( aabb.GetMaxBound()==p1 );
	TEST( aabb.GetCenter()==p1 );
	TEST( aabb.GetAxisExtent(0)==0 );
	TEST( aabb.GetVolume()==0 );
	TEST( aabb.GetInradius()==0 );
	TEST( aabb.GetIncenter()==p1 );
	TEST( aabb.GetCircumradius()==0 );

	VectorN<int,2> p2({2,2});
	TEST( aabb.Contains(p2)==false );
	
	Geometry::AxisAlignedBoundingBox< VectorN<int,2> > aabb2(p1,p2);
	TEST( aabb2.Contains(p1) );
	TEST( aabb2.Contains(p2)==false );	
	TEST( aabb2.GetMinBound()==p1 );
	TEST( aabb2.GetMaxBound()==p2 );
	TEST( aabb2.GetAxisExtent(0)==1 );
	TEST( aabb2.GetVolume()==1 );

	VectorN<int,2> p3({3,3});
	Geometry::AxisAlignedBoundingBox< VectorN<int,2> > aabb3(p1,p3);
	TEST( aabb3.Contains(p1) );
	TEST( aabb3.Contains(p2) );	
	TEST( aabb3.GetMinBound()==p1 );
	TEST( aabb3.GetMaxBound()==p3 );
	TEST( aabb3.GetAxisExtent(0)==2 );
	TEST( aabb3.GetVolume()==4 );
	

	TestAABB_GatherEdges<1>(1);
	TestAABB_GatherEdges<2>(4);
	TestAABB_GatherEdges<3>(12);
	TestAABB_GatherEdges<4>(12+12+8);
	TestAABB_GatherEdges<5>(80);

    TestAABB_Difference2d();
    
	Flush("TestAABB");
}

void TestSwizzle()
{
	VectorN<int,2> v1a({10,0});
	VectorN<int,3> v1b({10,0,20});
	VectorN<int,2> v2({1,0});
	VectorN<int,2> v3({0,10});

	TEST( v1a.Swizzle( v2 ) == v3 );
	TEST( v1b.Swizzle( v2 ) == v3 );

	Flush("TestSwizzle");
}

int main()
{
	TestLayout();
	TestTranslate();
	TestRotate();
	TestScale();
	TestTranspose();
	TestMultiply();
	TestPow();
	TestAABB();
	TestSwizzle();

	// Geometry::MatrixN<int,4> matrix11({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
	// in OGL format
	// x.x x.y x.z 0
	// y.x y.y y.z 0
	// z.x z.y z.z 0
	// p.x p.y p.z 1

	printf("=> %i failed%c\n", tests_failed, tests_failed==0 ? '!' : '.');
	return tests_failed;
}