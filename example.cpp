// Compile: clear && clang++ -std=c++20 example.cpp -o example
#include "ppmpp.hpp"

int main()
{
	ppm::Image img(1920,1080);
	std::vector<ppm::Pixel> colors={
		{255.0/255.0,205.0/255.0,210.0/255.0},
		{255.0/255.0,106.0/255.0,0.0},
		{227.0/255.0,25.0/255.0,55.0/255.0},
		{153.0/255.0,31.0/255.0,61.0/255.0}};

	img.drawGradients(colors,45.0);
	img.write("example2_1.ppm");
	double h=0.0f;double s=0.0f;double v=0.0f;
	for (auto& color : colors) {
		ppm::getHSV(h,s,v,color);
		v-=0.4f;
		ppm::setHSV(h,s,v,color);
	}
	img.drawGradients(colors,45.0);
	img.write("example2_2.ppm");
	img.convertToGrayscale();
	img.write("example2_3.ppm");

	return 0;
}
