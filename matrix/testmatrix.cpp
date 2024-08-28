#include <iostream>
#include <string>
#include <cmath>
#include "matrix.hpp"

namespace
{
	using matrix_t = math::Matrix<int>;

	enum unifunc : int
	{
		CTOR,
		CTORCOPY,
		CTORMOVE,
		OPASSIGN,
		OPASSIGNMOVE
	};
}

void ctorOpTest(int argc, char **argv)
{

	if(argc < 2)
	{
		std::cout << "set unit test number" << std::endl;
		return;
	}

	unifunc uf = static_cast<unifunc>(std::stoi(argv[1]));

	switch (uf)
	{
	case CTOR:
		{matrix_t m;}
		break;

	case CTORCOPY:
		{
			matrix_t m(5, 5);
			matrix_t mc(m);
		}
		break;

	case CTORMOVE:
		{
			matrix_t mc(7, 7);
			matrix_t m(std::move(mc));
		}
		break;

	case OPASSIGN:
		{
			matrix_t m(5, 5);
			matrix_t mc;
			mc = m;
		}
		break;

	case OPASSIGNMOVE:
		{
			matrix_t m;
			matrix_t mc(7, 7);
			m = std::move(mc);
		}
		break;
	
	default:
		std::cout << "Unknown function" << std::endl;
		break;
	}
}

template <typename T>
void determinantTest()
{
	int mSize = 0;
	std::cin >> mSize;

	math::Matrix<T> m(mSize, mSize);

	for(int i = 0; i < mSize; ++i)
		for(int j = 0; j < mSize; ++j)
			std::cin >> m[i][j];

	std::cout << m.determinant() << std::endl;
}

struct struct_t
{};

int main(int argc, char **argv)
{
#ifdef UNITTEST
	ctorOpTest(argc, argv);
#endif

#ifdef DET

	std::string str;
	std::cin >> str;

	if(str == "int")
		determinantTest<int>();
	else if(str == "double")
		determinantTest<double>();
	else
		std::cout << "Bad type for determinant test" << std::endl;
#endif


	return 0;
}