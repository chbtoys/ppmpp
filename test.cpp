// Compile: clang++ -std=c++17 test.cpp -o test
// Compile: clang++ -std=c++20 test.cpp -o test

// Tests of ppmpp.hpp

#include <tuple>
#include "ppmpp.hpp"

int main()
{
	ppm::image image("test.ppm");
	image.write("test_copy.ppm");
	std::cout << image.get_width() << " " << image.get_height() << std::endl;
	ppm::pixel color=std::make_tuple(0.0f,0.0f,0.0f);
	ppm::pixel color2=std::make_tuple(0.0f,1.0f,1.0f);
	ppm::image image2(1920,1080);
	std::cout << image2.get_width() << " " << image2.get_height() << std::endl;
	// image2.set_all_pixels(color2);
	for (int y=0;y<image2.get_height();++y)
	{
		for (int x=0;x<image2.get_width();++x)
		{
			image2.set_pixel(x,y,color2);
		}
	}
	ppm::coord coords=std::make_tuple(10,300,700,900);
	ppm::point center=std::make_tuple(1920/2,1080/2);
	image2.draw_line(coords,color);
	image2.get_angled_line(coords,center,7,300);
	image2.draw_line(coords,color);
	// Lerp requires C++20.
	ppm::point pt0=std::make_tuple(340,90);
	ppm::point pt1=std::make_tuple(480,2);
	ppm::point pt2=std::make_tuple(620,90);
	ppm::point pt3=std::make_tuple(0,0);
#if __cplusplus >= 202002L
	image2.draw_bezier_quadratic(pt0,pt1,pt2,50,color);
	pt0=std::make_tuple(200,270);
	pt1=std::make_tuple(293,182);
	pt2=std::make_tuple(386,357);
	pt3=std::make_tuple(480,270);
	image2.draw_bezier_cubic(pt0,pt1,pt2,pt3,50,color);
#endif
	ppm::point ptxy=std::make_tuple(500,500);
	ppm::point ptwh=std::make_tuple(900,200);
	image2.draw_rectangle(ptxy,ptwh,color);
	ptxy=std::make_tuple(100,100);
	ptwh=std::make_tuple(900,200);
	image2.draw_filled_rectangle(ptxy,ptwh,color);
	ptxy=std::make_tuple(945,300);
	image2.draw_circle(ptxy,250,color);
	ptxy=std::make_tuple(300,600);
	image2.draw_filled_circle(ptxy,250,color);
	ppm::pixel color4=std::make_tuple(0.8f,0.0f,0.8f);
	ppm::pixel color5=std::make_tuple(1.0f,1.0f,1.0f);
	ppm::pixel color3=image2.blend_colors(color4,color5,0.5f);
	ptxy=std::make_tuple(900,900);
	ptwh=std::make_tuple(100,100);
	image2.draw_filled_rectangle(ptxy,ptwh,color3);
	ppm::point pt=std::make_tuple(400,400);
	image2.draw_wedge(pt,150,0,45,color3);
	pt=std::make_tuple(800,800);
	image2.draw_filled_wedge(pt,150,0,45,color5);
	pt1=std::make_tuple(750,450);
	pt2=std::make_tuple(350,650);
	pt3=std::make_tuple(450,750);
	image2.draw_triangle(pt1,pt2,pt3,color5);
	pt1=std::make_tuple(750,450);
	pt2=std::make_tuple(950,750);
	pt3=std::make_tuple(1250,850);
	image2.draw_filled_triangle(pt1,pt2,pt3,color5);
	image2.write("test2.ppm");

	return 0;
}