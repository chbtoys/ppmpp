// Compile: clear && clang++ -std=c++20 test.cpp -o test

#include "ppmpp.hpp"
#include <tuple>

int main()
{
	ppm::Image image("test.ppm");
	image.write("test2_copy.ppm");
	std::cout << image.getWidth() << " " << image.getHeight() << std::endl;
	ppm::Pixel color=std::make_tuple(0.0f,0.0f,0.0f);
	ppm::Pixel color2=std::make_tuple(0.0f,1.0f,1.0f);
	ppm::Image image2(1920,1080);
	std::cout << image2.getWidth() << " " << image2.getHeight() << std::endl;
	// image2.setAllPixels(color2);
	for (int y=0;y<image2.getHeight();++y)
	{
		for (int x=0;x<image2.getWidth();++x)
		{
			image2.setPixel(x,y,color2);
		}
	}
	ppm::Coord coords=std::make_tuple(10,300,700,900);
	ppm::Point center=std::make_tuple(1920/2,1080/2);
	image2.drawLine(coords,color);
	image2.getAngledLine(coords,center,7,300);
	image2.drawLine(coords,color);

	ppm::Point pt0=std::make_tuple(340,90);
	ppm::Point pt1=std::make_tuple(480,2);
	ppm::Point pt2=std::make_tuple(620,90);
	ppm::Point pt3=std::make_tuple(0,0);

	image2.drawBezierQuadratic(pt0,pt1,pt2,50,color);
	pt0=std::make_tuple(1000,500);
	pt1=std::make_tuple(1500,800);
	pt2=std::make_tuple(1800,200);
	pt3=std::make_tuple(1200,1000);
	image2.drawBezierCubic(pt0,pt1,pt2,pt3,50,color);

	ppm::Point ptxy=std::make_tuple(500,500);
	ppm::Point ptwh=std::make_tuple(900,200);
	image2.drawRectangle(ptxy,ptwh,color);
	ptxy=std::make_tuple(100,100);
	ptwh=std::make_tuple(900,200);
	image2.drawFilledRectangle(ptxy,ptwh,color);
	ptxy=std::make_tuple(945,300);
	image2.drawCircle(ptxy,250,color);
	ptxy=std::make_tuple(300,600);
	image2.drawFilledCircle(ptxy,250,color);
	ppm::Pixel color4=std::make_tuple(0.8f,0.0f,0.8f);
	ppm::Pixel color5=std::make_tuple(1.0f,1.0f,1.0f);
	ppm::Pixel color3=ppm::blendColors(color4,color5,0.5f);
	ptxy=std::make_tuple(900,900);
	ptwh=std::make_tuple(100,100);
	image2.drawFilledRectangle(ptxy,ptwh,color3);
	ppm::Point pt=std::make_tuple(400,400);
	image2.drawWedge(pt,150,0,45,color3);
	pt=std::make_tuple(800,800);
	image2.drawFilledWedge(pt,150,0,45,color5);
	pt1=std::make_tuple(750,450);
	pt2=std::make_tuple(350,650);
	pt3=std::make_tuple(450,750);
	image2.drawTriangle(pt1,pt2,pt3,color5);
	pt1=std::make_tuple(750,450);
	pt2=std::make_tuple(950,750);
	pt3=std::make_tuple(1250,850);
	image2.drawFilledTriangle(pt1,pt2,pt3,color5);
	image2.write("test2_2.ppm");

	// Test of image2 functions in 2.0
	{ppm::Pixel px1=ppm::createPixelWithColor(0xff,0x6a,0x00);ppm::Pixel px2=ppm::createPixelWithColor(0xe3,0x19,0x37);image2.setAllPixels(ppm::blendColors(px1,px2,0.5));image2.write("test2_3.ppm");}
	{ppm::Pixel px=image.getAverageRgbOfImage();image2.setAllPixels(px);image2.write("test2_4.ppm");}
	{image2.read("test2_3.ppm");ppm::Pixel px1=image2.getAverageRgbOfImage();ppm::Pixel px2(0.0f,0.0f,0.0f);image2.setAllPixels(ppm::blendColors(px1,px2,0.3));ppm::Pixel px=ppm::createGrayPixel(0xcc);image2.drawRotatedRectangle(200,200,200,100,0.0,px);image2.drawRotatedRectangle(200,200,200,100,10.0,px);image2.drawRotatedRectangle(200,200,200,100,20.0,px);image2.drawRotatedRectangle(200,200,200,100,30.0,px);image2.drawRotatedRectangle(200,200,200,100,40.0,px);}
	{ppm::Pixel px=ppm::createGrayPixel(0x00);image2.drawFilledRotatedRectangle(600,200,200,100,0.0,px);px=ppm::createGrayPixel(0x11);image2.drawFilledRotatedRectangle(600,200,200,100,10.0,px);px=ppm::createGrayPixel(0x22);image2.drawFilledRotatedRectangle(600,200,200,100,20.0,px);px=ppm::createGrayPixel(0x33);image2.drawFilledRotatedRectangle(600,200,200,100,30.0,px);px=ppm::createGrayPixel(0x44);image2.drawFilledRotatedRectangle(600,200,200,100,40.0,px);}
	{ppm::Pixel px=ppm::createGrayPixel(0xcc);image2.drawRotatedEllipse(1000,200,200,100,0.0,px);image2.drawRotatedEllipse(1000,200,200,100,10.0,px);image2.drawRotatedEllipse(1000,200,200,100,20.0,px);image2.drawRotatedEllipse(1000,200,200,100,30.0,px);image2.drawRotatedEllipse(1000,200,200,100,40.0,px);}
	{ppm::Pixel px=ppm::createGrayPixel(0x00);image2.drawFilledRotatedEllipse(1400,200,200,100,0.0,px);px=ppm::createGrayPixel(0x11);image2.drawFilledRotatedEllipse(1400,200,200,100,10.0,px);px=ppm::createGrayPixel(0x22);image2.drawFilledRotatedEllipse(1400,200,200,100,20.0,px);px=ppm::createGrayPixel(0x33);image2.drawFilledRotatedEllipse(1400,200,200,100,30.0,px);px=ppm::createGrayPixel(0x44);image2.drawFilledRotatedEllipse(1400,200,200,100,40.0,px);}
	{std::vector<ppm::Point> vertices={{250,500},{350,500},{400,600},{300,700},{200,600}};ppm::Pixel px=ppm::createGrayPixel(0xcc);image2.drawRotatedPolygon(vertices,10.0,px);image2.drawRotatedPolygon(vertices,20.0,px);image2.drawRotatedPolygon(vertices,30.0,px);image2.drawRotatedPolygon(vertices,40.0,px);image2.drawRotatedPolygon(vertices,50.0,px);}
	{std::vector<ppm::Point> vertices={{650,500},{750,500},{800,600},{700,700},{600,600}};ppm::Pixel px=ppm::createGrayPixel(0x00);image2.drawFilledRotatedPolygon(vertices,10.0,px);px=ppm::createGrayPixel(0x11);image2.drawFilledRotatedPolygon(vertices,20.0,px);px=ppm::createGrayPixel(0x22);image2.drawFilledRotatedPolygon(vertices,30.0,px);px=ppm::createGrayPixel(0x33);image2.drawFilledRotatedPolygon(vertices,40.0,px);px=ppm::createGrayPixel(0x44);image2.drawFilledRotatedPolygon(vertices,50.0,px);image2.write("test2_5.ppm");}
	{image.read("test2_copy.ppm");image.convertToGrayscale();image.write("test2_6.ppm");}
	{image.read("test2_copy.ppm");image.write("blur2_0.ppm");image.applyGaussianBlur();image.write("blur2_1.ppm");image.applyGaussianBlur();image.write("blur2_2.ppm");image.applyGaussianBlur();image.write("blur2_3.ppm");image.applyGaussianBlur();image.write("blur2_4.ppm");for (int i=0;i<50;++i) {image.applyGaussianBlur();}image.write("blur2_5.ppm");}
	{std::vector<ppm::Pixel> colors={{255.0/255.0,205.0/255.0,210.0/255.0},{255.0/255.0,106.0/255.0,0.0f},{227.0/255.0,25.0/255.0,55.0/255.0},{153.0/255.0,31.0/255.0,61.0/255.0}};image.drawGradients(colors,20.0);image.write("test2_7.ppm");}

	// Test of new helper functions in 2.0
	if (ppm::getFloatColorElement(54) != 0.2117647059f) {std::cout<<"Error: if (ppm::getFloatColorElement(54) != 0.2117647059f)\n";}
	if (ppm::getfRedColorElement(color2) != 0.0f) {std::cout<<"Error: if (ppm::getfRedColorElement(color2) != 0.0f)\n";}
	if (ppm::getRedColorElement(color2) != 0) {std::cout<<"Error: if (ppm::getRedColorElement(color2) != 0)\n";}
	if (ppm::getfGreenColorElement(color2) != 1.0f) {std::cout<<"Error: if (ppm::getfGreenColorElement(color2) != 1.0f)\n";}
	if (ppm::getGreenColorElement(color2) != 255) {std::cout<<"Error: if (ppm::getGreenColorElement(color2) != 255)\n";}
	if (ppm::getfBlueColorElement(color2) != 1.0f) {std::cout<<"Error: if (ppm::getfBlueColorElement(color2) != 1.0f)\n";}
	if (ppm::getBlueColorElement(color2) != 255) {std::cout<<"Error: if (ppm::getBlueColorElement(color2) != 255)\n";}
	{ppm::Pixel px1=std::make_tuple(0.0f,1.0f,1.0f);ppm::Pixel px2=std::make_tuple(1.0f,1.0f,1.0f);ppm::setRedColorElement(px1,1.0f);if (px1 != px2) {std::cout<<"Error: if (ppm::setRedColorElement(px1,1.0f) != px2)\n";}}
	{ppm::Pixel px1=std::make_tuple(0.0f,1.0f,1.0f);ppm::Pixel px2=std::make_tuple(1.0f,1.0f,1.0f);ppm::setRedColorElement(px1,uint8_t(255));if (px1 != px2) {std::cout<<"Error: if (ppm::setRedColorElement(px1,uint8_t(255)) != px2)\n";}}
	{ppm::Pixel px1=std::make_tuple(0.0f,1.0f,1.0f);ppm::Pixel px2=std::make_tuple(0.0f,0.0f,1.0f);ppm::setGreenColorElement(px1,0.0f);if (px1 != px2) {std::cout<<"Error: if (ppm::setGreenColorElement(px1,0.0f) != px2)\n";}}
	{ppm::Pixel px1=std::make_tuple(0.0f,1.0f,1.0f);ppm::Pixel px2=std::make_tuple(0.0f,0.0f,1.0f);ppm::setGreenColorElement(px1,uint8_t(0));if (px1 != px2) {std::cout<<"Error: if (ppm::setGreenColorElement(px1,uint8_t(0)) != px2)\n";}}
	{ppm::Pixel px1=std::make_tuple(0.0f,1.0f,1.0f);ppm::Pixel px2=std::make_tuple(0.0f,1.0f,0.0f);ppm::setBlueColorElement(px1,0.0f);if (px1 != px2) {std::cout<<"Error: if (ppm::setBlueColorElement(px1,0.0f) != px2)\n";}}
	{ppm::Pixel px1=std::make_tuple(0.0f,1.0f,1.0f);ppm::Pixel px2=std::make_tuple(0.0f,1.0f,0.0f);ppm::setBlueColorElement(px1,uint8_t(0));if (px1 != px2) {std::cout<<"Error: if (ppm::setBlueColorElement(px1,uint8_t(0)) != px2)\n";}}
	{ppm::Pixel px1=ppm::createfPixelWithColor(0.5f,0.5f,0.5f);ppm::Pixel px2=std::make_tuple(0.5f,0.5f,0.5f);if (px1 != px2) {std::cout << "Error: if (createfPixelWithColor(0.5f,0.5f,0.5f) != px2)\n";}}
	{ppm::Pixel px1=ppm::createPixelWithColor(255,255,255);ppm::Pixel px2=std::make_tuple(1.0f,1.0f,1.0f);if (px1 != px2) {std::cout << "Error: if (ppm::createPixelWithColor(255,255,255) != px2)\n";}}
	{ppm::Pixel px1=ppm::createfGrayPixel(1.0f);ppm::Pixel px2=std::make_tuple(1.0f,1.0f,1.0f); if (px1 != px2) {std::cout << "Error: if (ppm::createfGrayPixel(1.0f) != px2)\n";}}
	{ppm::Pixel px1=ppm::createGrayPixel(255);ppm::Pixel px2=std::make_tuple(1.0f,1.0f,1.0f); if (px1 != px2) {std::cout << "Error: if (ppm::createGrayPixel(255) != px2)\n";}}
	{ppm::Coord co1=ppm::createCoord(10,10,20,20);ppm::Coord co2=std::make_tuple(10,10,20,20); if (co1 != co2) {std::cout << "Error: if (ppm::createCoord(10,10,20,20) != co2)\n";}}
	{ppm::Coord co=ppm::createCoord(10,15,20,25);int v2=10;int v1=ppm::getCoordX1(co); if (v1 != v2) {std::cout<<"Error: if (ppm::getCoordX1(co) != v2)\n";}}
	{ppm::Coord co=ppm::createCoord(10,15,20,25);int v2=20;int v1=ppm::getCoordX2(co); if (v1 != v2) {std::cout<<"Error: if (ppm::getCoordX2(co) != v2)\n";}}
	{ppm::Coord co=ppm::createCoord(10,15,20,25);int v2=15;int v1=ppm::getCoordY1(co); if (v1 != v2) {std::cout<<"Error: if (ppm::getCoordY1(co) != v2)\n";}}
	{ppm::Coord co=ppm::createCoord(10,15,20,25);int v2=25;int v1=ppm::getCoordY2(co); if (v1 != v2) {std::cout<<"Error: if (ppm::getCoordY2(co) != v2)\n";}}
	{ppm::Point pt1=ppm::getFirstPointFromCoord(ppm::createCoord(10,15,20,25));ppm::Point pt2=std::make_tuple(10,15); if (pt1 != pt2) {std::cout<<"Error: if (getFirstPointFromCoord(ppm::createCoord(10,15,20,25)) != pt2)\n";}}
	{ppm::Point pt1=ppm::getSecondPointFromCoord(ppm::createCoord(10,15,20,25));ppm::Point pt2=std::make_tuple(20,25); if (pt1 != pt2) {std::cout<<"Error: if (getSecondPointFromCoord(ppm::createCoord(10,15,20,25)) != pt2)\n";}}
	{ppm::Point pt1=std::make_tuple(10,15);ppm::Point pt2=std::make_tuple(20,25);ppm::Coord co1=ppm::createCoordFromPoints(pt1,pt2);ppm::Coord co2=ppm::createCoord(10,15,20,25); if (co1 != co2) {std::cout<<"Error: if (ppm::createCoordFromPoints(pt1,pt2) != co2)\n";}}
	{ppm::Point pt1=ppm::createPoint(10,15);ppm::Point pt2=std::make_tuple(10,15); if (pt1 != pt2) {std::cout<<"Error: if (createPoint(10,15) != pt2)\n";}}
	{ppm::Point pt=ppm::createPoint(10,15);int x1=ppm::getPointX1(pt);int ptx1=10; if (x1 != ptx1) {std::cout<<"Error: if (ppm::getPointX1(pt) != ptx1)\n";}}
	{ppm::Point pt=ppm::createPoint(10,15);int y1=ppm::getPointY1(pt);int pty1=15; if (y1 != pty1) {std::cout<<"Error: if (ppm::getPointY1(pt) != pty1)\n";}}
	{ppm::Pixel px1=ppm::createPixelWithColor(0xff,0x6a,0x00);ppm::Pixel px2=ppm::createPixelWithColor(0xe3,0x19,0x37);float alpha=0.5f;ppm::Pixel bc1=ppm::blendColors(px1,px2,alpha);ppm::Pixel bc2=ppm::createfPixelWithColor(ppm::getfRedColorElement(bc1),ppm::getfGreenColorElement(bc1),ppm::getfBlueColorElement(bc1)); if (bc1 != bc2) {std::cout<<"Error: if (ppm::blendColors(px1,px2,alpha) != bc2)\n";}}
	{ppm::Pixel px1(float(153)/255.0,float(31)/255.0,float(61)/255.0);ppm::Pixel px2(0.0f,0.0f,0.0f);double h=0.0f;double s=0.0f;double v=0.0f;ppm::getHSV(h,s,v,px1);double degree=35.0;h=(1.0/360.0)*degree;ppm::setHSV(h,s,v,px2); if (px1 == px2) {std::cout<<"Error: if (px1 == setHSV(h,s,v,px2))\n";}}
	{ppm::Pixel px1(float(153)/255.0,float(31)/255.0,float(61)/255.0);ppm::Pixel px2(0.0f,0.0f,0.0f);double h=0.0f;double s=0.0f;double v=0.0f;ppm::getHSV(h,s,v,px1);s-=0.2;ppm::setHSV(h,s,v,px2); if (px1 == px2) {std::cout<<"Error: if (px1 == setHSV(h,s,v,px2))\n";}}
	{ppm::Pixel px1(float(153)/255.0,float(31)/255.0,float(61)/255.0);ppm::Pixel px2(0.0f,0.0f,0.0f);double h=0.0f;double s=0.0f;double v=0.0f;ppm::getHSV(h,s,v,px1);v-=0.2;ppm::setHSV(h,s,v,px2); if (px1 == px2) {std::cout<<"Error: if (px1 == setHSV(h,s,v,px2))\n";}}

	// Operators
	ppm::Image img=image2;
	image=std::move(img);
	image.read("blur2_4.ppm");
	image2.read("blur2_4.ppm");
	if (image != image2) {std::cout<<"Error: if (image != image2)\n";}
	image.read("blur2_0.ppm");
	if (image == image2) {std::cout<<"Error: if (image == image2)\n";}

	return 0;
}
