

#include <iostream>
#include "PixelTerm.h"
#include "Projection.h"

int main()
{
	set_conio_terminal_mode();
	Projection renderer;

	long long t = 0;
	while(!kbhit())
	{
		Camera cam;
		double y = 6.0*sin(TAU*t/5000);
		cam.Position = {12*sin(TAU*t/1000),y,12*cos(TAU*t/1000)};
		cam.Rotation = {(y==0?0:(y<0?-1:1)*(PI/2-atan(12/abs(y)))),-TAU*t/1000,0};

		Matrix<double,4,4> transMat = renderer.GetRotMatXYZ(TAU*cos(-t*TAU/7000), TAU*sin(-t*TAU/4000), TAU*cos(-t*TAU/5000));

		Line line1{{10,0,0},{0,0,0}};
		Line line2{{0,10,0},{0,0,0}};
		Line line3{{0,0,10},{0,0,0}};
		//double z2 = 2.5;//-(double)i/50.0;
		//Line line2{{0.5,0.5,z2},{-0.5,-0.5,z2}};
		renderer.QueueLine(line1);
		renderer.QueueLine(line2);
		renderer.QueueLine(line3);

		std::vector<Line> lines;
		const double res1 = 26;
		const double res2 = 10;
		for (int c1 = 0; c1 < res1; c1++)
		{
			double a1 = c1*TAU/res1;
			double a1n = a1+TAU/res1;
			double x1 = 5*cos(a1); //start at +x
			double z1 = 5*sin(a1);
			double x1n = 5*cos(a1n);
			double z1n = 5*sin(a1n);
			/*lines.push_back(Line{
				{x1,0, z1},
				{x1n,0, z1n}});*/
			
			for (int c2 = 0; c2 < res2; c2++)
			{
				double a2 = c2*TAU/res2;
				double a2n = a2+TAU/res2;

				double x2 = x1+cos(a1)*cos(a2);
				double z2 = z1+sin(a1)*cos(a2);
				double y2 = sin(a2);

				double x2n = x1+cos(a1)*cos(a2n);
				double z2n = z1+sin(a1)*cos(a2n);
				double y2n = sin(a2n);

				double x3n = x1n+cos(a1n)*cos(a2);
				double z3n = z1n+sin(a1n)*cos(a2);
				double y3n = sin(a2);

				double x3nn = x1n+cos(a1n)*cos(a2n);
				double z3nn = z1n+sin(a1n)*cos(a2n);
				double y3nn = sin(a2n);

				lines.push_back(Line{{x2,y2,z2}, {x2n,y2n,z2n}}.Transform(transMat));
				lines.push_back(Line{{x2,y2,z2}, {x3n,y3n,z3n}}.Transform(transMat));
				lines.push_back(Line{{x2,y2,z2}, {x3nn,y3nn,z3nn}}.Transform(transMat));
			}
		}
		renderer.QueueLines(lines);

		renderer.Render(cam);

		usleep(0.001*1000*1000);

		t++;
	}
	getch();

	reset_terminal_mode();
	
	return 0;
}
