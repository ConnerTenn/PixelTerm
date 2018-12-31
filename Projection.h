
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <vector>
#include <math.h>

#define PI M_PIl
#define TAU (PI*2.0L)

template<class T, int Width, int Height>  
struct Matrix
{
	T Array[Width][Height];
	Matrix() {}
	Matrix(std::initializer_list<std::initializer_list<T>> arr)
	{
		if (arr.size() != Height || (arr.begin()[0]).size() != Width) { throw std::length_error("Invalid Init Mat Size"); }
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

	const T *operator[](int i) const
	{
		return Array[i];
	}

	Matrix<T, Width, Height> operator+(const Matrix<T,Width,Height> &other) const
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
	Matrix<T, Ow, Height> operator*(const Matrix<T,Ow,Oh> &other) const
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

struct Vec3
{
	double X;
	double Y;
	double Z;

	Matrix<double,1,4> ToMat()
	{
		return Matrix<double,1,4>({{X},{Y},{Z},{1}});
	}
	static Vec3 FromMat(const Matrix<double,1,4> &mat)
	{
		Vec3 vec;
		double w = abs(mat[0][3]);
		if (w==0) { w=0.0000000000001; }
		vec.X = mat[0][0]/w;
		vec.Y = mat[0][1]/w;
		vec.Z = mat[0][2]/w;
		return vec;
	}
	Vec3 Transform(const Matrix<double,4,4> &mat)
	{
		return FromMat(mat * ToMat());
	}

	std::string String()
	{
		return "["+std::to_string(X)+" "+std::to_string(Y)+" "+std::to_string(Z)+"]";
	}
};
struct Line
{
	Vec3 Point1;
	Vec3 Point2;

	Line Transform(const Matrix<double,4,4> &mat)
	{
		Line line;
		line.Point1 = Point1.Transform(mat);
		line.Point2 = Point2.Transform(mat);
		return line;
	}

	std::string String()
	{
		return Point1.String() + " " + Point2.String();
	}
};

struct Camera
{
	Vec3 Position;
	Vec3 Rotation;
};

class Projection
{
private:
	double N=1, F=1000, T=3, R=3;

	std::vector<Line> LineList;

public:
	Projection()
	{
		PixelTerm::Init();
	}
	~Projection()
	{
		PixelTerm::Close();
	}

	Matrix<double,4,4> GetProjMat()
	{
		return {{ N/R,   0,     0,             0             },
				{ 0,     N/T,   0,             0             },
				{ 0,     0,    -(F+N)/(F-N),  -(2*F*N)/(F-N) },
				{ 0,     0,    -1,             0             }};
	}

	Matrix<double,4,4> GetTransMat(double x, double y, double z)
	{
		return {{  1,   0,   0,   x },
				{  0,   1,   0,   y },
				{  0,   0,   1,   z },
				{  0,   0,   0,   1 }};
	}
			
	Matrix<double,4,4> GetScaleMat(double x, double y, double z)
	{
		return {{  x,   0,   0,   0 },
				{  0,   y,   0,   0 },
				{  0,   0,   z,   0 },
				{  0,   0,   0,   1 }};
	}

	Matrix<double,4,4> GetRotMatX(double ang)
	{
		return {{  1,   0,          0,          0 },
				{  0,   cos(ang),  -sin(ang),   0 },
				{  0,   sin(ang),   cos(ang),   0 },
				{  0,   0,           0      ,   1 }};
	}

	Matrix<double,4,4> GetRotMatY(double ang)
	{
		return {{  cos(ang),   0,   sin(ang),   0 },
				{  0,          1,   0,          0 },
				{ -sin(ang),   0,   cos(ang),   0 },
				{  0,          0,   0,          1 }};
	}

	Matrix<double,4,4> GetRotnMatZ(double ang)
	{
		return {{  cos(ang),  -sin(ang),   0,   0 },
				{  sin(ang),   cos(ang),   0,   0 },
				{  0,          0,          1,   0 },
				{  0,          0,          0,   1 }};
	}

private:
	void DrawLine(Line &line)
	{
		PixelTerm::DrawLine(line.Point1.X, line.Point1.Y, line.Point2.X, line.Point2.Y, {0xff,0xff,0xff});
	}

public:

	void QueueLine(Line line)
	{
		LineList.push_back(line);
	}

	void Render(const Camera &cam)
	{
		PixelTerm::ForceClear();


		Matrix<double,4,4> projmat;
		projmat = GetScaleMat(400, 400, 1) * GetTransMat(0.5, 0.5, 0) * GetScaleMat(1,-1,1) * GetProjMat() * 
					GetRotMatX(cam.Rotation.X) *GetRotMatY(cam.Rotation.Y) * GetTransMat(-cam.Position.X,-cam.Position.Y,-cam.Position.Z);

		//std::cout << projmat.String() << "\n";

		for (int i = 0; i < LineList.size(); i++)
		{
			Line line = LineList[i];
			line = line.Transform(projmat);
			//std::cout << line.String() << "\n";
			DrawLine(line);
		}
		LineList.clear();

		//std::cout << "\n";

		PixelTerm::Draw();
	}
};

