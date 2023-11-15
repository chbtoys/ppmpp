// MIT No Attribution
// 
// ppmpp.hpp - A header-only class to draw/read/write 2D graphics using only the standard library.
// Version (15:th of November 2023).
// Copyright (c) 2022-2023 Håkan Blomqvist
// 
// For more information:
// https://leanpub.com/2dcomputergraphicsinmoderncandstandardlibrary
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <numeric>

namespace ppm
{
	using Pixel = std::tuple<double, double, double>;
	using Coord = std::tuple<int, int, int, int>;
	using Point = std::tuple<int, int>;

	// Helper functions
	constexpr float getFloatColorElement(uint8_t element) { return static_cast<float>(element) / 255.0f; }
	constexpr float getfRedColorElement(Pixel px) { return std::get<0>(px); }
	constexpr uint8_t getRedColorElement(Pixel px) { return static_cast<uint8_t>(std::get<0>(px) * 255); }
	constexpr float getfGreenColorElement(Pixel px) { return std::get<1>(px); }
	constexpr uint8_t getGreenColorElement(Pixel px) { return static_cast<uint8_t>(std::get<1>(px) * 255); }
	constexpr float getfBlueColorElement(Pixel px) { return std::get<2>(px); }
	constexpr uint8_t getBlueColorElement(Pixel px) { return static_cast<uint8_t>(std::get<2>(px) * 255); }
	constexpr Pixel createfPixelWithColor(float r, float g, float b) { return std::make_tuple(r,g,b); }
	constexpr Pixel createPixelWithColor(uint8_t r, uint8_t g, uint8_t b) { return std::make_tuple(getFloatColorElement(r), getFloatColorElement(g), getFloatColorElement(b)); }
	constexpr Pixel createfGrayPixel(float v) { return createfPixelWithColor(v,v,v); }
	constexpr Pixel createGrayPixel(uint8_t v) { return createPixelWithColor(v,v,v); }
	constexpr Coord createCoord(int x1, int y1, int x2, int y2) { return std::make_tuple(x1,y1,x2,y2); }
	constexpr int getCoordX1(Coord co) { return std::get<0>(co); }
	constexpr int getCoordX2(Coord co) { return std::get<2>(co); }
	constexpr int getCoordY1(Coord co) { return std::get<1>(co); }
	constexpr int getCoordY2(Coord co) { return std::get<3>(co); }
	constexpr Point getFirstPointFromCoord(Coord co) { return std::make_tuple(getCoordX1(co), getCoordY1(co)); }
	constexpr Point getSecondPointFromCoord(Coord co) { return std::make_tuple(getCoordX2(co), getCoordY2(co)); }
	constexpr Coord createCoordFromPoints(Point pt1, Point pt2) { return std::make_tuple(std::get<0>(pt1), std::get<1>(pt1), std::get<0>(pt2), std::get<1>(pt2)); }
	constexpr Point createPoint(int x1, int y1) { return std::make_tuple(x1,y1); }
	constexpr int getPointX1(Point pt) { return std::get<0>(pt); }
	constexpr int getPointY1(Point pt) { return std::get<1>(pt); }
	
	void setRedColorElement(Pixel& px, float r) {px=std::make_tuple(r,getfGreenColorElement(px),getfBlueColorElement(px));}
	void setRedColorElement(Pixel& px, uint8_t r) {px=std::make_tuple(getFloatColorElement(r),getfGreenColorElement(px),getfBlueColorElement(px));}
	void setGreenColorElement(Pixel& px, float g) {px=std::make_tuple(getfRedColorElement(px),g,getfBlueColorElement(px));}
	void setGreenColorElement(Pixel& px, uint8_t g) {px=std::make_tuple(getfRedColorElement(px),getFloatColorElement(g),getfBlueColorElement(px));}
	void setBlueColorElement(Pixel& px, float b) {px=std::make_tuple(getfRedColorElement(px),getfGreenColorElement(px),b);}
	void setBlueColorElement(Pixel& px, uint8_t b) {px=std::make_tuple(getfRedColorElement(px),getfGreenColorElement(px),getFloatColorElement(b));}

	// Helper functions for colors
	Pixel blendColors(Pixel &colorbackground, Pixel &colorforeground, float alpha) {float r=0.0f; float g=0.0f; float b=0.0f;r = (std::get<0>(colorforeground) * alpha) + (std::get<0>(colorbackground) * (1.0 - alpha));g = (std::get<1>(colorforeground) * alpha) + (std::get<1>(colorbackground) * (1.0 - alpha));b = (std::get<2>(colorforeground) * alpha) + (std::get<2>(colorbackground) * (1.0 - alpha));return createfPixelWithColor(r,g,b);}
	void getHSV(double& h, double& s, double& v, const Pixel& px) {double r, g, b;std::tie(r, g, b) = px;double min_val = std::min({r, g, b});double max_val = std::max({r, g, b});double delta = max_val - min_val;v = max_val;if (max_val != 0.0) {s = delta / max_val;} else {s = 0.0;h = -1.0;return;}if (r == max_val) {h = (g - b) / delta;} else if (g == max_val) {h = 2.0 + (b - r) / delta;} else {h = 4.0 + (r - g) / delta;}h *= 60.0;if (h < 0) {h += 360.0;}h /= 360.0;}
	void setHSV(double h,double s,double v,Pixel& px) {if (s == 0) {px = {v, v, v};return;}h *= 360.0;h = std::fmod(h, 360.0);h /= 60.0;int i = std::floor(h);double f = h - i;double p = v * (1.0 - s);double q = v * (1.0 - s * f);double t = v * (1.0 - s * (1.0 - f));switch (i) {case 0:px = {v, t, p};break;case 1:px = {q, v, p};break;case 2:px = {p, v, t};break;case 3:px = {p, q, v};break;case 4:px = {t, p, v};break;default:px = {v, p, q};break;}}

	class Image
	{
	public:
		Image() {}

		Image(int width, int height) {
			resize(width, height);
		}
		
		explicit Image(std::string const& filename) {
			read(filename);
		}
        Image(const Image& other) : m_img(other.m_img), m_width(other.m_width), m_height(other.m_height) {}
        
        Image& operator=(const Image& other) {if (this == &other) return *this;m_img = other.m_img;m_width = other.m_width;m_height = other.m_height;return *this;}
        
        Image(Image&& other) noexcept : m_img(std::move(other.m_img)), m_width(other.m_width), m_height(other.m_height) {other.m_width = 0;other.m_height = 0;}
        
        Image& operator=(Image&& other) noexcept {
        	if (this == &other) return *this;
        	m_img = std::move(other.m_img);
        	m_width = other.m_width;
        	m_height = other.m_height;
        	other.m_width = 0;
        	other.m_height = 0;
        	return *this;
        }

        friend bool operator==(const Image& lhs, const Image& rhs)
        {
            if (lhs.m_width != rhs.m_width || lhs.m_height != rhs.m_height)
            {
                return false;
            }
            return lhs.m_img == rhs.m_img;
        }

        friend bool operator!=(const Image& lhs, const Image& rhs)
        {
            return !(lhs == rhs);
        }

        void resize(int width, int height) {
        	m_img.clear();
        	m_img.resize(width*height);
        	setWidth(width);
        	setHeight(height);
        }
		
		void setWidth(int width) { 
			m_width = width; 
		}
		
		void setHeight(int height) { 
			m_height = height; 
		}
		
		int getWidth() { 
			return m_width; 
		}
        
        int getHeight() { 
        	return m_height; 
        }
		
		void setPixel(int xCoord, int yCoord, const Pixel& newPixel) { 
			if (xCoord >= 0 && xCoord < m_width && yCoord >= 0 && yCoord < m_height) { 
				m_img[getIndex(xCoord, yCoord)] = newPixel; 
			}
		}
		
		const Pixel& getPixel(int xCoord, int yCoord) { 
			return m_img[xCoord + m_width * yCoord]; 
		}
		
		void setAllPixels(const Pixel& newPixel) {
			for(int y = 0; y < m_height; ++y) {
				for(int x = 0; x < m_width; ++x) {
					setPixel(x,y,newPixel);
				}
			}
		}

		void setImage(std::vector<Pixel>& image, int& width, int& height) {
			m_img.clear();
		    m_img = image;
		    m_width=width;
		    m_height=height;
		}

		std::vector<Pixel> getImage() {
		    return m_img;
		}
		
		void drawLine(Coord& startCoords, const Pixel& lineColor) {
			Coord clippedCoords;
			if (clipLineImpl(startCoords, clippedCoords)) {drawLineImpl(clippedCoords, lineColor);}
		}
		
		void getAngledLine(Coord& lineCoords, const Point& center, double degrees, int length) {
			double angleRad = degrees * (M_PI / 180.0);
			int xStart = std::get<0>(center);
			int yStart = std::get<1>(center);
			int xEnd = static_cast<int>(static_cast<double>(xStart) + std::cos(angleRad) * static_cast<double>(length));
			int yEnd = static_cast<int>(static_cast<double>(yStart) + std::sin(angleRad) * static_cast<double>(length));
			lineCoords = std::make_tuple(xStart, yStart, xEnd, yEnd);
		}
		
		void drawBezierQuadratic(const Point& pt0, const Point& pt1, const Point& pt2, int split, const Pixel& bezierColor) {
			std::vector<std::tuple<int, int>> xyCoordinates;
			quadraticBezierCurvesImpl(pt0, pt1, pt2, split, xyCoordinates);
			drawXyLineImpl(xyCoordinates, bezierColor, pt2);
		}
		
		void drawBezierCubic(const Point& pt0, const Point& pt1, const Point& pt2, const Point& pt3, int split, const Pixel& bezierColor) {
			std::vector<std::tuple<int, int>> xyCoordinates;
			cubicBezierCurvesImpl(pt0, pt1, pt2, pt3, split, xyCoordinates);
			drawXyLineImpl(xyCoordinates, bezierColor, pt3);
		}

		void drawRectangle(const Point& xy, const Point& wh, const Pixel& rectangleColor) {
        	drawRectImpl(xy, wh, rectangleColor);
    	}

		void drawFilledRectangle(const Point& xy, const Point& wh, const Pixel& rectangleColor) {
	        drawFilledRectImpl(xy, wh, rectangleColor);
	    }

		void drawCircle(const Point& xy, int radius, const Pixel& circleColor) {
	        drawCircleImpl(xy, radius, circleColor);
	    }

		void drawFilledCircle(const Point& xy, int radius, const Pixel& circleColor) {
	        drawFilledCircleImpl(xy, radius, circleColor);
	    }

		void drawWedge(const Point& center, int radius, int startAngle, int endAngle, const Pixel& wedgeColor) {
	        drawWedgeImpl(center, radius, startAngle, endAngle, wedgeColor);
	    }

		void drawFilledWedge(const Point& center, int radius, int startAngle, int endAngle, const Pixel& wedgeColor) {
	        drawFilledWedgeImpl(center, radius, startAngle, endAngle, wedgeColor);
	    }

		void drawTriangle(const Point& pt1, const Point& pt2, const Point& pt3, const Pixel& triangleColor) {
	        drawTriangleImpl(pt1, pt2, pt3, triangleColor);
	    }

		void drawFilledTriangle(const Point& pt1, const Point& pt2, const Point& pt3, const Pixel& fillColor) {
	        drawFilledTriangleImpl(pt1, pt2, pt3, fillColor);
	    }

		void drawRotatedRectangle(int x, int y, int w, int h, double angle, const Pixel& px) {
	        drawRotatedRectangleImpl(x, y, w, h, angle, px);
	    }

		void drawFilledRotatedRectangle(int x, int y, int w, int h, double angle, const Pixel& px) {
	        drawFilledRotatedRectangleImpl(x, y, w, h, angle, px);
	    }

		void drawRotatedEllipse(int x, int y, int w, int h, double angle, const Pixel& px) {
	        drawRotatedEllipseImpl(x, y, w, h, angle, px);
	    }

		void drawFilledRotatedEllipse(int x, int y, int w, int h, double angle, const Pixel& px) {
	        drawFilledRotatedEllipseImpl(x, y, w, h, angle, px);
	    }

		void drawRotatedPolygon(const std::vector<Point>& vertices, double angle, const Pixel& px) {
	        drawRotatedPolygonImpl(vertices, angle, px);
	    }

		void drawFilledRotatedPolygon(const std::vector<Point>& vertices, double angle, const Pixel& px) {
	        drawFilledRotatedPolygonImpl(vertices, angle, px);
	    }

		Pixel getAverageRgbOfImage() {
			double rSum = 0.0;
			double gSum = 0.0;
			double bSum = 0.0;
			double totalPixels = m_img.size();
			for (const auto& px : m_img) {
				rSum += std::get<0>(px);
				gSum += std::get<1>(px);
				bSum += std::get<2>(px);
			} 
			return createfPixelWithColor(rSum / totalPixels, gSum / totalPixels, bSum / totalPixels);
		}

		void convertToGrayscale() {
	        convertToGrayscaleImpl();
	    }

		void applyGaussianBlur() {
	        applyGaussianBlurImpl();
	    }

	    void applyAntiAliasing() {
            applyAntiAliasingImpl();
        }

        void downscale(int width, int height) {
        	downscaleImpl(width,height);
        }

        void upscale(int scale) {
        	upscaleImpl(scale);
        }

        void applyBloom(double threshold, double sigma) {
        	applyBloomImpl(threshold, sigma);
        }

        void applyLens(int numb) {
        	applyLensImpl(numb);
        }

		void drawGradients(const std::vector<Pixel>& colors, double angle_degree) {
			drawGradientsImpl(colors, angle_degree);
		}
		
		void read(const std::string& filename) {
			readImpl(filename);
		}

		void write(const std::string& filename) {
			writeImpl(filename);
		}
	private:
		int getIndex(int x, int y) {
	        return y * m_width + x;
	    }

		void drawLineImpl(const Coord& Coords, const Pixel& lineColor) {
	        int x1 = std::get<0>(Coords);
	        int y1 = std::get<1>(Coords);
	        int x2 = std::get<2>(Coords);
	        int y2 = std::get<3>(Coords);
	        
	        int dx = x2 - x1;
	        int dy = y2 - y1;
	        int dx1 = std::abs(dx);
	        int dy1 = std::abs(dy);
	        int px = 2 * dy1 - dx1;
	        int py = 2 * dx1 - dy1;
	        int x, y, xe, ye;

	        if (dx1 >= dy1) {
	            if (dx >= 0) { x = x1; y = y1; xe = x2; }
	            else { x = x2; y = y2; xe = x1; }

	            setPixel(x,y,lineColor);

	            for (int i = 0; x < xe; i++) {
	                x = x + 1;
	                if (px < 0) {
	                    px = px + 2 * dy1;
	                } else {
	                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1;
	                    else y = y - 1;
	                    px = px + 2 * (dy1 - dx1);
	                }
	                setPixel(x,y,lineColor);
	            }
	        } else {
	            if (dy >= 0) { x = x1; y = y1; ye = y2; }
	            else { x = x2; y = y2; ye = y1; }

	            setPixel(x,y,lineColor);

	            for (int i = 0; y < ye; i++) {
	                y = y + 1;
	                if (py <= 0) {
	                    py = py + 2 * dx1;
	                } else {
	                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1;
	                    else x = x - 1;
	                    py = py + 2 * (dx1 - dy1);
	                }
	                setPixel(x,y,lineColor);
	            }
	        }
	    }
		
		int findRegion(int x, int y) {
	        int code = 0;

	        if (y >= m_height) {
	            code |= 1;
	        } else if (y < 0) {
	            code |= 2;
	        }

	        if (x >= m_width) {
	            code |= 4;
	        } else if (x < 0) {
	            code |= 8;
	        }

	        return code;
	    }

		bool clipLineImpl(const Coord& startCoords, Coord& clippedCoords) {
	        int x1 = std::get<0>(startCoords);
	        int y1 = std::get<1>(startCoords);
	        int x2 = std::get<2>(startCoords);
	        int y2 = std::get<3>(startCoords);

	        int x3 = 0, y3 = 0, x4 = 0, y4 = 0;
	        int code1 = 0, code2 = 0, codeout = 0;

	        bool accept = false, done = false;
	        code1 = findRegion(x1, y1);
	        code2 = findRegion(x2, y2);

	        do {
	            if (!(code1 | code2)) {
	                accept = done = true;
	            } else if (code1 & code2) {
	                done = true;
	            } else {
	                int x, y;
	                codeout = code1 ? code1 : code2;

	                if (codeout & 1) { // top edge
	                    x = x1 + (x2 - x1) * (m_height - y1) / (y2 - y1);
	                    y = m_height - 1;
	                } else if (codeout & 2) { // bottom edge
	                    x = x1 + (x2 - x1) * -y1 / (y2 - y1);
	                    y = 0;
	                } else if (codeout & 4) { // right edge
	                    y = y1 + (y2 - y1) * (m_width - x1) / (x2 - x1);
	                    x = m_width - 1;
	                } else { // left edge
	                    y = y1 + (y2 - y1) * -x1 / (x2 - x1);
	                    x = 0;
	                }

	                if (codeout == code1) {
	                    x1 = x;
	                    y1 = y;
	                    code1 = findRegion(x1, y1);
	                } else {
	                    x2 = x;
	                    y2 = y;
	                    code2 = findRegion(x2, y2);
	                }
	            }
	        } while (!done);

	        if (accept) {
	            clippedCoords = std::make_tuple(x1, y1, x2, y2);
	            return true;
	        } else {
	            clippedCoords = std::make_tuple(0, 0, 0, 0);
	            return false;
	        }
	    }
		
		void quadraticBezierCurvesImpl(const Point& pt0, const Point& pt1, const Point& pt2, int split, std::vector<Point>& xyCoordinates) {
	        double t = 0.0;
	        double delta = double(1) / double(split);

	        int x = 0;
	        int y = 0;
	        int x1 = 0;
	        int y1 = 0;
	        int x2 = 0;
	        int y2 = 0;

	        for (int i = 0; i < split; ++i) {
	            x1 = std::lerp(std::get<0>(pt0), std::get<0>(pt1), t);
	            y1 = std::lerp(std::get<1>(pt0), std::get<1>(pt1), t);

	            x2 = std::lerp(std::get<0>(pt1), std::get<0>(pt2), t);
	            y2 = std::lerp(std::get<1>(pt1), std::get<1>(pt2), t);

	            x = std::lerp(x1, x2, t);
	            y = std::lerp(y1, y2, t);

	            xyCoordinates.push_back(std::make_tuple(x, y));

	            t += delta;
	        }
	    }

		void cubicBezierCurvesImpl(const Point& pt0, const Point& pt1, const Point& pt2, const Point& pt3, int split, std::vector<Point>& xyCoordinates) {
		    double t = 0.0;
		    double delta = 1.0 / static_cast<double>(split);

		    for (int i = 0; i <= split; ++i) {  // Changed condition to include the final point
		        auto [x0, y0] = pt0;
		        auto [x1, y1] = pt1;
		        auto [x2, y2] = pt2;
		        auto [x3, y3] = pt3;

		        double xA = std::lerp(x0, x1, t);  // Changed type to double
		        double yA = std::lerp(y0, y1, t);
		        double xB = std::lerp(x1, x2, t);
		        double yB = std::lerp(y1, y2, t);
		        double xC = std::lerp(x2, x3, t);
		        double yC = std::lerp(y2, y3, t);

		        double xAA = std::lerp(xA, xB, t);
		        double yAA = std::lerp(yA, yB, t);
		        double xBB = std::lerp(xB, xC, t);
		        double yBB = std::lerp(yB, yC, t);

		        int xFinal = static_cast<int>(std::lerp(xAA, xBB, t));  // Cast back to int
		        int yFinal = static_cast<int>(std::lerp(yAA, yBB, t));

		        xyCoordinates.push_back(std::make_tuple(xFinal, yFinal));
		        t += delta;
		    }
		}

		void drawXyLineImpl(const std::vector<Point>& xyCoordinates, const Pixel& bezierColor, const Point& pt2) {
		    Point temp = xyCoordinates.front();
		    Point store = pt2;

		    std::vector<Point> updatedXyCoordinates = xyCoordinates;
		    updatedXyCoordinates.push_back(store);

		    std::vector<Coord> lineSegments;

		    for (size_t i = 1; i < updatedXyCoordinates.size(); ++i) {
		        store = updatedXyCoordinates[i];

		        auto [xTemp, yTemp] = temp;
		        auto [xStore, yStore] = store;

		        lineSegments.push_back(std::make_tuple(xTemp, yTemp, xStore, yStore));
		        temp = store;
		    }

		    for (auto& line : lineSegments) {
		        drawLine(line, bezierColor);
		    }
		}

		void drawXyDotsImpl(const std::vector<Point>& xy, const Pixel& color) {
		    for (const auto& pt : xy) {
		        auto [x, y] = pt;
		        m_img[getIndex(x, y)] = color;
		    }
		}
		
		void drawRectImpl(const Point& topLeft, const Point& dimensions, const Pixel& rectangleColor) {
		    auto [x, y] = topLeft;
		    auto [w, h] = dimensions;

		    Coord lineCoords;
		    lineCoords = std::make_tuple(x, y, x + w, y);
		    drawLine(lineCoords, rectangleColor);
		    lineCoords = std::make_tuple(x + w, y, x + w, y + h);
		    drawLine(lineCoords, rectangleColor);
		    lineCoords = std::make_tuple(x + w, y + h, x, y + h);
		    drawLine(lineCoords, rectangleColor);
		    lineCoords = std::make_tuple(x, y + h, x, y);
		    drawLine(lineCoords, rectangleColor);
		}

		void drawFilledRectImpl(const Point& topLeft, const Point& dimensions, const Pixel& rectangleColor) {
		    auto [x, y] = topLeft;
		    auto [w, h] = dimensions;

		    int x2 = x + w;
		    int y2 = y + h;

		    Coord lineCoords;

		    for (int i = y; i < y2; ++i) {
		        lineCoords = std::make_tuple(x, i, x2, i);
		        drawLine(lineCoords, rectangleColor);
		    }
		}

		void drawCircleImpl(const Point& center, int radius, const Pixel& circleColor) {
		    if (radius == 0) return;

		    auto [centerX, centerY] = center;

		    int x0 = 0;
		    int y0 = radius;
		    int d = 3 - 2 * radius;

		    Coord lineCoords;

		    while (y0 >= x0) {
		        for (int i = 0; i < 8; ++i) {
		            int xx, yy;

		            switch (i) {
		                case 0: xx = centerX + x0; yy = centerY - y0; break;
		                case 1: xx = centerX + y0; yy = centerY - x0; break;
		                case 2: xx = centerX + y0; yy = centerY + x0; break;
		                case 3: xx = centerX + x0; yy = centerY + y0; break;
		                case 4: xx = centerX - x0; yy = centerY + y0; break;
		                case 5: xx = centerX - y0; yy = centerY + x0; break;
		                case 6: xx = centerX - y0; yy = centerY - x0; break;
		                case 7: xx = centerX - x0; yy = centerY - y0; break;
		            }

		            lineCoords = std::make_tuple(xx, yy, xx, yy);
		            drawLine(lineCoords, circleColor);
		        }

		        if (d < 0) {
		            d += 4 * x0++ + 6;
		        } else {
		            d += 4 * (x0++ - y0--) + 10;
		        }
		    }
		}

		void drawFilledCircleImpl(const Point& xy, int radius, const Pixel& circleColor) {
		    if (radius == 0) {
		        return;
		    }

		    auto [x, y] = xy;
		    int x0 = 0;
		    int y0 = radius;
		    int d = 3 - 2 * radius;

		    while (y0 >= x0) {
		        Coord upperLeft  = {x - x0, y - y0, x + x0, y - y0};
		        Coord upperRight = {x - y0, y - x0, x + y0, y - x0};
		        Coord lowerLeft  = {x - x0, y + y0, x + x0, y + y0};
		        Coord lowerRight = {x - y0, y + x0, x + y0, y + x0};

		        drawLine(upperLeft,  circleColor);
		        drawLine(upperRight, circleColor);
		        drawLine(lowerLeft,  circleColor);
		        drawLine(lowerRight, circleColor);

		        if (d < 0) {
		            d += 4 * x0++ + 6;
		        } else {
		            d += 4 * (x0++ - y0--) + 10;
		        }
		    }
		}

		void drawWedgeImpl(const Point& center, int radius, int startAngle, int endAngle, const Pixel& wedgeColor) {
		    std::vector<Point> Coords;
		    const float pi = 3.14159265358979323846;

		    auto [centerX, centerY] = center;

		    float angle = std::min(startAngle, endAngle) * (pi / 180.0);
		    float range = std::max(startAngle, endAngle) * (pi / 180.0);

		    float x = radius * std::cos(angle);
		    float y = radius * std::sin(angle);

		    do {
		        Coords.emplace_back(static_cast<int>(centerX + x + 0.5), static_cast<int>(centerY - y + 0.5));
		        angle += 0.001;
		        x = radius * std::cos(angle);
		        y = radius * std::sin(angle);
		    } while (angle <= range);

		    auto Coord1 = Coords.front();
		    auto Coord2 = Coords.back();

		    Coord line1 = {centerX, centerY, std::get<0>(Coord1), std::get<1>(Coord1)};
		    Coord line2 = {centerX, centerY, std::get<0>(Coord2), std::get<1>(Coord2)};

		    getLineCoords(line1, Coords);
		    getLineCoords(line2, Coords);

		    for (const auto& [x, y] : Coords) {
		        Coord singlePointLine = {x, y, x, y};
		        drawLine(singlePointLine, wedgeColor);
		    }
		}

		void drawFilledWedgeImpl(const Point& center, int radius, int startAngle, int endAngle, const Pixel& wedgeColor) {
		    std::vector<Point> Coords;
		    const float pi = 3.14159265358979323846;

		    auto [centerX, centerY] = center;

		    float angle = std::min(startAngle, endAngle) * (pi / 180.0);
		    float range = std::max(startAngle, endAngle) * (pi / 180.0);

		    float x = radius * std::cos(angle);
		    float y = radius * std::sin(angle);

		    do {
		        Coords.emplace_back(static_cast<int>(centerX + x + 0.5), static_cast<int>(centerY - y + 0.5));
		        angle += 0.001;
		        x = radius * std::cos(angle);
		        y = radius * std::sin(angle);
		    } while (angle <= range);

		    auto Coord1 = Coords.front();
		    auto Coord2 = Coords.back();

		    Coord line1 = {centerX, centerY, std::get<0>(Coord1), std::get<1>(Coord1)};
		    Coord line2 = {centerX, centerY, std::get<0>(Coord2), std::get<1>(Coord2)};

		    getLineCoords(line1, Coords);
		    getLineCoords(line2, Coords);

		    std::vector<int> ys;
		    getAllYs(ys, Coords);

		    std::vector<Coord> lines;
		    for (int searchY : ys) {
		        std::vector<int> xs;
		        for (const auto& [x, y] : Coords) {
		            if (y == searchY) {
		                xs.push_back(x);
		            }
		        }
		        std::sort(xs.begin(), xs.end());
		        lines.emplace_back(xs.front(), searchY, xs.back(), searchY);
		    }

		    for (const auto& lineCoord : lines) {
		        Coord non_const_lineCoord = lineCoord;
				drawLine(non_const_lineCoord, wedgeColor);
		    }
		}

		void drawTriangleImpl(const Point& pt1, const Point& pt2, const Point& pt3, const Pixel& triangleColor) {
		    auto [x1, y1] = pt1;
		    auto [x2, y2] = pt2;
		    auto [x3, y3] = pt3;

		    Coord line1 = {x1, y1, x2, y2};
		    Coord line2 = {x2, y2, x3, y3};
		    Coord line3 = {x3, y3, x1, y1};

		    drawLine(line1, triangleColor);
		    drawLine(line2, triangleColor);
		    drawLine(line3, triangleColor);
		}

		void drawFilledTriangleImpl(const Point& pt1, const Point& pt2, const Point& pt3, const Pixel& fillColor) {
		    auto [x1, y1] = pt1;
		    auto [x2, y2] = pt2;
		    auto [x3, y3] = pt3;

		    if (y1 > y2) { std::swap(y1, y2); std::swap(x1, x2); }
		    if (y1 > y3) { std::swap(y1, y3); std::swap(x1, x3); }
		    if (y2 > y3) { std::swap(y2, y3); std::swap(x2, x3); }

		    int t1x = x1, t2x = x1, y = y1;
		    int dx1 = std::abs(x2 - x1), dx2 = std::abs(x3 - x1);
		    int dy1 = y2 - y1, dy2 = y3 - y1;
		    int signx1 = (x2 - x1 > 0) ? 1 : -1, signx2 = (x3 - x1 > 0) ? 1 : -1;
		    int e1 = dx1 / 2, e2 = dx2 / 2;
		    bool changed1 = false, changed2 = false;
		    if (dy1 > dx1) { std::swap(dy1, dx1); changed1 = true; }
		    if (dy2 > dx2) { std::swap(dy2, dx2); changed2 = true; }

		    for (; y <= y3; y++) {
		        int minx = std::min(t1x, t2x);
		        int maxx = std::max(t1x, t2x);
		        Coord tempCoord = {minx, y, maxx, y};
				drawLine(tempCoord, fillColor);


		        // Update t1x, t2x
		        for (int i = 0; i < dx1; i++) {
		            e1 += dy1;
		            while (e1 >= dx1) {
		                e1 -= dx1;
		                if (changed1) break;
		                else t1x += signx1;
		            }
		            if (changed1) t1x += signx1;
		        }

		        for (int i = 0; i < dx2; i++) {
		            e2 += dy2;
		            while (e2 >= dx2) {
		                e2 -= dx2;
		                if (changed2) break;
		                else t2x += signx2;
		            }
		            if (changed2) t2x += signx2;
		        }
		    }
		}
		
		void drawRotatedRectangleImpl(int x, int y, int w, int h, double angle, const Pixel& px) {
		    double rad = angle * M_PI / 180.0;
		    double cos_angle = std::cos(rad);
		    double sin_angle = std::sin(rad);
		    
		    int cx = x + w / 2;
		    int cy = y + h / 2;
		    
		    std::vector<Point> corners;
		    
		    for (int dx : {-w / 2, w / 2}) {
		        for (int dy : {-h / 2, h / 2}) {
		            int x_rot = cx + dx * cos_angle - dy * sin_angle;
		            int y_rot = cy + dx * sin_angle + dy * cos_angle;
		            corners.emplace_back(x_rot, y_rot);
		        }
		    }

		    Coord Coords1 = std::make_tuple(std::get<0>(corners[0]), std::get<1>(corners[0]), std::get<0>(corners[1]), std::get<1>(corners[1]));
		    Coord Coords2 = std::make_tuple(std::get<0>(corners[1]), std::get<1>(corners[1]), std::get<0>(corners[3]), std::get<1>(corners[3]));
		    Coord Coords3 = std::make_tuple(std::get<0>(corners[3]), std::get<1>(corners[3]), std::get<0>(corners[2]), std::get<1>(corners[2]));
		    Coord Coords4 = std::make_tuple(std::get<0>(corners[2]), std::get<1>(corners[2]), std::get<0>(corners[0]), std::get<1>(corners[0]));

		    drawLine(Coords1, px);
		    drawLine(Coords2, px);
		    drawLine(Coords3, px);
		    drawLine(Coords4, px);
		}

		void drawFilledRotatedRectangleImpl(int x, int y, int w, int h, double angle, const Pixel& px) {
		    double angle_rad = -angle * M_PI / 180.0;
		    double cos_angle = std::cos(angle_rad);
		    double sin_angle = std::sin(angle_rad);
		    
		    double aabb_width = std::abs(w * cos_angle) + std::abs(h * sin_angle);
		    double aabb_height = std::abs(w * sin_angle) + std::abs(h * cos_angle);
		    
		    int rect_center_x = x + w / 2;
		    int rect_center_y = y + h / 2;
		    
		    int aabb_x1 = rect_center_x - aabb_width / 2;
		    int aabb_y1 = rect_center_y - aabb_height / 2;
		    int aabb_x2 = rect_center_x + aabb_width / 2;
		    int aabb_y2 = rect_center_y + aabb_height / 2;
		    
		    for (int image_y = aabb_y1; image_y <= aabb_y2; ++image_y) {
		        for (int image_x = aabb_x1; image_x <= aabb_x2; ++image_x) {
		            int dx = image_x - rect_center_x;
		            int dy = image_y - rect_center_y;
		            
		            int rect_x = dx * cos_angle - dy * sin_angle;
		            int rect_y = dx * sin_angle + dy * cos_angle;
		            
		            if (isInsideRectangle(rect_x, rect_y, w, h)) {
		                if (image_x >= 0 && image_x < m_width && image_y >= 0 && image_y < m_height) {
		                    setPixel(image_x, image_y, px);
		                }
		            }
		        }
		    }
		}

		void drawRotatedEllipseImpl(int x, int y, int w, int h, double angle, const Pixel& px) {
		    double angle_rad = angle * M_PI / 180.0;
		    double cos_angle = std::cos(angle_rad);
		    double sin_angle = std::sin(angle_rad);

		    int ellipse_center_x = x + w / 2;
		    int ellipse_center_y = y + h / 2;

		    for (double theta = 0; theta <= 2 * M_PI; theta += 0.001) {
		        int dx = w / 2 * std::cos(theta);
		        int dy = h / 2 * std::sin(theta);

		        int image_x = ellipse_center_x + (dx * cos_angle - dy * sin_angle);
		        int image_y = ellipse_center_y + (dx * sin_angle + dy * cos_angle);

		        if (image_x >= 0 && image_x < m_width && image_y >= 0 && image_y < m_height) {
		            m_img[getIndex(image_x, image_y)] = px;
		        }
		    }
		}

		void drawFilledRotatedEllipseImpl(int x, int y, int w, int h, double angle, const Pixel& px) {
		    double angle_rad = angle * M_PI / 180.0;
		    double cos_angle = std::cos(angle_rad);
		    double sin_angle = std::sin(angle_rad);

		    int ellipse_center_x = x + w / 2;
		    int ellipse_center_y = y + h / 2;

		    int aabb_x1 = ellipse_center_x - (w / 2) - std::abs((h / 2) * sin_angle);
		    int aabb_x2 = ellipse_center_x + (w / 2) + std::abs((h / 2) * sin_angle);
		    int aabb_y1 = ellipse_center_y - (h / 2) - std::abs((w / 2) * cos_angle);
		    int aabb_y2 = ellipse_center_y + (h / 2) + std::abs((w / 2) * cos_angle);

		    auto isinbounds = [this](const Point& pt) -> bool {
		        return std::get<0>(pt) >= 0 && std::get<0>(pt) < m_width && std::get<1>(pt) >= 0 && std::get<1>(pt) < m_height;
		    };

		    for (int scan_y = aabb_y1; scan_y <= aabb_y2; ++scan_y) {
		        for (int scan_x = aabb_x1; scan_x <= aabb_x2; ++scan_x) {
		            int dx = scan_x - ellipse_center_x;
		            int dy = scan_y - ellipse_center_y;

		            int rotated_x = dx * cos_angle + dy * sin_angle;
		            int rotated_y = -dx * sin_angle + dy * cos_angle;

		            if (4.0 * rotated_x * rotated_x / (w * w) + 4.0 * rotated_y * rotated_y / (h * h) <= 1.0) {
		                Point pt = std::make_tuple(scan_x, scan_y);
		                if (isinbounds(pt)) {
		                    m_img[getIndex(scan_x, scan_y)] = px;
		                }
		            }
		        }
		    }
		}

		void drawRotatedPolygonImpl(const std::vector<Point>& vertices, double angle, const Pixel& px) {
		    int sum_x = 0, sum_y = 0;
		    for (const auto& vertex : vertices) {
		        sum_x += std::get<0>(vertex);
		        sum_y += std::get<1>(vertex);
		    }
		    Point centroid = std::make_tuple(sum_x / vertices.size(), sum_y / vertices.size());

		    auto rotate_Point = [centroid](Point p, double angle_rad) {
		        double cos_angle = std::cos(angle_rad);
		        double sin_angle = std::sin(angle_rad);
		        int dx = std::get<0>(p) - std::get<0>(centroid);
		        int dy = std::get<1>(p) - std::get<1>(centroid);
		        int rotated_x = dx * cos_angle - dy * sin_angle;
		        int rotated_y = dx * sin_angle + dy * cos_angle;
		        return std::make_tuple(std::get<0>(centroid) + rotated_x, std::get<1>(centroid) + rotated_y);
		    };

		    double angle_rad = angle * M_PI / 180.0;
		    std::vector<Point> rotated_vertices;
		    for (const auto& vertex : vertices) {
		        rotated_vertices.push_back(rotate_Point(vertex, angle_rad));
		    }

		    auto isinbounds = [this](const Point& pt) -> bool {
		        return std::get<0>(pt) >= 0 && std::get<0>(pt) < m_width && std::get<1>(pt) >= 0 && std::get<1>(pt) < m_height;
		    };

		    for (size_t i = 0; i < rotated_vertices.size(); ++i) {
		        Point p1 = rotated_vertices[i];
		        Point p2 = rotated_vertices[(i + 1) % rotated_vertices.size()];
		        if (isinbounds(p1) && isinbounds(p2)) {
		            Coord line = std::make_tuple(std::get<0>(p1), std::get<1>(p1), std::get<0>(p2), std::get<1>(p2));
		            drawLine(line, px);
		        }
		    }
		}

		void drawFilledRotatedPolygonImpl(const std::vector<Point>& vertices, double angle, const Pixel& px) {
		    int sum_x = 0, sum_y = 0;
		    for (const auto& vertex : vertices) {
		        sum_x += std::get<0>(vertex);
		        sum_y += std::get<1>(vertex);
		    }
		    Point centroid = std::make_tuple(sum_x / vertices.size(), sum_y / vertices.size());

		    auto rotate_Point = [centroid](Point p, double angle_rad) {
		        double cos_angle = std::cos(angle_rad);
		        double sin_angle = std::sin(angle_rad);
		        int dx = std::get<0>(p) - std::get<0>(centroid);
		        int dy = std::get<1>(p) - std::get<1>(centroid);
		        int rotated_x = dx * cos_angle - dy * sin_angle;
		        int rotated_y = dx * sin_angle + dy * cos_angle;
		        return std::make_tuple(std::get<0>(centroid) + rotated_x, std::get<1>(centroid) + rotated_y);
		    };

		    double angle_rad = angle * M_PI / 180.0;
		    std::vector<Point> rotated_vertices;
		    for (const auto& vertex : vertices) {
		        rotated_vertices.push_back(rotate_Point(vertex, angle_rad));
		    }

		    auto isinbounds = [this](const Point& pt) -> bool {
		        return std::get<0>(pt) >= 0 && std::get<0>(pt) < m_width && std::get<1>(pt) >= 0 && std::get<1>(pt) < m_height;
		    };

		    for (int y = 0; y < m_height; ++y) {
		        std::vector<int> intersections;
		        for (size_t i = 0; i < rotated_vertices.size(); ++i) {
		            Point p1 = rotated_vertices[i];
		            Point p2 = rotated_vertices[(i + 1) % rotated_vertices.size()];
		            int y1 = std::get<1>(p1), y2 = std::get<1>(p2);
		            int x1 = std::get<0>(p1), x2 = std::get<0>(p2);

		            if ((y1 < y && y2 >= y) || (y2 < y && y1 >= y)) {
		                int x = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
		                intersections.push_back(x);
		            }
		        }

		        std::sort(intersections.begin(), intersections.end());

		        for (size_t i = 0; i + 1 < intersections.size(); i += 2) {
		            for (int x = intersections[i]; x <= intersections[i + 1]; ++x) {
		                if (isinbounds({x, y})) {
		                    setPixel(x,y,px);
		                }
		            }
		        }
		    }
		}

		bool isGrayscaleRGBImpl(double r, double g, double b) {
		    return r == g && g == b;
		}

	    void convertToGrayscaleImpl() {
	        for (auto& pixel : m_img) {
	            auto& [r, g, b] = pixel;
	            if (!isGrayscaleRGBImpl(r,g,b)) {
	            	double gray = 0.299 * r + 0.587 * g + 0.114 * b;
	            	pixel = {gray, gray, gray};
	        	}
	        }
		}

	    void applyGaussianBlurImpl() {
	        std::array<std::array<double, 3>, 3> kernel = {{
	            {1.0 / 16, 2.0 / 16, 1.0 / 16},
	            {2.0 / 16, 4.0 / 16, 2.0 / 16},
	            {1.0 / 16, 2.0 / 16, 1.0 / 16}
	        }};
	        
	        for (int y = 1; y < m_height - 1; ++y) {
	            for (int x = 1; x < m_width - 1; ++x) {
	                double sumR = 0.0, sumG = 0.0, sumB = 0.0;

	                for (int k = -1; k <= 1; ++k) {
	                    for (int j = -1; j <= 1; ++j) {
	                        const Pixel& px = m_img[getIndex(x + j, y + k)];
	                        auto [r, g, b] = px;
	                        sumR += r * kernel[k + 1][j + 1];
	                        sumG += g * kernel[k + 1][j + 1];
	                        sumB += b * kernel[k + 1][j + 1];
	                    }
	                }

	                m_img[getIndex(x, y)] = {sumR, sumG, sumB};
	            }
	        }
	    }

	    double cubicWeightImpl(double distance) {
		    // The cubic convolution function (you can change the coefficient -0.5 to -1 or -2 for sharper or softer interpolation)
		    const double a = -0.5; 

		    double absDistance = std::abs(distance);
		    double absDistanceSquared = absDistance * absDistance;
		    double absDistanceCubed = absDistanceSquared * absDistance;

		    if (absDistance <= 1.0) {
		        return (a + 2.0) * absDistanceCubed - (a + 3.0) * absDistanceSquared + 1.0;
		    } else if (absDistance <= 2.0) {
		        return a * absDistanceCubed - 5.0 * a * absDistanceSquared + 8.0 * a * absDistance - 4.0 * a;
		    } else {
		        return 0.0;
		    }
		}

		Pixel bicubicInterpolateImpl(const std::vector<Pixel>& image, double x, double y, int originalWidth, int originalHeight) {
	        int fx = static_cast<int>(std::floor(x));
	        int fy = static_cast<int>(std::floor(y));
	        Pixel result(0.0, 0.0, 0.0);

	        for (int m = -1; m <= 2; ++m) {
	            for (int n = -1; n <= 2; ++n) {
	                // Safely get pixel color (clamp or wrap based on your need)
	                Pixel sample = image[std::clamp(fy + m, 0, originalHeight - 1) * originalWidth + std::clamp(fx + n, 0, originalWidth - 1)];
	                double weight = cubicWeightImpl(x - (fx + n)) * cubicWeightImpl(y - (fy + m));

	                std::get<0>(result) += weight * std::get<0>(sample);
	                std::get<1>(result) += weight * std::get<1>(sample);
	                std::get<2>(result) += weight * std::get<2>(sample);
	            }
	        }

	         // Clamp the result to the valid pixel value range (0.0 to 1.0)
		    std::get<0>(result) = std::clamp(std::get<0>(result), 0.0, 1.0);
		    std::get<1>(result) = std::clamp(std::get<1>(result), 0.0, 1.0);
		    std::get<2>(result) = std::clamp(std::get<2>(result), 0.0, 1.0);

	        return result;
	    }

	    void upscaleImpl(int scale) {
	        // Calculate new dimensions
		    int newWidth = m_width * scale;
		    int newHeight = m_height * scale;
		    std::vector<Pixel> newImg(newWidth * newHeight, Pixel(0,0,0)); // Initialize upscaled image with black pixels

		    // Iterate over new image pixels and assign bicubic interpolated values from old image
		    for (int y = 0; y < newHeight; ++y) {
		        for (int x = 0; x < newWidth; ++x) {
		            // Calculate the position on the original image
		            double origX = static_cast<double>(x) / static_cast<double>(scale);
		            double origY = static_cast<double>(y) / static_cast<double>(scale);

		            // Bicubic interpolate pixel from original image
		            newImg[y * newWidth + x] = bicubicInterpolateImpl(m_img, origX, origY, m_width, m_height);
		        }
		    }

		    // Replace current image with the upscaled image
		    m_img = std::move(newImg);
		    m_width = newWidth;
		    m_height = newHeight;
	    }

	    void downscaleImpl(int width, int height) {
	        // Calculate the scale assuming the upscale and downscale factors are the same
	        int scale = m_width / width;

	        // New image data
	        std::vector<Pixel> newImg(width * height);

	        // Averaging blocks of pixels
	        for (int y = 0; y < height; ++y) {
	            for (int x = 0; x < width; ++x) {
	                double sumR = 0.0, sumG = 0.0, sumB = 0.0;

	                // Average the colors of the `scale`x`scale` block
	                for (int dy = 0; dy < scale; ++dy) {
	                    for (int dx = 0; dx < scale; ++dx) {
	                        Pixel p = m_img[(y * scale + dy) * m_width + (x * scale + dx)];
	                        sumR += std::get<0>(p);
	                        sumG += std::get<1>(p);
	                        sumB += std::get<2>(p);
	                    }
	                }

	                // Calculate the average color
	                double numPixels = scale * scale;
	                newImg[y * width + x] = {
	                    sumR / numPixels,
	                    sumG / numPixels,
	                    sumB / numPixels
	                };
	            }
	        }

	        // Set the downscaled image as the current image
	        m_img.swap(newImg);
	        m_width = width;
	        m_height = height;
	    }

	    void applyAntiAliasingImpl() {
            int width=m_width;
            int height=m_height;
            int upscaleFactor = 4;
            upscaleImpl(upscaleFactor);
            applyGaussianBlurImpl();
            downscaleImpl(width,height);
        }

        void applyBloomImpl(double threshold, double sigma) {
            // Lambda function for Gaussian blur
            auto gaussianBlur = [&](std::vector<Pixel>& img) {
                int kernelSize = static_cast<int>(std::round(sigma * 6)) + 1;
                std::vector<double> kernel(kernelSize);
                double sigma2 = 2 * sigma * sigma;
                double sum = 0.0;

                // Generate Gaussian kernel
                for (int i = 0; i < kernelSize; ++i) {
                    int x = i - kernelSize / 2;
                    kernel[i] = std::exp(-(x * x) / sigma2);
                    sum += kernel[i];
                }

                // Normalize the kernel
                for (double &value : kernel) {
                    value /= sum;
                }

                // Apply Gaussian blur horizontally
                std::vector<Pixel> blurred(m_img.size());
                for (int y = 0; y < m_height; ++y) {
                    for (int x = 0; x < m_width; ++x) {
                        double red = 0.0, green = 0.0, blue = 0.0;
                        for (int k = 0; k < kernelSize; ++k) {
                            int pixelPosX = x + k - kernelSize / 2;
                            if (pixelPosX < 0) pixelPosX = 0;
                            if (pixelPosX >= m_width) pixelPosX = m_width - 1;

                            Pixel &p = img[pixelPosX + y * m_width];
                            red += std::get<0>(p) * kernel[k];
                            green += std::get<1>(p) * kernel[k];
                            blue += std::get<2>(p) * kernel[k];
                        }
                        blurred[x + y * m_width] = Pixel(red, green, blue);
                    }
                }

                // Apply Gaussian blur vertically
                for (int y = 0; y < m_height; ++y) {
                    for (int x = 0; x < m_width; ++x) {
                        double red = 0.0, green = 0.0, blue = 0.0;
                        for (int k = 0; k < kernelSize; ++k) {
                            int pixelPosY = y + k - kernelSize / 2;
                            if (pixelPosY < 0) pixelPosY = 0;
                            if (pixelPosY >= m_height) pixelPosY = m_height - 1;

                            Pixel &p = blurred[x + pixelPosY * m_width];
                            red += std::get<0>(p) * kernel[k];
                            green += std::get<1>(p) * kernel[k];
                            blue += std::get<2>(p) * kernel[k];
                        }
                        img[x + y * m_width] = Pixel(red, green, blue);
                    }
                }
            };

            std::vector<Pixel> brightPass(m_img.begin(), m_img.end());
            for (Pixel& pixel : brightPass) {
                double brightness = 0.2126 * std::get<0>(pixel) + 0.7152 * std::get<1>(pixel) + 0.0722 * std::get<2>(pixel);
                if (brightness <= threshold) {
                    pixel = Pixel(0, 0, 0);
                }
            }

            gaussianBlur(brightPass);

            for (size_t i = 0; i < m_img.size(); ++i) {
                double br, bg, bb;
                std::tie(br, bg, bb) = brightPass[i];

                double r, g, b;
                std::tie(r, g, b) = m_img[i];

                m_img[i] = Pixel(
                    std::min(r + br, 1.0),
                    std::min(g + bg, 1.0),
                    std::min(b + bb, 1.0)
                );
            }
        }

        void applyLensImpl(int numb) {
            int lensRadius = m_width / 10; // Example radius for each lens
            double refractionIndex = 1.5; // Index of refraction for the lens material

            std::random_device rd;
            std::mt19937 gen(rd());

            std::uniform_int_distribution<> disWidth(0, m_width - 1);
            std::uniform_int_distribution<> disHeight(0, m_height - 1);

            for (int i = 0; i < numb; ++i) {
                int centerX = disWidth(gen);
                int centerY = disHeight(gen);

                for (int y = -lensRadius; y <= lensRadius; ++y) {
                    for (int x = -lensRadius; x <= lensRadius; ++x) {
                        if (x * x + y * y <= lensRadius * lensRadius) {
                            double dist = std::sqrt(x * x + y * y);
                            double distRatio = dist / lensRadius;
                            double theta = std::atan2(y, x);
                            double refraction = 1.0 - std::sqrt(1.0 - std::pow(refractionIndex - 1.0, 2)) / refractionIndex;
                            double newDist = distRatio * dist * refraction;

                            int srcX = static_cast<int>(centerX + newDist * std::cos(theta));
                            int srcY = static_cast<int>(centerY + newDist * std::sin(theta));

                            srcX = std::clamp(srcX, 0, m_width - 1);
                            srcY = std::clamp(srcY, 0, m_height - 1);

                            Pixel& srcPixel = m_img[srcX + srcY * m_width];

                            setPixel(centerX + x, centerY + y, srcPixel);
                        }
                    }
                }
            }
        }

	    void drawGradientsImpl(const std::vector<Pixel>& colors, double angle_degree) {
	        double angle = angle_degree * M_PI / 180.0;

	        if (colors.size() < 2 || colors.size() > 5) {
	            std::cerr << "Invalid number of colors";
	            return;
	        }

	        auto interpolate = [](const Pixel& a, const Pixel& b, double t) -> Pixel {
	            auto [r1, g1, b1] = a;
	            auto [r2, g2, b2] = b;
	            return {r1 + t * (r2 - r1), g1 + t * (g2 - g1), b1 + t * (b2 - b1)};
	        };

	        double max_projection = m_width * std::cos(angle) + m_height * std::sin(angle);
	        double min_projection = 0;
	        double total_length = max_projection - min_projection;
	        double segment_length = total_length / (colors.size() - 1);

	        for (int y = 0; y < m_height; ++y) {
	            for (int x = 0; x < m_width; ++x) {
	                double projection = x * std::cos(angle) + y * std::sin(angle);
	                int segment_index = std::min(static_cast<int>(projection / segment_length), static_cast<int>(colors.size()) - 2);
	                double t_local = (projection - segment_index * segment_length) / segment_length;

	                Pixel color = interpolate(colors[segment_index], colors[segment_index + 1], t_local);
	                setPixel(x,y,color);
	            }
	        }
	    }

	    void readImpl(const std::string& filename) {
	        std::string magic;
	        int max;
	        uint8_t buffer[3];
	        Pixel color;

	        std::ifstream in(filename, std::ifstream::binary);
	        if (!in.is_open()) {
	            std::cout << "Can't open " << filename << std::endl;
	            exit(1);
	        }

	        in >> magic;
	        in.seekg(1, std::ios::cur);
	        char c;
	        in.get(c);
	        if (c == '#') {
	            while (c != '\n') {
	                in.get(c);
	            }
	        } else {
	            in.seekg(-1, std::ios::cur);
	        }

	        in >> m_width >> m_height >> max;
	        if (max != 255) {
	            std::cout << "Not 8 bit per RGB color." << std::endl;
	            exit(1);
	        }

	        if (magic == "P6") {
	            in.seekg(1, std::ios::cur);
	            m_img.clear();
	            for (int i = 0; i < m_width * m_height; ++i) {
	                in.read(reinterpret_cast<char*>(buffer), 3);
	                color = std::make_tuple(
	                    static_cast<double>(buffer[0]) / 255.0,
	                    static_cast<double>(buffer[1]) / 255.0,
	                    static_cast<double>(buffer[2]) / 255.0
	                );
	                m_img.push_back(color);
	            }
	        } else {
	            std::cout << filename << " is not a P6 file." << std::endl;
	            exit(1);
	        }
	        
	        in.close();
	    }

	    void writeImpl(const std::string& filename) {
	        Pixel color;
	        std::string fname = setSuffix(filename, ".ppm");

	        std::ofstream out(fname, std::ios_base::out | std::ios_base::binary);
	        if (!out.is_open()) {
	            std::cerr << "Could not open " << fname << " for writing." << std::endl;
	            exit(1);
	        }

	        out << "P6\n" << m_width << ' ' << m_height << "\n255\n";

	        for (int i = 0; i < m_width * m_height; ++i) {
	            color = m_img[i];
	            out << static_cast<char>(std::get<0>(color) * 255.0)
	                << static_cast<char>(std::get<1>(color) * 255.0)
	                << static_cast<char>(std::get<2>(color) * 255.0);
	        }

	        out.close();
	    }

		void getAllYs(std::vector<int> &ys, std::vector<Point> &Coords) {
		    for (auto& c : Coords) {
		        ys.push_back(std::get<1>(c));
		    }
		    std::sort(ys.begin(), ys.end());
		    ys.erase(std::unique(ys.begin(), ys.end()), ys.end());
		}

		void getLineCoords(const Coord& line, std::vector<Point>& Coords) {
		    auto [x1, y1, x2, y2] = line;
		    int dx = x2 - x1;
		    int dy = y2 - y1;
		    int dx1 = std::abs(dx);
		    int dy1 = std::abs(dy);

		    if (dx == 0) {
		        if (y2 < y1) std::swap(y1, y2);
		        for (int y = y1; y <= y2; ++y) {
		            Coords.emplace_back(x1, y);
		        }
		        return;
		    }

		    if (dy == 0) {
		        if (x2 < x1) std::swap(x1, x2);
		        for (int x = x1; x <= x2; ++x) {
		            Coords.emplace_back(x, y1);
		        }
		        return;
		    }

		    int px = 2 * dy1 - dx1;
		    int py = 2 * dx1 - dy1;
		    int x, y, end;

		    if (dy1 <= dx1) {
		        if (dx >= 0) {
		            x = x1;
		            y = y1;
		            end = x2;
		        } else {
		            x = x2;
		            y = y2;
		            end = x1;
		        }
		        Coords.emplace_back(x, y);
		        for (int i = 0; x < end; ++i) {
		            ++x;
		            if (px < 0) {
		                px += 2 * dy1;
		            } else {
		                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) ++y;
		                else --y;
		                px += 2 * (dy1 - dx1);
		            }
		            Coords.emplace_back(x, y);
		        }
		    } else {
		        if (dy >= 0) {
		            x = x1;
		            y = y1;
		            end = y2;
		        } else {
		            x = x2;
		            y = y2;
		            end = y1;
		        }
		        Coords.emplace_back(x, y);
		        for (int i = 0; y < end; ++i) {
		            ++y;
		            if (py <= 0) {
		                py += 2 * dx1;
		            } else {
		                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) ++x;
		                else --x;
		                py += 2 * (dx1 - dy1);
		            }
		            Coords.emplace_back(x, y);
		        }
		    }
		}
		
		bool isInsideRectangle(int x, int y, int w, int h) {
		    return x >= -w / 2 && x <= w / 2 && y >= -h / 2 && y <= h / 2;
		}

		std::string setSuffix(const std::string& pathFilename, const std::string& suffix) {
		    if (getSuffix(pathFilename) != suffix && !suffix.empty()) {
		        std::filesystem::path filePath(pathFilename);
		        filePath.replace_extension(suffix);
		        return filePath.string();
		    }
		    return pathFilename;
		}

		std::string getSuffix(const std::string& pathFilename) {
		    std::filesystem::path filePath(pathFilename);
		    if (filePath.has_extension()) {
		        return filePath.extension().string();
		    }
		    return "";
		}

		std::vector<Pixel> m_img;
        int m_width = 0;
        int m_height = 0;
	};
} // namespace ppm
