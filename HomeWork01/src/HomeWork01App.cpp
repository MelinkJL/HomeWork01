#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

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

	static const int kSurfaceSize=1024;
	Surface* mySurface_;
};

void HomeWork01App::setup()
{
	// Initialize mySurface_
	mySurface_ = new Surface(kSurfaceSize,kSurfaceSize,false);

}

void HomeWork01App::mouseDown( MouseEvent event )
{
}

void HomeWork01App::update()
{
}

void HomeWork01App::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_BASIC( HomeWork01App, RendererGl )
