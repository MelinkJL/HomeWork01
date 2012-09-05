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
* This project was written for credit in Dr. Brinkman's CSE 274 class at
* Miami University during the fall semester of 2012.
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
	int disp;	// For animation in update()
	int count;	// Also for animation in update()
	int top;	// ""
	int bottom;	// ""
	int left;	// ""
	int right;	// ""
	bool fast;	// ""
	void drawCircle(uint8_t* pixels,int x, int y, int r, Color8u c);
	void drawSquare(uint8_t* pixels,int x, int y, int r, Color8u c);
	void drawDiamond(uint8_t* pixels,int x, int y, int r, Color8u c);
	void drawRectangle(uint8_t* pixels,int center_x, int center_y, int rx, int ry, Color8u c);
	void blackOutWindow(uint8_t* pixels);
	void gradient(uint8_t* pixels, Color8u c);
	void tint(uint8_t* pixels, Color8u t);
	void drawLine(uint8_t* pixels, int x0, int y0, int x1, int y1, Color8u c);
	void drawRays(uint8_t* pixels,int center_x, int center_y, int r, int degree_0, int degree_1, Color8u c);
	void drawRightTriangle(uint8_t* pixels,int x0, int y0,int x1, int y1,Color8u c);
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
				//if((distanceFromCenter/7)%2 == 1 ){  //...(distanceFromCenter/7)
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
					//}
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

			if((x == center_x-r)&&(x == center_x+r)&&(y == center_y-r)&&(y == center_y+r)) {

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

			int indeces = 3*(x + y*kSurfaceSize);
			pixels[indeces] = pixels[indeces]+c.r;
			pixels[indeces] = pixels[indeces+1]+c.g;
			pixels[indeces] = pixels[indeces+2]+c.b;
			}
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

void HomeWork01App::drawRightTriangle(uint8_t* pixels,int x0, int y0,int x1, int y1,Color8u c) {

	if(y1 < y0) {
		int temp = y1;
		y1 = y0;
		y0 = temp;
	}
	if(x1 < x0) {
		int temp = y1;
		x1 = x0;
		x0 = temp;
	}
	int height = y1 - y0;
	int width = x1 - x0;
	bool flag = true;
	int r = x0;

	for(int y = y0; y <= y1; y++) {

		int index = 3*(r + y*kSurfaceSize);

		myPixels[index] = c.r;
		myPixels[index+1] = c.g;
		myPixels[index+2] = c.b;

		if(r >= x0 + width) {
			flag = false;
		}
		if(flag) {
			r++;
		} else if( r <= 0) continue;
		if(!flag) {
			r--;
		}
	}
}

// Credit to Dr. Brinkman
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
	/*
		static uint8_t work_buffer[3*kSurfaceSize*kSurfaceSize];
		//This memcpy is not much of a performance hit.
		memcpy(work_buffer,myPixels,3*kSurfaceSize*kSurfaceSize);

		//These are used in right shifts.
		//Both of these kernels actually darken as well as blur.
		uint8_t kernelA[9] =
			{4,3,4,
			4,3,4,
			4,3,4};
		
		uint8_t total_red =0;
		uint8_t total_green=0;
		uint8_t total_blue =0;
			int index;
			int k;
			int y,x,ky,kx;
				
		//Visit every pixel in the image, except the ones on the edge.
		//TODO Special purpose logic to handle the edge cases
			for( y=1;y<kWinHeight-1;y++){
				for( x=1;x<kWinWidth-1;x++){


					index = 3*(x + y*kWinWidth);
				if(work_buffer[index] < 256/3){
		//Compute the convolution of the kernel with the region around the current pixel
		//I use ints for the totals and the kernel to avoid overflow
					total_red=0;
					total_green=0;
					total_blue=0;
					
					for( ky=-1;ky<=1;ky++){
						for( kx=-1;kx<=1;kx++){
							index = 3*(x + kx + (y+ky)*kSurfaceSize);
							k = kernelA[kx+1 + (ky+1)*3];
							total_red += (work_buffer[index ] >> k);
							total_green += (work_buffer[index+1] >> k);
							total_blue += (work_buffer[index+2] >> k);
						}
					}
					
				}
		
			index = 3*(x + y*kSurfaceSize);
			myPixels[index] = total_red;
			myPixels[index+1] = total_green;
			myPixels[index+2] = total_blue;
			}
		}
		*/
}


void HomeWork01App::setup()
{
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


	//drawRightTriangle(myPixels,100,100,200,105,Color(255,50,50));
	tint(myPixels, Color8u(40,0,40));
	//drawLine(myPixels,100,100,110,320,Color(255,255,255));
	//drawLine(myPixels,100,100,120,310,Color(255,255,255));
	//drawRays(myPixels,400,500,50,0,360,Color8u(255,255,255));
	//blur(myPixels);
	
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
