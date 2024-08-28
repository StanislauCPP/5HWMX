/*Computing determination of matrix*/

#include <iostream>

#include "matrix.hpp"

int main()
{
	int mSize = 0;
	std::cin >> mSize;

	math::Matrix<int> m(mSize, mSize);
	for(int i = 0; i < mSize; ++i)
		for(int j = 0; j < mSize; ++j)
			std::cin >> m[i][j];

	std::cout << m.determinant() << std::endl;

	return 0;
}