#include "../matrixn.h"
#include "../vector3d.h"

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

void DebugPrint(Geometry::MatrixN<int,4>& matrix)
{
	for (int n=0;n!=4;n++)
	{
		printf("%i,%i,%i,%i\n", matrix[n][0],matrix[n][1],matrix[n][2],matrix[n][3]);
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
		
	Flush("TestTranspose");
}

void TestMultiply()
 {
	int a= 0, b= 1, c= 2, d= 3, 
		e= 4, f= 5, g= 6, h= 7, 
		i= 8, j= 9, k=10, l=11, 
		m=12, n=13, o=14, p=15;
	int q=20, r=21, s=22, t=23, u=24, v=25, w=26, x=27;
	Geometry::MatrixN<int,4> matrixA = { 
		0, 1, 2, 3, 
		4, 5, 6, 7, 
		8, 9, 10, 11, 
		12, 13, 14, 15 
	};	
	Geometry::MatrixN<int,4> matrixB = matrixA*matrixA;
	
	int a2 = a*a + b*e + c*i + d*m;
	TEST( matrixB[0][0] == a2 );
	int b2 = a*a + b*f + c*j + d*n;
	TEST( matrixB[0][1] == b2 );
	int j2 = i*b + j*f + k*j + l*n;
	TEST( matrixB[2][1] == j2 );
	
	Geometry::MatrixNM<int,4,2> matrixC = {
		q, r,
		s, t,
		u, v, 
		w, x
	};
	
	Geometry::MatrixNM<int,4,2> matrixD = matrixA * matrixC;
	TEST( matrixD[0][0] == a*q + b*s + c*u + d*w );
	TEST( matrixD[0][1] == a*r + b*t + c*v + d*x );	
	
	Flush("TestMultiply");
}

int main()
{	
	TestLayout();
	TestTranslate();
	TestScale();
	TestTranspose();
	TestMultiply();
	
	// Geometry::MatrixN<int,4> matrix11({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
	// in OGL format
	// x.x x.y x.z 0
	// y.x y.y y.z 0
	// z.x z.y z.z 0
	// p.x p.y p.z 1
	
	printf("=> %i failed%c\n", tests_failed, tests_failed==0 ? '!' : '.');
	return tests_failed;
}