

#include <iostream>
#include "PixelTerm.h"
#include "Projection.h"

int main()
{
	Projection renderer;

	for (int i = 0; i < 1000; i++)
	{
		Camera cam;
		cam.Position = {12*sin(TAU*i/1000),10,12*cos(TAU*i/1000)};
		cam.Rotation = {0,-TAU*i/1000,0};

		Line line1{{10,0,0},{0,0,0}};
		Line line2{{0,10,0},{0,0,0}};
		Line line3{{0,0,10},{0,0,0}};
		//double z2 = 2.5;//-(double)i/50.0;
		//Line line2{{0.5,0.5,z2},{-0.5,-0.5,z2}};
		renderer.QueueLine(line1);
		renderer.QueueLine(line2);
		renderer.QueueLine(line3);

		renderer.Render(cam);

		usleep(0.001*1000*1000);
	}
	
	return 0;
}
