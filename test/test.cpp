#include "../matrixn.h"
#include "../vector3d.h"

using namespace Geometry;
	
int tests_passed = 0;
int tests_failed = 0;
#define TEST( condition ) if (!(condition)) { \
	printf("failed: %s\n", #condition); tests_failed++; \
	} else { tests_passed++; } 

void Flush(const char* name)
{
	printf("%s: %i passed\n", name, tests_passed);
	tests_passed = 0;
}

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
			
	Flush("TestTranslate");
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
	test1.Scale(2);
	TEST( test1 * v == v2 );
	
	MatrixN<int,4> test2;
	test2.Scale(v);
	TEST( test2 * v == v_sq );
	
	Flush("TestScale");
}


int main()
{	
	TestLayout();
	TestTranslate();
	TestScale();
	
	// Geometry::MatrixN<int,4> matrix11({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
	// in OGL format
	// x.x x.y x.z 0
	// y.x y.y y.z 0
	// z.x z.y z.z 0
	// p.x p.y p.z 1
	
	printf("%i failed\n", tests_failed);
	return tests_failed;
}