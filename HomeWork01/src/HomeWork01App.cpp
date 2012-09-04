/**
* @file HomeWork01.cpp
* 
* @author Cary Willard
* @date 2012-08-28
* 
* @note This file is (c) 2012.  It is licensed under the
* CC gv 3.0 license (http://creativecommons.org/licenses/by/3.0/),
* which means you are free to use, share, and remix is as long as you
* give attribution.  Commercial uses are allowed.
* 
* This project was written for credit in Dr. Brinkman's CSE 274 class at
* Miami University during the fall semester of 2012.
*
* This project was heavily influenced by Dr. Brinkman's HW01 project:
* https://github.com/brinkmwj/HW01
* 
* This project fulfills the following requirements for homework 1:
* 
*
*/


#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class HomeWork01App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void presets(Settings* settings);

private:
	static const int kWinWidth = 900;
	static const int kWinHeight = 900;
	static const int kSurfaceSize=1024;
	Surface* mySurface_;
	uint8_t* myPixels;
	void drawCircle(uint8_t* pixels,int x, int y, int r, Color8u c);
	void drawSquare(uint8_t* pixels,int x, int y, int r, Color8u c);
	void drawDiamond(uint8_t* pixels,int x, int y, int r, Color8u c);
	void drawRectangle(uint8_t* pixels,int center_x, int center_y, int rx, int ry, Color8u c);
	void blackOutWindow(uint8_t* pixels);
	void gradient(uint8_t* pixels, Color8u c);
	void tint(uint8_t* pixels, Color8u t);
	void drawLine(uint8_t* pixels, int x0, int y0, int x1, int y1, Color8u c);
	void drawRays(uint8_t* pixels,int center_x, int center_y, int r, int degree_0, int degree_1, Color8u c);
};

void HomeWork01App::presets(Settings* settings) {
	(*settings).setWindowSize(kWinWidth,kWinHeight);
	(*settings).setResizable(false);
}

void HomeWork01App::blackOutWindow(uint8_t* pixels)
{
	Color8u c = Color8u(0,0,0);

	for(int y = 0; y < kSurfaceSize;y++) {
		for(int x = 0; x < kSurfaceSize;x++) {

			int offset = 3*(x + y*kSurfaceSize);
			pixels[offset] = c.r; //Red
			pixels[offset+1] = c.g; //Green
			pixels[offset+2] = c.b; // Blue

		}
	}

}

/*
Right now, this is almost a verbatim implementation of Dr. Brinkman's drawRings method from his HW01 project.
I am analyzing it so that I can understand it well enough to improvise later.
This is why I sometimes used variable names that are unwieldy.
In short, I have no intention of leaving this method in this form.
*/
void HomeWork01App::drawCircle(uint8_t* pixels,int center_x, int center_y, int r, Color8u c)
{
	if(r <= 0) return;
	int count = 0;
	int colorAdd = 0;
	bool flag = false;
	int ripple = 3;

	for(int y=center_y-r; y<=center_y+r; y++){
		for(int x=center_x-r; x<=center_x+r; x++){
			// Bounds test, to stop out-of-bounds memory access
			if(y < 0 || x < 0 || x >= kWinWidth || y >= kWinHeight) continue;
			count++;
			if(colorAdd >= 255) {
				flag = true;
			}
			if(colorAdd < 0) {
				flag = false;
			}
			if(flag) {
				colorAdd--;
			} else {
				colorAdd++;
			}

			int distanceFromCenter = (int)sqrt((double)((x-center_x)*(x-center_x) + (y-center_y)*(y-center_y)));
			if(distanceFromCenter <= r){ 
				if((distanceFromCenter/7)%2 == 1 ){  //...(distanceFromCenter/7)
					ripple = ripple + 2;
					if(ripple >= 15) {
						ripple = 3;
					}
					
					if(count = 4) {
					count = 0;
					c.r = colorAdd;
					c.g = colorAdd;
					c.b = colorAdd;
				}

						int offset = 3*(x + y*kSurfaceSize);
						pixels[offset] = pixels[offset]/2 + c.r/2; //Red
						pixels[offset+1] = pixels[offset+1]/2 + c.g/2; //Green
						pixels[offset+2] = pixels[offset+2]/2 + c.b/2; // Blue
					}
				}
			}	
		}
	}


void HomeWork01App::drawSquare(uint8_t* pixels,int center_x, int center_y, int r, Color8u c)
{
	if(r <= 0) return;

	for(int y=center_y-r;y<=center_y+r; y++){
		for(int x=center_x-r;x<=center_x+r; x++){
			
			if(y < 0 || x < 0 || x >= kWinWidth || y >= kWinHeight) continue;

			if((x == center_x-r)||(x == center_x+r)||(y == center_y-r)||(y == center_y+r)) {

			int indeces = 3*(x + y*kSurfaceSize); // Why not (y+x*kSurfaceSize)?
			pixels[indeces] = pixels[indeces]+c.r;
			pixels[indeces] = pixels[indeces+1]+c.g;
			pixels[indeces] = pixels[indeces+2]+c.b;
			}
	}
	}
}

// Let's rotate that square by pi/4
void HomeWork01App::drawDiamond(uint8_t* pixels,int center_x, int center_y, int r, Color8u c)
{

	int yRad=(int)sqrt((double)(r*r)+(double)(r*r));
	int xRad=0;
	if(r <= 0) return;

	for(int y=center_y-yRad;y<=center_y+yRad; y++){
		//for(int x=center_x;x<=center_x+xRad; x++){
			int xLeft=center_x-xRad;
			int xRight=center_x+xRad;

			if(y < 0 || xLeft < 0 || xRight < 0 || xLeft >= kWinWidth || xRight >= kWinWidth || y >= kWinHeight) continue;

			//if((x == center_x-xRad)||(x == center_x+xRad)) {

			int indeces = 3*(xLeft + y*kSurfaceSize);
			pixels[indeces] = pixels[indeces]+c.r;
			pixels[indeces] = pixels[indeces+1]+c.g;
			pixels[indeces] = pixels[indeces+2]+c.b;

			indeces = 3*(xRight + y*kSurfaceSize);
			pixels[indeces] = pixels[indeces]+c.r;
			pixels[indeces] = pixels[indeces+1]+c.g;
			pixels[indeces] = pixels[indeces+2]+c.b;


			//}
			if(y<center_y) {
			xRad--;
			} else if(y>=center_y) {
				xRad++;
			}
		//}
	}
}

// Now we'll extend that square into a rectangle
void HomeWork01App::drawRectangle(uint8_t* pixels,int center_x, int center_y, int rx, int ry, Color8u c)
{
	if(rx <= 0) return;
	if(ry <= 0) return;

	for(int y=center_y-ry;y<=center_y+ry; y++){
		for(int x=center_x-rx;x<=center_x+rx; x++){
			
			if(y < 0 || x < 0 || x >= kWinWidth || y >= kWinHeight) continue;

			if((x == center_x-rx)||(x == center_x+rx)||(y == center_y-ry)||(y == center_y+ry)) {

			int indeces = 3*(x + y*kSurfaceSize); // Why not (y+x*kSurfaceSize)?
			pixels[indeces] = pixels[indeces]+c.r;
			pixels[indeces] = pixels[indeces+1]+c.g;
			pixels[indeces] = pixels[indeces+2]+c.b;
			}
		}
	}
}

void HomeWork01App::gradient(uint8_t* pixels, Color8u c)
{
	//Color8u c = Color8u(200,200,200);
	int count = 0;
	int colorAdd = 0;
	bool flag = false;

	for(int y=0; y<kSurfaceSize; y++) {
		for(int x=0; x<kSurfaceSize; x++) {
			count = x;

			if(count==((int)(255/kSurfaceSize))) {
				count = 0;
				
				if(colorAdd == 255) {
					flag = true;
				}
				if(colorAdd == 0) {
					flag = false;
				}

				if(flag) {
				colorAdd--;
				} else {
					colorAdd++;
				}
				c.r = colorAdd;
				c.g = colorAdd;
				c.b = colorAdd;
			}

			int offset = 3*(x + y*kSurfaceSize);
			pixels[offset] = c.r; //Red
			pixels[offset+1] = c.g; //Green
			pixels[offset+2] = c.b; // Blue

		}
	}
}

/*
This drawLine() function is adapted from Bresenham's line algorithm for drawing line segments 
as outlined in the Wikipedia article of the same name:
http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
*/
void HomeWork01App::drawLine(uint8_t* pixels, int x0, int y0, int x1, int y1, Color8u c)
{
	/*
	if(x0 > x1) {
		int temp = x0; 
		x0 = x1;
		x1 = temp;
	}
	if(y0 > y1) {
		int temp = y0; 
		y0 = y1;
		y1 = temp;
	}
	*/

	int sx;
	int sy;

	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);

	if(x0 < x1) {
		sx = 1;
	} else {
		sx = -1;
	}
	if(y0 < y1) {
		sy = 1;
	} else {
		sy = -1;
	}

	int err = dx - dy;

	for(x0; x0 <= x1;x0++) {
		for(y0;y0 <= y1; y0++) {
			// Don't plot out of bounds
			if(x0 < 0 || y0 < 0 || x1 < 0 || y1 < 0
				|| x0 > kSurfaceSize
				|| y0 > kSurfaceSize  // Possibly contains unnecessary code
				|| x1 > kSurfaceSize
				|| y1 > kSurfaceSize) continue;
			// Plot x,y
			int indeces = 3*(x0 + y0*kSurfaceSize);
			pixels[indeces] = c.r;
			pixels[indeces+1] = c.g;
			pixels[indeces+2] = c.b;

			if((x0 == x1)&&(y0 == y1)) continue;
			int e2 = 2*err;
			if(e2 > (-1*dy)) {
				err = err - dy;
				x0 = x0 + sx;
			}
			if(e2 < dx){
				err = err + dx;
				y0 = y0 + sy;
			}
		}
	}

}


void HomeWork01App::tint(uint8_t* pixels, Color8u t)
{

	for(int y=0; y<kSurfaceSize; y++) {
		for(int x=0; x<kSurfaceSize; x++) {
			int offset = 3*(x + y*kSurfaceSize);
			
			if((pixels[offset] + t.r) > 255) {
			pixels[offset] = 255;
			} else {
			pixels[offset] = pixels[offset] + t.r;
			}
			if((pixels[offset+1] + t.g) > 255) {
			pixels[offset+1] = 255;
			} else {
			pixels[offset+1] = pixels[offset+1] + t.g;
			}
			if((pixels[offset+2] + t.b) > 255) {
			pixels[offset+2] = 255;
			} else {
			pixels[offset+2] = pixels[offset+2] + t.b;
			}
		}
	}
}

void HomeWork01App::drawRays(uint8_t* pixels,int center_x, int center_y, int r, int degree_0, int degree_1, Color8u c) {

	for(degree_0; degree_0 <= degree_1;degree_0++) {

		double rad = (degree_0*((3.14159)/180.0));
		int end_x = ((int)(r*cos((rad))));
		int end_y = ((int)(r*sin(rad)));

		drawLine(pixels,center_x,center_y,end_x,end_y,c);
	}
}

void HomeWork01App::setup()
{
	// Initialize mySurface_
	mySurface_ = new Surface(kSurfaceSize,kSurfaceSize,false);
	myPixels = (*mySurface_).getData();
	blackOutWindow(myPixels);
	//gradient(myPixels, Color8u(0,0,0));
	//drawCircle(myPixels,313, 250, 100, Color8u(0,180,0));
	//drawSquare(myPixels,300, 350, 100, Color8u(180,180,180));
	//drawDiamond(myPixels,300, 100, 100, Color8u(180,255,180));
	//drawRectangle(myPixels,300, 350, 100, 200, Color8u(180,180,180));
	//tint(myPixels, Color8u(0,50,0));
	drawLine(myPixels,100,100,200,200,Color(255,255,255));
	drawLine(myPixels,100,200,200,100,Color(255,255,255));
	drawRays(myPixels,313,350,100,0,360,Color8u(255,255,255));
	
}

void HomeWork01App::mouseDown( MouseEvent event )
{
}

void HomeWork01App::update()
{
}

void HomeWork01App::draw()
{
	//gl::Texture myTexture = gl::Texture( myPixels);
	gl::clear(Color8u(0,0,0));
	gl::draw(*mySurface_);
}

CINDER_APP_BASIC( HomeWork01App, RendererGl )
