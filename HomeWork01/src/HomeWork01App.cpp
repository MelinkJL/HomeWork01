/**
* @file HomeWork01.cpp
* 
* @author Cary Willard
* @date 09-05-2012
* 
* @note This file is (c) 2012.  It is licensed under the
* CC gv 3.0 license (http://creativecommons.org/licenses/by/3.0/),
* which means you are free to use, share, and remix is as long as you
* give attribution.  Commercial uses are allowed.
* 
* Welcome to my second C++ application!
*
* This project was written for credit in Dr. Brinkman's CSE 274 class at
* Miami University during the fall semester of 2012.
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
	/*
	* mousedown changes the movement of rectangles and toggles blur
	* when the application window is clicked
	*
	* Submitted towards the "mouse interaction" and "animation" requirements
	* in 2.E
	*/
	void mouseDown( MouseEvent event );
	/*
	* Update redraws animates rectangles on the screen and
	* refreshes the blur function.
	* 
	* Submitted towards the "animation" requirement in 2.E
	*/
	void update();
	void draw();
	void presets(Settings* settings);

private:
	static const int kWinWidth = 900;
	static const int kWinHeight = 900;
	static const int kSurfaceSize=1024;
	Surface* mySurface_;
	uint8_t* myPixels;

	// For animation in update
	int disp;
	int count;
	int top;
	int bottom;
	int left;
	int right;
	bool fast;

	// Makes the Surface black
	void blackOutWindow(uint8_t* pixels);
	
         // A little bit of overkill on the comments here.  Save the details for when you actually get to the method.

	/*
	* Creates a gradient that changes from the Color8u c parameter
	* to white.  The gradient changes smoothly from the top of the
	* Surface to the bottom and peaks at white twice.
	*
	* Submitted towards the "gradient" requirement in 1.A
	*/
	void gradient(uint8_t* pixels, Color8u c);
	/*
	* Draws a filled circle with a gradient effect.
	*
	* Credit to Dr. Brinkman's HWO1 project for the idea of using
	* for loops to read through the array and an "if" statement that
	* compares by calculating the distance of each pixel using the distance formula.
	* https://github.com/brinkmwj/HW01
	* 
	* Submitted towards the "circle" requirement in 1.A
	*/
	void drawCircle(uint8_t* pixels,int x, int y, int r, Color8u c);

	/*
	* An ancestor to drawRectangle, this function draws a square around
	* a center point where the radius r is half the length of a side.
	*/
	void drawSquare(uint8_t* pixels,int x, int y, int r, Color8u c);

	/*
	* Draws an empty, square diamond on the Surface.  The r parameter is the distance
	* from the center of the diamond to any of the corners.
	* 
	* Similar to drawRectangle()
	*/
	void drawDiamond(uint8_t* pixels,int x, int y, int r, Color8u c);
	
	/*
	* Draws an empty rectangle to the screen in the supplied color.
	* x and y are the coordinates of the top left corner, height 
	* extends down from that point.
	* 
	* Submitted towards the "rectangle" requirement in 1.A
	*/
	void drawRectangle(uint8_t* pixels,int x, int y, int width, int height, Color8u c);

	/*
	* This drawLine() function is adapted from Bresenham's line algorithm for drawing line segments 
	* as outlined in the Wikipedia article of the same name:
	* http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	* 
	* That said, this function doesn't work.
	*/
	void drawLine(uint8_t* pixels, int x0, int y0, int x1, int y1, Color8u c);
	
	/*
	* This function takes a range of polar coordinates about cartesian point and translates
	* the end points of the polar "rays" into cartesian points.  Then it calls drawLine(), 
	* and plots the rays.
	*
	* I do not think this function works correctly, but drawLine() does not work either,
	* so testing is difficult.
	*/
	void drawRays(uint8_t* pixels,int center_x, int center_y, int r, int degree_0, int degree_1, Color8u c);

	/*
	* Tints the Surface by adding the RGB values of Color8u t to
	* all of the Surface pixels, and limits the Surface's RGB 
	* values to 255.
	*/
	void tint(uint8_t* pixels, Color8u t);
	/*
	* This function blurs the Surface picture by averaging the 
	* individual RGB values of 9x9 kernels sampled from the Surface itself.
	* 
	* It uses 4 nested for loops, so it can cause a significant drop in frame rate.
	* If you click on the application window, blur turns on and the rectangles move 
	* in longer steps over a wider range.  If you click again, it turns off, the 
	* framerate accelerates, and the rectangles move more smoothly through smaller ranges.
	*
	* Submitted towards the "blur" requirement in 1.B and the "mouse interaction"
	* requirement of 2.E
	*/
	void blur(uint8_t* myPixels);
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
			pixels[offset] = c.r;
			pixels[offset+1] = c.g;
			pixels[offset+2] = c.b;

		}
	}
}

void HomeWork01App::gradient(uint8_t* pixels, Color8u c)
{

	int count = 0;
	int colorAdd = 0;
	bool flag = false;

	for(int y=0; y<kSurfaceSize; y++) {
		for(int x=0; x<kSurfaceSize; x++) {
			count = x;

			if(count==((int)(255/kSurfaceSize))) {
				count = 0;
				
				if(colorAdd == 125) {
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
			pixels[offset] = c.r;
			pixels[offset+1] = c.g;
			pixels[offset+2] = c.b;

		}
	}
}

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
                          // You shouldn't need this if you set the bounds of your nested for statment correctly.
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
						pixels[offset] = pixels[offset]/2 + c.r/2;
						pixels[offset+1] = pixels[offset+1]/2 + c.g/2;
						pixels[offset+2] = pixels[offset+2]/2 + c.b/2;
				}
			}	
		}
	}

/*
* Draws an empty square.
* When commenting methods, make sure you specify what the parameters mean like so: @param pixels: The pixel array to be 
* modified.
*/
void HomeWork01App::drawSquare(uint8_t* pixels,int center_x, int center_y, int r, Color8u c)
{
	if(r <= 0) return;

	for(int y=center_y-r;y<=center_y+r; y++){
		for(int x=center_x-r;x<=center_x+r; x++){
			
                           // What's the point of this line?  Is there any way to combine it with the line below it?
			if(y < 0 || x < 0 || x >= kWinWidth || y >= kWinHeight) continue;

			if((x == center_x-r)&&(x == center_x+r)&&(y == center_y-r)&&(y == center_y+r)) {

			int indeces = 3*(x + y*kSurfaceSize); // Why not (y+x*kSurfaceSize)?
			pixels[indeces] = pixels[indeces]+c.r;
			pixels[indeces] = pixels[indeces+1]+c.g;
			pixels[indeces] = pixels[indeces+2]+c.b;
			}
	}
	}
}

/*
* Draws an empty diamond
*/
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

/*
* Draws an empty rectangle
*/
void HomeWork01App::drawRectangle(uint8_t* pixels,int x, int y, int width, int height, Color8u c)
{
	if(width <= 0) return;
	if(height <= 0) return;

	for(int ky=y-height;ky<=y+height; ky++){
		for(int kx=x-width;kx<=x+width;kx++){
			
			if(ky < 0 || kx < 0 || kx >= kWinWidth || ky >= kWinHeight) continue;

			if((kx == x-width)||(kx == x+width)||(ky == y-height)||(ky == y+height)) {

			int indeces = 3*(kx + ky*kSurfaceSize);
			pixels[indeces] = pixels[indeces]+c.r;
			pixels[indeces] = pixels[indeces+1]+c.g;
			pixels[indeces] = pixels[indeces+2]+c.b;
			}
		}
	}
}

/*
* This drawLine() function is adapted from Bresenham's line algorithm for drawing line segments 
* as outlined in the Wikipedia article of the same name:
* http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
* 
* Currently, it doesn't work.
*/
void HomeWork01App::drawLine(uint8_t* pixels, int x0, int y0, int x1, int y1, Color8u c)
{
	// Attempt at adapting to floats instead of avoiding them
	// as Bresenham avoided them.
	/*
	for(float x = 0; x < 1;x=x0+(x/(x1-x))) {
		
		int indeces = 3*(y + x*kSurfaceSize);
		pixels[indeces] = c.r;
		pixels[indeces+1] = c.g;
		pixels[indeces+2] = c.b;
	}
	*/

	
	bool steep = abs(y1 - y0) > abs(x1 - x0);
	if(steep) {
		// Swap x0,y0
		int temp = x0;
		x0 = y0;
		y0= temp;
	}
	if(x0 > x1) {
		// Swap x0,x1
		int temp = x0;
		x0 = x1;
		x1 = temp;

		// Swap y0,y1
		temp = y0;
		y0 = y1;
		y1 = temp;
	}
	int dx = x1-x0;
	int dy = abs(y1-y0);
	float error = 0;  // May work better as doubles
	float dError = (float)(dy/dx);
	int yStep;
	int y = y0;
	if(y0 < y1) {
		yStep = 1;
	} else {
		yStep = -1;
	}
	for(int x = x0; x <= x1; x++) {
		if(steep) {

			if(x0 < 0 || y0 < 0 || x1 < 0 || y1 < 0
				|| x0 > kSurfaceSize
				|| y0 > kSurfaceSize  // Probably contains unnecessary code
				|| x1 > kSurfaceSize
				|| y1 > kSurfaceSize) continue;

			//plot(y,x);
			int indeces = 3*(y + x*kSurfaceSize);
			pixels[indeces] = c.r;
			pixels[indeces+1] = c.g;
			pixels[indeces+2] = c.b;
		} else {

			if(x0 < 0 || y0 < 0 || x1 < 0 || y1 < 0
				|| x0 > kSurfaceSize
				|| y0 > kSurfaceSize  // Probably contains unnecessary code
				|| x1 > kSurfaceSize
				|| y1 > kSurfaceSize) continue;

			// plot(x,y)
			int indeces = 3*(x + y*kSurfaceSize);
			pixels[indeces] = c.r;
			pixels[indeces+1] = c.g;
			pixels[indeces+2] = c.b;
		}
		error = error + dError;
		if(error >= 0.5) {
			y = y+yStep;
			error = error - 1.0;
		}
	}
	
}

/*
* Buggy and calls drawLine()
*/
void HomeWork01App::drawRays(uint8_t* pixels,int center_x, int center_y, int r, int degree_0, int degree_1, Color8u c) {

	for(degree_0; degree_0 <= degree_1;degree_0++) {

		double rad = (degree_0*((3.14159)/180.0));
		int end_x = ((int)(r*cos((rad))));
		int end_y = ((int)(r*sin(rad)));

		drawLine(pixels,center_x,center_y,end_x,end_y,c);
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
/*
* This blur function works effectively, but it is very, very taxing.
* You may be able to blur every other pixel to help with the heavy drop in framerate instead of actually blurring every
* picture if it is that much of a problem.
*/
void HomeWork01App::blur(uint8_t* myPixels) {

	static uint8_t copyArray[3*kSurfaceSize*kSurfaceSize];
	memcpy(copyArray,myPixels,3*kSurfaceSize*kSurfaceSize);

	uint8_t redSum = 0;
	uint8_t greenSum = 0;
	uint8_t blueSum = 0;
	int index = 0;

	for(int y = 1; y < kSurfaceSize-1;y++) {
		for(int x = 1; x < kSurfaceSize-1; x++) {
			redSum = 0;
			greenSum = 0;
			blueSum = 0;
			for(int ky = -1; ky < 2;ky++) {
				for(int kx = -1; kx < 2; kx++) {
					index = 3*(x+kx+(y+ky)*kSurfaceSize);
					redSum = redSum + copyArray[index]/9.0;
					greenSum = greenSum + copyArray[index+1]/9.0;
					blueSum = blueSum + copyArray[index+2]/9.0;
				}
			}
			index = 3*(x+y*kSurfaceSize);
			myPixels[index] = redSum;
			myPixels[index+1] = greenSum;
			myPixels[index+2] = blueSum;
		}
	}
}


void HomeWork01App::setup()
{
	// For animation in update
	top = 146;
	bottom = 354;
	left = 209;
	right = 429;
	disp = 4;
	fast = false;
	count = 0;

	// Initialize mySurface_
	mySurface_ = new Surface(kSurfaceSize,kSurfaceSize,false);
	myPixels = (*mySurface_).getData();

	// Gradient hides this function
	//blackOutWindow(myPixels);

	gradient(myPixels, Color8u(0,0,255));

	drawCircle(myPixels,313, 250, 100, Color8u(0,0,255));
	drawDiamond(myPixels,313, 250, 200, Color8u(180,255,180));
	drawDiamond(myPixels,313, 250, 150, Color8u(180,255,180));
	drawDiamond(myPixels,313, 250, 100, Color8u(180,255,180));
	
	//drawSquare(myPixels,313, 250, 100, Color8u(255,255,255));
	//drawSquare(myPixels,468, 250, 100, Color8u(255,255,255));

	//Top
	drawRectangle(myPixels,313, top, 367, 3, Color8u(180,180,180));
	drawRectangle(myPixels,313, top-4, 367, 3, Color8u(180,180,180));
	drawRectangle(myPixels,313, top-8, 367, 3, Color8u(180,180,180));
	drawRectangle(myPixels,313, top-12, 367, 3, Color8u(180,180,180));
	//Bottom
	drawRectangle(myPixels,313, bottom, 367, 3, Color8u(180,180,180));
	drawRectangle(myPixels,313, bottom+4, 367, 3, Color8u(180,180,180));
	drawRectangle(myPixels,313, bottom+8, 367, 3, Color8u(180,180,180));
	drawRectangle(myPixels,313, bottom+12, 367, 3, Color8u(180,180,180));
	//Left
	drawRectangle(myPixels,left, 250, 3, 367, Color8u(180,180,180));
	drawRectangle(myPixels,left-4, 250, 3, 367, Color8u(180,180,180));
	drawRectangle(myPixels,left-8, 250, 3, 367, Color8u(180,180,180));
	drawRectangle(myPixels,left-12, 250, 3, 367, Color8u(180,180,180));
	//Right
	drawRectangle(myPixels,right, 250, 3, 367, Color8u(180,180,180));
	drawRectangle(myPixels,right-4, 250, 3, 367, Color8u(180,180,180));
	drawRectangle(myPixels,right-8, 250, 3, 367, Color8u(180,180,180));
	drawRectangle(myPixels,right-12, 250, 3, 367, Color8u(180,180,180));

	tint(myPixels, Color8u(40,0,40));	
}


void HomeWork01App::mouseDown( MouseEvent event )
{
	fast = !fast;
}

void HomeWork01App::update()
{
	if(fast) {
		disp = 8;
	} else {
		disp = 4;
	}

	count++;

	gradient(myPixels, Color8u(0,0,255));
	drawCircle(myPixels,313, 250, 100, Color8u(0,0,255));
	drawDiamond(myPixels,313, 250, 200, Color8u(180,255,180));
	drawDiamond(myPixels,313, 250, 150, Color8u(180,255,180));
	drawDiamond(myPixels,313, 250, 100, Color8u(180,255,180));

         // Shorten this by looping the 4 drawRectangle calls per section.

	if((count%116) < 58){
		//Top
		top = top + disp;
	drawRectangle(myPixels,313, top, 367, 3, Color8u(180,180,180));
	drawRectangle(myPixels,313, top-4, 367, 3, Color8u(180,180,180));
	drawRectangle(myPixels,313, top-8, 367, 3, Color8u(180,180,180));
	drawRectangle(myPixels,313, top-12, 367, 3, Color8u(180,180,180));
	//Bottom
	bottom = bottom - disp;
	drawRectangle(myPixels,313, bottom, 367, 3, Color8u(180,180,180));
	drawRectangle(myPixels,313, bottom+4, 367, 3, Color8u(180,180,180));
	drawRectangle(myPixels,313, bottom+8, 367, 3, Color8u(180,180,180));
	drawRectangle(myPixels,313, bottom+12, 367, 3, Color8u(180,180,180));
	//Left
	left = left + disp;
	drawRectangle(myPixels,left, 250, 3, 367, Color8u(180,180,180));
	drawRectangle(myPixels,left-4, 250, 3, 367, Color8u(180,180,180));
	drawRectangle(myPixels,left-8, 250, 3, 367, Color8u(180,180,180));
	drawRectangle(myPixels,left-12, 250, 3, 367, Color8u(180,180,180));
	//Right
	right = right - disp;
	drawRectangle(myPixels,right, 250, 3, 367, Color8u(180,180,180));
	drawRectangle(myPixels,right-4, 250, 3, 367, Color8u(180,180,180));
	drawRectangle(myPixels,right-8, 250, 3, 367, Color8u(180,180,180));
	drawRectangle(myPixels,right-12, 250, 3, 367, Color8u(180,180,180));
	} else {
	//Top
	top = top - disp;
	drawRectangle(myPixels,313, top, 367, 3, Color8u(180,180,180));
	drawRectangle(myPixels,313, top-4, 367, 3, Color8u(180,180,180));
	drawRectangle(myPixels,313, top-8, 367, 3, Color8u(180,180,180));
	drawRectangle(myPixels,313, top-12, 367, 3, Color8u(180,180,180));
	//Bottom
	bottom = bottom + disp;
	drawRectangle(myPixels,313, bottom, 367, 3, Color8u(180,180,180));
	drawRectangle(myPixels,313, bottom+4, 367, 3, Color8u(180,180,180));
	drawRectangle(myPixels,313, bottom+8, 367, 3, Color8u(180,180,180));
	drawRectangle(myPixels,313, bottom+12, 367, 3, Color8u(180,180,180));
	//Left
	left = left -disp;
	drawRectangle(myPixels,left, 250, 3, 367, Color8u(180,180,180));
	drawRectangle(myPixels,left-4, 250, 3, 367, Color8u(180,180,180));
	drawRectangle(myPixels,left-8, 250, 3, 367, Color8u(180,180,180));
	drawRectangle(myPixels,left-12, 250, 3, 367, Color8u(180,180,180));
	//Right
	right = right + disp;
	drawRectangle(myPixels,right, 250, 3, 367, Color8u(180,180,180));
	drawRectangle(myPixels,right-4, 250, 3, 367, Color8u(180,180,180));
	drawRectangle(myPixels,right-8, 250, 3, 367, Color8u(180,180,180));
	drawRectangle(myPixels,right-12, 250, 3, 367, Color8u(180,180,180));
	}
	tint(myPixels, Color8u(40,0,40));
	if(fast) {
		blur(myPixels);
	}
}

void HomeWork01App::draw()
{
	gl::draw(*mySurface_);
}

CINDER_APP_BASIC( HomeWork01App, RendererGl )
