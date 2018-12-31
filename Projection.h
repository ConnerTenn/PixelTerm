
#include <initializer_list>
#include <stdexcept>
#include <string>

template<class T, int Width, int Height>  
struct Matrix
{
	T Array[Width][Height];
	Matrix() {}
	Matrix(std::initializer_list<std::initializer_list<T>> arr)
	{
		for (int y = 0; y < arr.size(); y++)
		{
			for (int x = 0; x < (arr.begin()[y]).size(); x++)
			{
				Array[x][y] = (arr.begin()[y]).begin()[x];
			}
		}
	}

	T *operator[](int i)
	{
		return Array[i];
	}

	Matrix<T, Width, Height>operator +(Matrix<T,Width,Height> &other)
	{
		Matrix<T, Width, Height> out;
		for (int y = 0; y < Height; y++)
		{
			for (int x = 0; x < Width; x++)
			{
				out[x][y] = Array[x][y] + other[x][y];
			}
		}
		return out;	
	}

	template<int Ow, int Oh>
	Matrix<T, Ow, Height>operator *(Matrix<T,Ow,Oh> &other)
	{
		if (Width != Oh) { throw std::length_error("Matrix Mismatch"); }
		Matrix<T, Ow, Height> out;

		for (int y = 0; y < Height; y++)
		{
			for (int x = 0; x < Ow; x++)
			{
				out[x][y] = 0;
				for (int i = 0; i < Width; i++)
				{
					out[x][y] += Array[i][y] * other[x][i];
				}
			}
		}

		return out;
	}

	std::string String(bool nl = true)
	{
		std::string out;
		if (!nl) { out += "[ "; }
		for (int y = 0; y < Height; y++)
		{
			out += "[ ";
			for (int x = 0; x < Width; x++)
			{
				out += std::to_string(Array[x][y]) + " ";
			}
			out += (nl?"]\n":"] ");
		}
		if (!nl) { out += "]"; }
		return out;
	}
};

class Projection
{
	double N=1, F=1000, T=3, R=3;
	
	Matrix<double,1,1> GetProjMat()
	{
		return {{ N/R,   0,     0,             0             },
				{ 0,     N/T,   0,             0             },
				{ 0,     0,    -(F+N)/(F-N),  -(2*F*N)/(F-N) },
				{ 0,     0,    -1,             0             }
				};
	}
};

