// Compile: clang++ -std=c++20 example.cpp -o example

#include <tuple>
#include "ppmpp.hpp"

int main()
{
	ppm::pixel color=std::make_tuple(0.0f,0.0f,0.0f);
	float r=0.0f;
	float g=0.0f;
	float b=0.0f;

	ppm::image img(1920,1080);

	for (int y=0;y<img.get_height();++y)
	{
		for (int x=0;x<img.get_width();++x)
		{
			r = float(x) / float(img.get_width());
			g = float(y) / float(img.get_height());
			b = 0.2;
			color=std::make_tuple(r,g,b);
			img.set_pixel(x,y,color);
		}
	}

	img.write("example.ppm");

	return 0;
}