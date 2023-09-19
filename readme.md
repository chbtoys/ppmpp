# ppmpp.hpp - C++ header-only draw/read/write 2D graphics using the standard library only.

Draw/read/write PPM P6 images (Portable Pixel Map) without any hassle.

Created by Håkan Blomqvist
Part of the book: https://leanpub.com/2dcomputergraphicsinmoderncandstandardlibrary/

![ppmpp](https://github.com/chbtoys/ppmpp/blob/main/images/ppmpp.png)

## Example
```
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
```


## Clone ppmpp and build test.cpp

```
git clone https://github.com/chbtoys/ppmpp.git
cd ppmpp
clang++ -std=c++20 test.cpp -o test
./test
```

## Overview

### Types
using **Pixel** = std::tuple<double, double, double>;

using **Coord** = std::tuple<int, int, int, int>;

using **Point** = std::tuple<int, int>;

### Helper functions
constexpr float **getFloatColorElement**(uint8_t element)

constexpr float **getfRedColorElement**(Pixel px)

constexpr uint8_t **getRedColorElement**(Pixel px) 

constexpr float **getfGreenColorElement**(Pixel px) 

constexpr uint8_t **getGreenColorElement**(Pixel px) 

constexpr float **getfBlueColorElement**(Pixel px) 

constexpr uint8_t **getBlueColorElement**(Pixel px)

constexpr Pixel **createfPixelWithColor**(float r, float g, float b)

constexpr Pixel **createPixelWithColor**(uint8_t r, uint8_t g, uint8_t b) 

constexpr Pixel **createfGrayPixel**(float v) 

constexpr Pixel **createGrayPixel**(uint8_t v)

constexpr Coord **createCoord**(int x1, int y1, int x2, int y2) 

constexpr int **getCoordX1**(Coord co) 

constexpr int **getCoordX2**(Coord co) 

constexpr int **getCoordY1**(Coord co) 

constexpr int **getCoordY2**(Coord co) 

constexpr Point **getFirstPointFromCoord**(Coord co) 

constexpr Point **getSecondPointFromCoord**(Coord co) 

constexpr Coord **createCoordFromPoints**(Point pt1, point pt2)

constexpr Point **createPoint**(int x1, int y1) 

constexpr int **getPointX1**(Point pt) 

constexpr int **getPointY1**(Point pt)

void **setRedColorElement**(Pixel& px, float r) 

void **setRedColorElement**(Pixel& px, uint8_t r) 

void **setGreenColorElement**(Pixel& px, float g) 

void **setGreenColorElement**(Pixel& px, uint8_t g) 

void **setBlueColorElement**(Pixel& px, float b) 

void **setBlueColorElement**(Pixel& px, uint8_t b)

### Helper functions for colors
Pixel **blendColors**(Pixel &colorbackground, Pixel &colorforeground, float alpha)

void **getHSV**(double& h, double& s, double& v, const Pixel& px)

void **setHSV**(double h,double s,double v,Pixel& px)

### Default constructor
**Image**()

### Parameterized constructor
**Image**(int width, int height)

### Parameterized constructor
explicit **Image**(std::string const& filename)

### Copy constructor
**Image**(const Image& other)

### Copy assignment operator
Image& **operator=**(const Image& other)

### Move constructor
**Image**(Image&& other) noexcept

### Move assignment operator
Image& **operator=**(Image&& other) noexcept

### Equality comparison operator
friend bool **operator==**(const Image& lhs, const Image& rhs)

### Inequality comparison operator
friend bool **operator!=**(const Image& lhs, const Image& rhs)

### Public methods
void **resize**(int width, int height) _// Resizes the image to the specified width and height._

void **setWidth**(int width) _// Sets the width of the image._

void **setHeight**(int height) _// Sets the height of the image._

int **getWidth**() _// Retrieves the current width of the image._

int **getHeight**() _// Retrieves the current height of the image._

void **setPixel**(int xCoord, int yCoord, const Pixel& newPixel) _// Sets the pixel at the given coordinates to the specified color._

const Pixel& **getPixel**(int xCoord, int yCoord) _// Retrieves the pixel color at the given coordinates._

void **setAllPixels**(const Pixel& newPixel) _// Sets all pixels in the image to the specified color._

void **drawLine**(Coord& startCoords, const Pixel& lineColor) _// Draws a line between specified coordinates with the given color._

void **getAngledLine**(Coord& lineCoords, const Point& center, double degrees, int length) _// Draws an angled line based on the center point, angle, and length._

void **drawBezierQuadratic**(const Point& pt0, const Point& pt1, const Point& pt2, int split, const Pixel& bezierColor) _// Draws a quadratic Bezier curve based on three control points._

void **drawBezierCubic**(const Point& pt0, const Point& pt1, const Point& pt2, const Point& pt3, int split, const Pixel& bezierColor) _// Draws a cubic Bezier curve based on four control points._

void **drawRectangle**(const Point& xy, const Point& wh, const Pixel& rectangleColor) _// Draws a rectangle defined by top-left corner and dimensions._

void **drawFilledRectangle**(const Point& xy, const Point& wh, const Pixel& rectangleColor) _// Draws a filled rectangle defined by top-left corner and dimensions._

void **drawCircle**(const Point& xy, int radius, const Pixel& circleColor) _// Draws a circle defined by center point and radius._

void **drawFilledCircle**(const Point& xy, int radius, const Pixel& circleColor) _// Draws a filled circle defined by center point and radius._

void **drawWedge**(const Point& center, int radius, int startAngle, int endAngle, const Pixel& wedgeColor) _// Draws a wedge within a circle defined by angles._

void **drawFilledWedge**(const Point& center, int radius, int startAngle, int endAngle, const Pixel& wedgeColor) _// Draws a filled wedge within a circle defined by angles._

void **drawTriangle**(const Point& pt1, const Point& pt2, const Point& pt3, const Pixel& triangleColor) _// Draws a triangle defined by three vertices._

void **drawFilledTriangle**(const Point& pt1, const Point& pt2, const Point& pt3, const Pixel& fillColor) _// Draws a filled triangle defined by three vertices._

void **drawRotatedRectangle**(int x, int y, int w, int h, double angle, const Pixel& px) _// Draws a rotated rectangle at the given coordinates._

void **drawFilledRotatedRectangle**(int x, int y, int w, int h, double angle, const Pixel& px) _// Draws a filled rotated rectangle at the given coordinates._

void **drawRotatedEllipse**(int x, int y, int w, int h, double angle, const Pixel& px) _// Draws a rotated ellipse at the given coordinates._

void **drawFilledRotatedEllipse**(int x, int y, int w, int h, double angle, const Pixel& px) _// Draws a filled rotated ellipse at the given coordinates._

void **drawRotatedPolygon**(const std::vector<Point>& vertices, double angle, const Pixel& px) _// Draws a polygon with rotation at the given coordinates._

void **drawFilledRotatedPolygon**(const std::vector<Point>& vertices, double angle, const Pixel& px) _// Draws a filled polygon with rotation at the given coordinates._

Pixel **getAverageRgbOfImage**() _// Returns the average RGB value of the entire image._

void **convertToGrayscale**() _// Converts the image to grayscale._

void **applyGaussianBlur**() _// Applies Gaussian blur to the image._

void **drawGradients**(const std::vector<Pixel>& colors, double angle_degree) _// Draws gradient colors at a specified angle._

void **read**(const std::string& filename) _// Reads PPM P6 image from a file._

void **write**(const std::string& filename) _// Writes PPM P6 image to a file._


## License

MIT No Attribution

ppmpp.hpp - A header-only class to draw/read/write 2D graphics using only the standard library.
Copyright (c) 2022-2023 Håkan Blomqvist

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
