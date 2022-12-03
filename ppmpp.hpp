// MIT No Attribution
// 
// ppmpp.hpp - A header-only class to draw/read/write 2D graphics using only the standard library.
// Version 1.0 release (3:rd of December 2022).
// Copyright (c) 2022-2023 Håkan Blomqvist
// 
// For more information:
// https://leanpub.com/2ddrawingwithc17usingthestandardlibraryonly
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

#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>

namespace ppm
{
	// Typedef
	typedef std::tuple<float, float, float> pixel;
	typedef std::tuple<int, int, int, int> coord;
	typedef std::tuple<int, int> point;

	class image
	{
	public:
		// Constructs an empty image.
		image() {}

		// Constructs an empty image of specified width and height.
		image(int width, int height) {resize(width, height);}

		// Constructs an image reading data from specified file
		explicit image(std::string const& filename)
		{
			read(filename);
		}

		// Resizes the image pixel buffer.
		void resize(int width, int height)
		{
			m_img.clear();
			m_img.resize(width*height);
			set_width(width);
			set_height(height);
		}

		void set_width(int width)
		{
			m_width=width;
		}

		void set_height(int height)
		{
			m_height=height;
		}

		int get_width()
		{
			return m_width;
		}

		int get_height()
		{
			return m_height;
		}

		void set_pixel(int x, int y, pixel px)
		{
			m_img[x+m_width*y]=px;
		}

		pixel get_pixel(int x, int y)
		{
			return m_img[x+m_width*y];
		}

		void set_all_pixels(pixel px)
		{
			for (int y=0;y<m_height;++y)
			{
				for (int x=0;x<m_width;++x)
				{
					m_img[x+m_width*y]=px;
				}
			}
		}

		// Drawline
		void draw_line(coord &coords, pixel &linecolor)
		{
			coord coords2;
			if (clipline(coords,coords2)) {
				drawline(coords2,linecolor);
			}
		}

		// Get angled line
		void get_angled_line(coord &coords, point &center, double degrees, int length)
		{
			double angle = degrees * (M_PI / 180);
			coords=std::make_tuple(std::get<0>(center),std::get<1>(center),int(double(std::get<0>(center)) + cos(angle)*double(length)),int(double(std::get<1>(center)) + sin(angle)*double(length)));
		}

		// Draw Curves (requires C++20)
#if __cplusplus >= 202002L
		void draw_bezier_quadratic(point pt0, point pt1, point pt2, int split, pixel beziercolor)
		{
			std::vector<std::tuple<int,int>> xy;
			quadraticbeziercurves(pt0,pt1,pt2,split,xy);
			// drawxydots(xy,beziercolor);
			drawxyline(xy,beziercolor,pt2);
		}

		void draw_bezier_cubic(point pt0, point pt1, point pt2, point pt3, int split, pixel beziercolor)
		{
			std::vector<std::tuple<int,int>> xy;
			cubicbeziercurves(pt0,pt1,pt2,pt3,split,xy);
			drawxyline(xy,beziercolor,pt3);
		}
#endif

		// Draw rectangles
		void draw_rectangle(point &xy, point &wh, pixel &rectanglecolor)
		{
			drawrect(xy,wh,rectanglecolor);
		}

		void draw_filled_rectangle(point &xy, point &wh, pixel &rectanglecolor)
		{
			drawfilledrect(xy,wh,rectanglecolor);
		}

		// Draw Circles
		void draw_circle(point &xy, int radius, pixel &circlecolor)
		{
			drawcircle(xy,radius,circlecolor);
		}

		void draw_filled_circle(point &xy, int radius, pixel &circlecolor)
		{
			drawfilledcircle(xy,radius,circlecolor);
		}

		// Draw Wedge
		void draw_wedge(point center,int radius,int start_angle,int end_angle,pixel &wedgecolor)
		{
			drawwedge(center,radius,start_angle,end_angle,wedgecolor);
		}

		void draw_filled_wedge(point center,int radius,int start_angle,int end_angle,pixel &wedgecolor)
		{
			drawfilledwedge(center,radius,start_angle,end_angle,wedgecolor);
		}

		// Draw Triangles
		void draw_triangle(point pt1, point pt2, point pt3, pixel &trianglecolor)
		{
			drawtriangle(pt1,pt2,pt3,trianglecolor);
		}

		void draw_filled_triangle(point pt1,point pt2,point pt3,pixel &color)
		{
			drawfilledtriangle(pt1,pt2,pt3,color);
		}

		// Blend Colors
		pixel blend_colors(pixel &colorbackground, pixel &colorforeground, float alpha)
		{
			float r=0.0f; float g=0.0f; float b=0.0f;
			r = (std::get<0>(colorforeground) * alpha) + (std::get<0>(colorbackground) * (1.0 - alpha));
			g = (std::get<1>(colorforeground) * alpha) + (std::get<1>(colorbackground) * (1.0 - alpha));
			b = (std::get<2>(colorforeground) * alpha) + (std::get<2>(colorbackground) * (1.0 - alpha));
		
			return std::make_tuple(r,g,b);
		}

		// Read PPM Image
		void read(std::string const& filename)
		{
			std::string magic;
			int max;
			uint8_t buffer[3];
			pixel color;

			std::ifstream in(filename, std::ifstream::binary);

			if (!in.is_open()) 
			{
				std::cout << "Can't open " << filename << std::endl;
				exit(1);
			}

			in >> magic;
			in.seekg(1, in.cur);
			char c;
			in.get(c);
			if (c == '#')
			{
				// We got comments in the PPM image and skip the comments
				while (c != '\n')
				{
					in.get(c);
				}
			}
			else
			{
				in.seekg(-1, in.cur);
			}

			in >> m_width >> m_height >> max;

			if (max != 255) {
				std::cout << "Not 8 bit per rgb color." << std::endl;
				exit(1);
			}

			if (magic == "P6")
			{
				// Move curser once to skip '\n'
				in.seekg(1, in.cur);

				m_img.clear();

				for (int i = 0; i < m_width * m_height; ++i)
				{
					in.read(reinterpret_cast<char *>(buffer), 3);
					color=std::make_tuple(float(float(buffer[0])/255.0),float(float(buffer[1])/255.0),float(float(buffer[2])/255.0));
					m_img.push_back(color);
				}
			} else {
				std::cout << filename << " is not a P6 file." << std::endl;
				exit(1);
			}

			in.close();
		}

		// Write PPM image.
		void write(std::string const& filename)
		{
			pixel color;
			std::ofstream out(filename, std::ios_base::out | std::ios_base::binary);
			out << "P6" << std::endl << m_width << ' ' << m_height << std::endl << "255" << std::endl;

			for (int i=0;i<(m_width*m_height);++i)
			{
				color=m_img[i];
				out << char(std::get<0>(color)*255.0f) << char(std::get<1>(color)*255.0f) << char(std::get<2>(color)*255.0f);
			}
			out.close();
		}

	protected:
		int getindex(int x, int y)
		{
			return x+m_width*y;
		}

		void drawline(coord &coords, pixel &color)
		{
			int x1=std::get<0>(coords); int y1=std::get<1>(coords); int x2=std::get<2>(coords); int y2=std::get<3>(coords);
			int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
			dx = x2 - x1; dy = y2 - y1;
			if (dx == 0)
			{
				if (y2 < y1) std::swap(y1, y2);
				for (y = y1; y <= y2; y++)
					m_img[getindex(x1,y)]=color;
				return;
			}
			if (dy == 0)
			{
				if (x2 < x1) std::swap(x1, x2);
				for (x = x1; x <= x2; x++)
					m_img[getindex(x,y1)]=color;
				return;
			}
			dx1 = abs(dx); dy1 = abs(dy);
			px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
			if (dy1 <= dx1)
			{
				if (dx >= 0)
				{
					x = x1; y = y1; xe = x2;
				}
				else
				{
					x = x2; y = y2; xe = x1;
				}
				m_img[getindex(x,y)]=color;
				for (i = 0; x<xe; i++)
				{
					x = x + 1;
					if (px<0)
						px = px + 2 * dy1;
					else
					{
						if ((dx<0 && dy<0) || (dx>0 && dy>0)) y = y + 1; else y = y - 1;
						px = px + 2 * (dy1 - dx1);
					}
					m_img[getindex(x,y)]=color;
				}
			}
			else
			{
				if (dy >= 0)
				{
					x = x1; y = y1; ye = y2;
				}
				else
				{
					x = x2; y = y2; ye = y1;
				}
				m_img[getindex(x,y)]=color;
				for (i = 0; y<ye; i++)
				{
					y = y + 1;
					if (py <= 0)
						py = py + 2 * dx1;
					else
					{
						if ((dx<0 && dy<0) || (dx>0 && dy>0)) x = x + 1; else x = x - 1;
						py = py + 2 * (dx1 - dy1);
					}
					m_img[getindex(x,y)]=color;
				}
			}
		}

		int findregion(int x, int y, int width, int height)
		{
			int code=0;
			if(y >= height)
			code |= 1; //top
			else if(y < 0)
			code |= 2; //bottom
			if(x >= width)
			code |= 4; //right
			else if (x < 0)
			code |= 8; //left
			return(code);
		}

		bool clipline(coord &coords1, coord &coords2)
		{
			int x1=std::get<0>(coords1); int y1=std::get<1>(coords1); int x2=std::get<2>(coords1); int y2=std::get<3>(coords1);
			int x3=0; int y3=0; int x4=0; int y4=0;
			int code1=0, code2=0, codeout=0;
			bool accept = 0, done=0;
			code1 = findregion(x1, y1, m_width, m_height); //the region outcodes for the endpoints
			code2 = findregion(x2, y2, m_width, m_height);
			do //In theory, this can never end up in an infinite loop, it'll always come in one of the trivial cases eventually
			{
				if(!(code1 | code2)) accept = done = 1;  //accept because both endpoints are in screen or on the border, trivial accept
				else if(code1 & code2) done = 1; //the line isn't visible on screen, trivial reject
				else  //if no trivial reject or accept, continue the loop
				{
					int x, y;
					codeout = code1 ? code1 : code2;
					if(codeout & 1) //top
					{
						x = x1 + (x2 - x1) * (m_height - y1) / (y2 - y1);
						y = m_height - 1;
					}
					else if(codeout & 2) //bottom
					{
						x = x1 + (x2 - x1) * -y1 / (y2 - y1);
						y = 0;
					}
					else if(codeout & 4) //right
					{
						y = y1 + (y2 - y1) * (m_width - x1) / (x2 - x1);
						x = m_width - 1;
					}
					else //left
					{
						y = y1 + (y2 - y1) * -x1 / (x2 - x1);
						x = 0;
					}
					if(codeout == code1) //first endpoint was clipped
					{
						x1 = x; y1 = y;
						code1 = findregion(x1, y1, m_width, m_height);
					}
					else //second endpoint was clipped
					{
						x2 = x; y2 = y;
						code2 = findregion(x2, y2, m_width, m_height);
					}
				}
			}
			while(done == 0);
			if(accept)
			{
				x3 = x1;
				x4 = x2;
				y3 = y1;
				y4 = y2;
				coords2=std::make_tuple(x3,y3,x4,y4);
				return 1;
			}
			else
			{
				x3 = x4 = y3 = y4 = 0;
				coords2=std::make_tuple(x3,y3,x4,y4);
				return 0;
			}
		}

#if __cplusplus >= 202002L
		void quadraticbeziercurves(point pt0, point pt1, point pt2, int split, std::vector<std::tuple<int,int>>& xy)
		{
			double t=0.0;
			double delta=(double(1)/double(split));
			int x=0;
			int y=0;
			int x1=0;
			int y1=0;
			int x2=0;
			int y2=0;
			for (int i=0;i<split;++i)
			{
				x1 = std::lerp(std::get<0>(pt0),std::get<0>(pt1),t);
				y1 = std::lerp(std::get<1>(pt0),std::get<1>(pt1),t);
				x2 = std::lerp(std::get<0>(pt1),std::get<0>(pt2),t);
				y2 = std::lerp(std::get<1>(pt1),std::get<1>(pt2),t);
				x = std::lerp(x1,x2,t);
				y = std::lerp(y1,y2,t);
				xy.push_back(std::make_tuple(x,y));
				t+=delta;
			}
		}

		void cubicbeziercurves(point pt0, point pt1, point pt2, point pt3, int split, std::vector<std::tuple<int,int>>& xy)
		{
			float t=0.0;
			float delta=(float(1)/float(split));
			int x=0;
			int y=0;
			int x1=0;
			int y1=0;
			int x2=0;
			int y2=0;
			int x3=0;
			int y3=0;
			int x4=0;
			int y4=0;
			int xx1=0;
			int yy1=0;
			int xx2=0;
			int yy2=0;
			for (int i=0;i<split;++i)
			{
				x1 = std::lerp(std::get<0>(pt0),std::get<0>(pt1),t);
				y1 = std::lerp(std::get<1>(pt0),std::get<1>(pt1),t);
				x2 = std::lerp(std::get<0>(pt1),std::get<0>(pt2),t);
				y2 = std::lerp(std::get<1>(pt1),std::get<1>(pt2),t);
		
				x3 = std::lerp(std::get<0>(pt1),std::get<0>(pt2),t);
				y3 = std::lerp(std::get<1>(pt1),std::get<1>(pt2),t);
				x4 = std::lerp(std::get<0>(pt2),std::get<0>(pt3),t);
				y4 = std::lerp(std::get<1>(pt2),std::get<1>(pt3),t);
		
				xx1 = std::lerp(x1,x2,t);
				yy1 = std::lerp(y1,y2,t);
				xx2 = std::lerp(x3,x4,t);
				yy2 = std::lerp(y3,y4,t);
		
				x = std::lerp(xx1,xx2,t);
				y = std::lerp(yy1,yy2,t);
				xy.push_back(std::make_tuple(x,y));
				t+=delta;
			}
		}
#endif

		void drawxyline(std::vector<point> &xy, pixel &color, point stop)
		{
			point temp=xy[0];
			point store;
			store=std::make_tuple(std::get<0>(stop),std::get<1>(stop));
			xy.push_back(store);
			std::vector<coord> xyxy;
			for (int i=1;i<xy.size();++i)
			{
				store=xy[i];
				xyxy.push_back(std::make_tuple(std::get<0>(temp),std::get<1>(temp),std::get<0>(store),std::get<1>(store)));
				temp=xy[i];
			}
		
			for (auto& li : xyxy)
			{
				draw_line(li,color);
			}
		}

		void drawxydots(std::vector<point> &xy, pixel &color)
		{
			for (auto& pt : xy)
			{
				m_img[getindex(std::get<0>(pt),std::get<1>(pt))]=color;
			}
		}

		void drawrect(point &xy, point &wh, pixel &color)
		{
			int x=std::get<0>(xy);int y=std::get<1>(xy);int w=std::get<0>(wh);int h=std::get<1>(wh);
			coord coords=std::make_tuple(x, y, x+w, y);
			draw_line(coords, color);
			coords=std::make_tuple(x+w, y, x+w, y+h);
			draw_line(coords, color);
			coords=std::make_tuple(x+w, y+h, x, y+h);
			draw_line(coords, color);
			coords=std::make_tuple(x, y+h, x, y);
			draw_line(coords, color);
		}

		void drawfilledrect(point &xy, point &wh, pixel &color)
		{
			int x2 = std::get<0>(xy) + std::get<0>(wh);
			int y2 = std::get<1>(xy) + std::get<1>(wh);
		
			coord coords=std::make_tuple(0,0,0,0);
			for (int i = std::get<1>(xy); i < y2; ++i)
			{
				coords=std::make_tuple(std::get<0>(xy),i,x2,i);
				draw_line(coords, color);
			}
		}

		void drawcircle(point &xy, int radius, pixel &color)
		{
			int x0 = 0;
			int y0 = radius;
			int d = 3 - 2 * radius;
			int xx=0;
			int yy=0;
			int x=std::get<0>(xy);
			int y=std::get<1>(xy);
			coord coords=std::make_tuple(0,0,0,0);
			unsigned char mask=255;
			if (!radius) return;
		
			while (y0 >= x0)
			{
				if (mask & 0x01) {xx=x + x0;yy=y - y0;coords=std::make_tuple(xx,yy,xx,yy);draw_line(coords, color);}
				if (mask & 0x02) {xx=x + y0;yy=y - x0;coords=std::make_tuple(xx,yy,xx,yy);draw_line(coords, color);}
				if (mask & 0x04) {xx=x + y0;yy=y + x0;coords=std::make_tuple(xx,yy,xx,yy);draw_line(coords, color);}
				if (mask & 0x08) {xx=x + x0;yy=y + y0;coords=std::make_tuple(xx,yy,xx,yy);draw_line(coords, color);}
				if (mask & 0x10) {xx=x - x0;yy=y + y0;coords=std::make_tuple(xx,yy,xx,yy);draw_line(coords, color);}
				if (mask & 0x20) {xx=x - y0;yy=y + x0;coords=std::make_tuple(xx,yy,xx,yy);draw_line(coords, color);}
				if (mask & 0x40) {xx=x - y0;yy=y - x0;coords=std::make_tuple(xx,yy,xx,yy);draw_line(coords, color);}
				if (mask & 0x80) {xx=x - x0;yy=y - y0;coords=std::make_tuple(xx,yy,xx,yy);draw_line(coords, color);}
				if (d < 0) d += 4 * x0++ + 6;
				else d += 4 * (x0++ - y0--) + 10;
			}
		}

		void drawfilledcircle(point &xy, int radius, pixel &color)
		{
			int x0 = 0;
			int y0 = radius;
			int d = 3 - 2 * radius;
			int x=std::get<0>(xy);
			int y=std::get<1>(xy);
			coord coords=std::make_tuple(0,0,0,0);
			if (!radius) return;
		
			while (y0 >= x0)
			{
				coords=std::make_tuple(x-x0,y-y0,x+x0,y-y0);
				draw_line(coords, color);
				coords=std::make_tuple(x-y0,y-x0,x+y0,y-x0);
				draw_line(coords, color);
				coords=std::make_tuple(x-x0,y+y0,x+x0,y+y0);
				draw_line(coords, color);
				coords=std::make_tuple(x-y0,y+x0,x+y0,y+x0);
				draw_line(coords, color);
				if (d < 0) d += 4 * x0++ + 6;
				else d += 4 * (x0++ - y0--) + 10;
			}
		}

		void getallys(std::vector<int> &ys,std::vector<point> &coords)
		{
			for (auto& c : coords) {
				ys.push_back(std::get<1>(c));
			}
		
			sort(ys.begin(), ys.end());
			ys.erase( unique( ys.begin(), ys.end() ), ys.end() );
		}

		void getlinecoords(coord line,std::vector<point> &coords)
		{
			int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
			int x1=std::get<0>(line);int y1=std::get<1>(line);int x2=std::get<2>(line);int y2=std::get<3>(line);
			dx = x2 - x1; dy = y2 - y1;
			if (dx == 0)
			{
				if (y2 < y1) std::swap(y1, y2);
				for (y = y1; y <= y2; y++)
					coords.push_back(std::make_tuple(x1,y));
				return;
			}
			if (dy == 0)
			{
				if (x2 < x1) std::swap(x1, x2);
				for (x = x1; x <= x2; x++)
					coords.push_back(std::make_tuple(x,y1));
				return;
			}
			dx1 = abs(dx); dy1 = abs(dy);
			px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
			if (dy1 <= dx1)
			{
				if (dx >= 0)
				{
					x = x1; y = y1; xe = x2;
				}
				else
				{
					x = x2; y = y2; xe = x1;
				}
				coords.push_back(std::make_tuple(x,y));
				for (i = 0; x<xe; i++)
				{
					x = x + 1;
					if (px<0)
						px = px + 2 * dy1;
					else
					{
						if ((dx<0 && dy<0) || (dx>0 && dy>0)) y = y + 1; else y = y - 1;
						px = px + 2 * (dy1 - dx1);
					}
					coords.push_back(std::make_tuple(x,y));
				}
			}
			else
			{
				if (dy >= 0)
				{
					x = x1; y = y1; ye = y2;
				}
				else
				{
					x = x2; y = y2; ye = y1;
				}
				coords.push_back(std::make_tuple(x,y));
				for (i = 0; y<ye; i++)
				{
					y = y + 1;
					if (py <= 0)
						py = py + 2 * dx1;
					else
					{
						if ((dx<0 && dy<0) || (dx>0 && dy>0)) x = x + 1; else x = x - 1;
						py = py + 2 * (dx1 - dy1);
					}
					coords.push_back(std::make_tuple(x,y));
				}
			}
		}

		void drawwedge(point center,int radius,int start_angle,int end_angle,pixel &color)
		{
			std::vector<point> coords;
		
			float angle=(((start_angle<=end_angle)?start_angle:end_angle)*(M_PI/180));
			float range=(((end_angle>start_angle)?end_angle:start_angle)*(M_PI/180));
			float x=(radius*cos(angle));
			float y=(radius*sin(angle));
			do
			{
				coords.push_back(std::make_tuple((int)(std::get<0>(center)+x+0.5),(int)(std::get<1>(center)-y+0.5)));
				angle+=0.001;
				x=(radius*cos(angle));
				y=(radius*sin(angle));
			}
			while(angle<=range);
		
			point co1=coords.front();
			point co2=coords.back();
		
			coord line1=std::make_tuple(std::get<0>(center),std::get<1>(center),std::get<0>(co1),std::get<1>(co1));
			coord line2=std::make_tuple(std::get<0>(center),std::get<1>(center),std::get<0>(co2),std::get<1>(co2));

			getlinecoords(line1,coords);
			getlinecoords(line2,coords);
		
			for (auto & e : coords)
			{
				line1=std::make_tuple(std::get<0>(e),std::get<1>(e),std::get<0>(e),std::get<1>(e));
				draw_line(line1,color);
			}
		}

		void drawfilledwedge(point center,int radius,int start_angle,int end_angle,pixel &color)
		{
			std::vector<point> coords;
		
			float angle=(((start_angle<=end_angle)?start_angle:end_angle)*(M_PI/180));
			float range=(((end_angle>start_angle)?end_angle:start_angle)*(M_PI/180));
			float x=(radius*cos(angle));
			float y=(radius*sin(angle));
			do
			{
				coords.push_back(std::make_tuple((int)(std::get<0>(center)+x+0.5),(int)(std::get<1>(center)-y+0.5)));
				angle+=0.001;
				x=(radius*cos(angle));
				y=(radius*sin(angle));
			}
			while(angle<=range);
		
			point co1=coords.front();
			point co2=coords.back();

			coord line1=std::make_tuple(std::get<0>(center),std::get<1>(center),std::get<0>(co1),std::get<1>(co1));
			coord line2=std::make_tuple(std::get<0>(center),std::get<1>(center),std::get<0>(co2),std::get<1>(co2));
		
			getlinecoords(line1,coords);
			getlinecoords(line2,coords);
		
			std::vector<int> ys;
			std::vector<int> xs;
			getallys(ys,coords);
			std::vector<coord> lines;
		
			for (int search=0;search<=ys.size();++search)
			{
				for (auto& c : coords) {
					if (std::get<1>(c) == ys[search]) {
						xs.push_back(std::get<0>(c));
					}
				}
				sort(xs.begin(), xs.end());
				lines.push_back(std::make_tuple(xs.front(),ys[search],xs.back(),ys[search]));
				xs.clear();
			}
		
			coord cd=std::make_tuple(0,0,0,0);
			for (auto& l : lines) {
				draw_line(l,color);
			}
		}

		void drawtriangle(point pt1, point pt2, point pt3, pixel &color)
		{
			coord coords=std::make_tuple(std::get<0>(pt1),std::get<1>(pt1),std::get<0>(pt2),std::get<1>(pt2));
			draw_line(coords,color);
			coords=std::make_tuple(std::get<0>(pt2),std::get<1>(pt2),std::get<0>(pt3),std::get<1>(pt3));
			draw_line(coords,color);
			coords=std::make_tuple(std::get<0>(pt3),std::get<1>(pt3),std::get<0>(pt1),std::get<1>(pt1));
			draw_line(coords,color);
		}

		void drawfilledtriangle(point pt1,point pt2,point pt3,pixel &color)
		{
			int x1=std::get<0>(pt1); int y1=std::get<1>(pt1); int x2=std::get<0>(pt2); int y2=std::get<1>(pt2); int x3=std::get<0>(pt3); int y3=std::get<1>(pt3);
			coord coords=std::make_tuple(0,0,0,0);
			auto SWAP = [](int &x, int &y) { int t = x; x = y; y = t; };
		
			int t1x, t2x, y, minx, maxx, t1xp, t2xp;
			bool changed1 = false;
			bool changed2 = false;
			int signx1, signx2, dx1, dy1, dx2, dy2;
			int e1, e2;
			if (y1>y2) { SWAP(y1, y2); SWAP(x1, x2); }
			if (y1>y3) { SWAP(y1, y3); SWAP(x1, x3); }
			if (y2>y3) { SWAP(y2, y3); SWAP(x2, x3); }
		
			t1x = t2x = x1; y = y1;
			dx1 = (int)(x2 - x1); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
			else signx1 = 1;
			dy1 = (int)(y2 - y1);
		
			dx2 = (int)(x3 - x1); if (dx2<0) { dx2 = -dx2; signx2 = -1; }
			else signx2 = 1;
			dy2 = (int)(y3 - y1);
		
			if (dy1 > dx1) {
				SWAP(dx1, dy1);
				changed1 = true;
			}
			if (dy2 > dx2) {
				SWAP(dy2, dx2);
				changed2 = true;
			}
		
			e2 = (int)(dx2 >> 1);
			if (y1 == y2) goto next;
			e1 = (int)(dx1 >> 1);
		
			for (int i = 0; i < dx1;) {
				t1xp = 0; t2xp = 0;
				if (t1x<t2x) { minx = t1x; maxx = t2x; }
				else { minx = t2x; maxx = t1x; }
				while (i<dx1) {
					i++;
					e1 += dy1;
					while (e1 >= dx1) {
						e1 -= dx1;
						if (changed1) t1xp = signx1;
						else          goto next1;
					}
					if (changed1) break;
					else t1x += signx1;
				}
			next1:
				while (1) {
					e2 += dy2;
					while (e2 >= dx2) {
						e2 -= dx2;
						if (changed2) t2xp = signx2;
						else          goto next2;
					}
					if (changed2)     break;
					else              t2x += signx2;
				}
			next2:
				if (minx>t1x) minx = t1x; 
				if (minx>t2x) minx = t2x;
				if (maxx<t1x) maxx = t1x; 
				if (maxx<t2x) maxx = t2x;
				coords=std::make_tuple(minx,y,maxx,y);
				draw_line(coords,color);
				if (!changed1) t1x += signx1;
				t1x += t1xp;
				if (!changed2) t2x += signx2;
				t2x += t2xp;
				y += 1;
				if (y == y2) break;
		
				}
			next:
			dx1 = (int)(x3 - x2); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
			else signx1 = 1;
			dy1 = (int)(y3 - y2);
			t1x = x2;
		
			if (dy1 > dx1) {
				SWAP(dy1, dx1);
				changed1 = true;
			}
			else changed1 = false;
		
			e1 = (int)(dx1 >> 1);
		
			for (int i = 0; i <= dx1; i++) {
				t1xp = 0; t2xp = 0;
				if (t1x<t2x) { minx = t1x; maxx = t2x; }
				else { minx = t2x; maxx = t1x; }
				while (i<dx1) {
					e1 += dy1;
					while (e1 >= dx1) {
						e1 -= dx1;
						if (changed1) { t1xp = signx1; break; }
						else          goto next3;
					}
					if (changed1) break;
					else   	   	  t1x += signx1;
					if (i<dx1) i++;
				}
			next3:
				while (t2x != x3) {
					e2 += dy2;
					while (e2 >= dx2) {
						e2 -= dx2;
						if (changed2) t2xp = signx2;
						else          goto next4;
					}
					if (changed2)     break;
					else              t2x += signx2;
				}
			next4:
		
				if (minx>t1x) minx = t1x;
				if (minx>t2x) minx = t2x;
				if (maxx<t1x) maxx = t1x; 
				if (maxx<t2x) maxx = t2x;
				coords=std::make_tuple(minx,y,maxx,y);
				draw_line(coords,color);
				if (!changed1) t1x += signx1;
				t1x += t1xp;
				if (!changed2) t2x += signx2;
				t2x += t2xp;
				y += 1;
				if (y>y3) return;
			}
		}

		std::vector<pixel> m_img;
		int m_width;
		int m_height;
	};

} // namespace ppm