#include "../matrixn.h"

int tests_passed = 0;
#define TEST( condition ) if (!condition) { printf("failed: %s\n", #condition); } else {tests_passed++;} 

void Flush(const char* name)
{
	printf("%s: %i passed\n", name, tests_passed);
	tests_passed = 0;
}

void TestLayout()
{
	int matrixA[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	int matrixB[4][4] = { { 0, 1, 2, 3 }, { 4, 5, 6, 7 }, { 8, 9, 10, 11 }, { 12, 13, 14, 15 } };
	Geometry::MatrixN<int,4> matrixC(matrixA);
	for (int n=0;n!=4;n++)
		for (int m=0;m!=4;m++)
			TEST( matrixB[n][m]==matrixC[n][m] );
	Flush("TestLayout");
}

int main()
{
	using namespace Geometry;
	
	TestLayout();
	
	MatrixN<int,4> test(uninitialised);
	
	int dx=10;
	int dy=0;	
	int dz=0;
	
	// in OGL format
	// x.x x.y x.z 0
	// y.x y.y y.z 0
	// z.x z.y z.z 0
	// p.x p.y p.z 1

	const int data[4*4] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		dx, dy, dz, 1
	};
	
	MatrixN<int,4> t(
		data
	);
	
	VectorN<int,4> v(uninitialised);
	v[2]=v[1]=v[0]=10;
	v[3]=1;
	
	VectorN<int,4> v2 = t * v;
	
	printf("%i,%i,%i,%i\n", v2[0], v2[1], v2[2], v2[3]);
	
	return 0;
}