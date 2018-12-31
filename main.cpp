

#include "PixelTerm.h"

int main()
{
	PixelTerm::Init();

	Image image(400, 400);

	for (int i = 0; i < 400; i++)
	{
		PixelTerm::ForceClear();
		for (int y = 0; y < 400; y++)
		{
			for (int x = 0; x < 400; x++)
			{
				PixelTerm::DrawPixel(x,y,{0,(x>0+i&&x<20+i)*255});
				//image.Pixel(x,y,{0,(x>0+i&&x<20+i)*255});
			}
		}
		//PixelTerm::DrawRectangle(50+1*i, 50, 100, 200, {0xff,0,0});
		//PixelTerm::DrawImage(0,0,&image);
		PixelTerm::Draw();
		//printf("Sleep\n");
		usleep(0.0001*1000*1000);
	}


	PixelTerm::Close();

	printf("Done\n");
	
	return 0;
}
