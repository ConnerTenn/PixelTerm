

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
		renderer.QueueLine(Line{{10,0,0},{0,0,0}});
		renderer.QueueLine(Line{{0,10,0},{0,0,0}});
		renderer.QueueLine(Line{{0,0,10},{0,0,0}});

		renderer.QueueLine(		Line{ {-10,-10, 10}, { 10,-10, 10} }	);
		renderer.QueueLine(		Line{ {-10, 10, 10}, { 10, 10, 10} }	);
		renderer.QueueLine(		Line{ {-10, 10, 10}, {-10,-10, 10} }	);
		renderer.QueueLine(		Line{ { 10, 10, 10}, { 10,-10, 10} }	);

		renderer.QueueLine(		Line{ {-10,-10,-10}, { 10,-10,-10} }	);
		renderer.QueueLine(		Line{ {-10, 10,-10}, { 10, 10,-10} }	);
		renderer.QueueLine(		Line{ {-10, 10,-10}, {-10,-10,-10} }	);
		renderer.QueueLine(		Line{ { 10, 10,-10}, { 10,-10,-10} }	);

		renderer.QueueLine(		Line{ {-10,-10, 10}, {-10,-10,-10} }	);
		renderer.QueueLine(		Line{ { 10,-10, 10}, { 10,-10,-10} }	);
		renderer.QueueLine(		Line{ {-10, 10, 10}, {-10, 10,-10} }	);
		renderer.QueueLine(		Line{ { 10, 10, 10}, { 10, 10,-10} }	);

		renderer.Render(t);

		usleep(0.001*1000*1000);

		t++;
	}

	getch();

	reset_terminal_mode();
	
	return 0;
}
