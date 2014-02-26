#include "../matrixn.h"
#include "../vector3d.h"

using namespace Geometry;
	
int tests_passed = 0;
#define TEST( condition ) if (!(condition)) { printf("failed: %s\n", #condition); } else {tests_passed++;} 

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
	v[0]=8; v[1]=16; v[2]=32;

	// position vector
	v[3]=1;
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

template< int N >
class T {
	public:
	T(const int data[N]) { std::copy(data, data+N, mD); }
	T(std::initializer_list<int> data) { 
		assert(data.size()==N); // "Exactly N elements required."
		std::copy(data.begin(), data.end(), mD); 
	}
	int mD[N];
};

T<2> t {1,2};

int main()
{	
	TestLayout();
	TestTranslate();
	
	MatrixN<int,4> test(uninitialised);
	VectorN<int,4> v(Vector3d<float>(1,2,3),1);

	// Geometry::MatrixN<int,4> matrix11({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
	// in OGL format
	// x.x x.y x.z 0
	// y.x y.y y.z 0
	// z.x z.y z.z 0
	// p.x p.y p.z 1
	
	
	return 0;
}