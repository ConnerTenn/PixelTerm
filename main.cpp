

#include <iostream>
#include "PixelTerm.h"
#include "Projection.h"

int main()
{
	PixelTerm::Init();

	Matrix<int,2,2> mat1 = {{1,2},{3,4}};
	Matrix<int,2,2> mat2 = {{5,6},{7,8}};

	Matrix<int,2,2> mat3 = mat1 * mat2;

	std::cout << mat1.String() <<"\n"<< mat2.String() <<"\n"<< mat3.String() <<"\n";

	for (int i = 0; i < 400; i++)
	{
		PixelTerm::ForceClear();


		PixelTerm::Draw();
		//printf("Sleep\n");
		usleep(0.0001*1000*1000);
	}


	PixelTerm::Close();

	printf("Done\n");
	
	return 0;
}
