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

private:
	static const int kWinWidth = 900;
	static const int kWinHeight = 900;
	static const int kSurfaceSize=1024;

	// Strobe Variables & Constants
	int currentFrame;
	int red;
	int green;
	int blue;
	double redStrobePeriod;
	double greenStrobePeriod;
	double blueStrobePeriod;
	int strobeDivisor;
	int redAmp;
	int greenAmp;
	int blueAmp;


	Surface* mySurface_;
	uint8_t* myPixels;
	void blackOutWindow(uint8_t* pixels);
	double pi();
	void strobe(int currentFrame);
	void drawCircle(uint8_t* pixels,int x, int y, int r, Color8u c);
	void drawSquare(uint8_t* pixels,int x, int y, int r, Color8u c);
	void drawDiamond(uint8_t* pixels,int x, int y, int r, Color8u c);
};

void HomeWork01App::setup()
{
	// Strobe Variables & Constants
	currentFrame = 1;
	//PI=3.141592;
	red = 127;
	green = 127;
	blue = 127;
	redStrobePeriod = 1;
	greenStrobePeriod = 2;
	blueStrobePeriod = 4;
	strobeDivisor = 20;
	redAmp = 255;
	greenAmp = 255;
	blueAmp = 255;

	// Initialize mySurface_
	mySurface_ = new Surface(kSurfaceSize,kSurfaceSize,false);
	myPixels = (*mySurface_).getData();
	blackOutWindow(myPixels);
	strobe(currentFrame);
	drawCircle(myPixels,300, 350, 5, Color(red,green,blue));
	drawSquare(myPixels,300, 350, 100, Color(180,180,180));
	drawDiamond(myPixels,200, 250, 100, Color(red,green,blue));
	
}

void HomeWork01App::blackOutWindow(uint8_t* pixels)
{
	Color8u c = Color(0,0,0);

	for(int y=0; y<kWinWidth; y++) {
		for(int x=0; x<kWinWidth; x++) {
			int offset = 3*(x + y*kSurfaceSize);
			pixels[offset] = pixels[offset]/2 + c.r/2; //Red
			pixels[offset+1] = pixels[offset+1]/2 + c.g/2; //Green
			pixels[offset+2] = pixels[offset+2]/2 + c.b/2; // Blue
		}
	}
}

double HomeWork01App::pi()
{
	return 3.141592;
}

/*
This function is supposed to change the integer variables that make up
a Color according the number of frames since the app started.
*/
void HomeWork01App::strobe(int currentFrame)
{
double frame =  (double)currentFrame/strobeDivisor;

red = (int)(redAmp*sin(frame*pi()/redStrobePeriod));
green = (int)(greenAmp*sin(frame*pi()/greenStrobePeriod));
blue = (int)(blueAmp*sin(frame*pi()/blueStrobePeriod));

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

	for(int y=center_y-r; y<=center_y+r; y++){
		for(int x=center_x-r; x<=center_x+r; x++){
			// Bounds test, to stop out-of-bounds memory access
			if(y < 0 || x < 0 || x >= kWinWidth || y >= kWinHeight) continue;

			int distanceFromCenter = (int)sqrt((double)((x-center_x)*(x-center_x) + (y-center_y)*(y-center_y)));
			if(distanceFromCenter <= r){
				//if((distanceFromCenter/7)%2 == 1 ){  //...(distanceFromCenter/7)
						int offset = 3*(x + y*kSurfaceSize);
						//Dr. Brinkman: "By blending the colors I get a semi-transparent effect"
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
			int xLeft=center_x-xRad;
			int xRight=center_x+xRad;

			if(y < 0 || xLeft < 0 || xRight < 0 || xLeft >= kWinWidth || xRight >= kWinWidth || y >= kWinHeight) continue;

			int indeces = 3*(xLeft + y*kSurfaceSize);
			pixels[indeces] = pixels[indeces]+c.r;
			pixels[indeces] = pixels[indeces+1]+c.g;
			pixels[indeces] = pixels[indeces+2]+c.b;

			indeces = 3*(xRight + y*kSurfaceSize);
			pixels[indeces] = pixels[indeces]+c.r;
			pixels[indeces] = pixels[indeces+1]+c.g;
			pixels[indeces] = pixels[indeces+2]+c.b;


			if(y<center_y) {
			xRad--;
			} else if(y>=center_y) {
				xRad++;
		}
	}
}

void HomeWork01App::mouseDown( MouseEvent event )
{
}

void HomeWork01App::update()
{
	//blackOutWindow(myPixels);
	currentFrame++;
	strobe(currentFrame);
	drawDiamond(myPixels,200, 250, 100, Color(red,green,blue));

	drawCircle(myPixels,300, 350, 100, Color(red,green,blue));
	//drawSquare(myPixels,300, 350, 100, Color(180,180,180));


	//Get our array of pixel information
	//uint8_t* dataArray = (*mySurface_).getData();

}

void HomeWork01App::draw()
{
	//gl::Texture myTexture = gl::Texture( myPixels);
	gl::clear(Color(0,0,0));
	gl::draw(*mySurface_);
}

CINDER_APP_BASIC( HomeWork01App, RendererGl )
