# ppmpp.hpp - C++ header-only draw/read/write 2D graphics using the standard library only.

Draw/read/write PPM P6 images (Portable Pixel Map) without any hassle.

Created by Håkan Blomqvist
Part of the book: https://leanpub.com/2dcomputergraphicsinmoderncandstandardlibrary/

More information TBD...

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

NOTE: Uses **-std=c++20** for std::lerp when making bezier quadratic and cubic. Everything else works great with **-std=c++17**.

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
