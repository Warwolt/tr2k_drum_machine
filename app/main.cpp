#define SDL_MAIN_HANDLED 2 // define needed to not get winmain-errors
#include "testprint.h"
#include "SDL2/SDL.h"
#include "SDLRenderingManager.h"
#include "SDLEventDispatcher.h"
#include "SDLQuitListener.h"
#include "SDLMouseListener.h"
#include "CollisionDetection.h"
#include "Rectangle.h"

using Milliseconds = U32;

struct ColorTuple
{
	U8 r;
	U8 g;
	U8 b;
};

void init();
void iterateGameLoop();
void update();
void handleEvents();
void draw();
void deinit();

static SDLRenderingManager renderer;
static SDLEventDispatcher dispatcher;
static SDLQuitListener quitListener;
static SDLMouseListener mouseListener;

static const U16 WINDOW_WIDTH = 640;
static const U16 WINDOW_HEIGHT = 480;
static const Milliseconds msPerFrame = 17; // 58.82 Hz

// rectangles
static Rectangle mouseRect = {x: 0, y: 0, w: 50, h: 50};
static Rectangle midRect = {x: (WINDOW_WIDTH-100)/2, y: (WINDOW_HEIGHT-100)/2, w: 100, h: 100};
static Rectangle minkowskiDiffRect;

// colors
static ColorTuple mouseRectColor = {0x00, 0xFF, 0x00};
static ColorTuple midRectColor = {0xEA, 0xEA, 0xEA};
static ColorTuple minkowskiDiffColor = {0x00, 0x11, 0xFF};

// other
static bool quitRequested = false;

int main(int argc, char const *argv[])
{
	/* Startup */
	init();

	/* Run program */
	while(!quitRequested)
	{
		iterateGameLoop();
	}

	/* Quit */
	deinit();
	return 0;
}

void init()
{
	setbuf(stdout, NULL); // get around issue of no trace prints

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_ShowCursor(false); // hide the mouse cursor
	renderer.createWindow("r2kPong", WINDOW_WIDTH, WINDOW_HEIGHT);
	renderer.clearWindow();
	dispatcher.subscribeEventListener(quitListener);
	dispatcher.subscribeEventListener(mouseListener);
}

void iterateGameLoop()
{
	static Milliseconds previousTime = 0;
	static Milliseconds elapsedTime = 0;

	update();

	Milliseconds currentTime = SDL_GetTicks();
	elapsedTime += currentTime - previousTime;

	if(elapsedTime >= msPerFrame)
	{
		draw();
		elapsedTime = 0;
	}

	previousTime = currentTime;
}

void update()
{
	dispatcher.dispatchNextQueuedEvent();

	if(quitListener.quitRequested())
	{
		quitRequested = true;
	}
	else
	{
		handleEvents();
	}
}

void handleEvents()
{
	// update position of mouse rectangle
	MousePosition pos = mouseListener.getCurrentMousePosition();
	mouseRect.x = pos.x - mouseRect.w/2;
	mouseRect.y = pos.y - mouseRect.h/2;

	// check if collision using minkowski difference
	minkowskiDiffRect = CollisionDetection::minkowskiDifference(midRect, mouseRect);
	bool diffOverlapsOrigin = CollisionDetection::rectangleOverlapsOrigin(minkowskiDiffRect);
	if(diffOverlapsOrigin)
	{
		mouseRectColor = {0xFF, 0x00, 0x00}; // red = collision
	}
	else
	{
		mouseRectColor = {0x00, 0xFF, 0x00}; // green = no collision
	}
}

void draw()
{
	/* Draw background */
	renderer.setDrawColor(0, 0, 0);
	renderer.drawRectFill(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// draw rectangles
	renderer.setDrawColor(minkowskiDiffColor.r, minkowskiDiffColor.g, minkowskiDiffColor.b);
	renderer.drawRectFill(minkowskiDiffRect.x, minkowskiDiffRect.y, minkowskiDiffRect.w, minkowskiDiffRect.h);

	renderer.setDrawColor(midRectColor.r, midRectColor.g, midRectColor.b);
	renderer.drawRectFill(midRect.x, midRect.y, midRect.w, midRect.h);

	renderer.setDrawColor(mouseRectColor.r, mouseRectColor.g, mouseRectColor.b);
	renderer.drawRectFill(mouseRect.x, mouseRect.y, mouseRect.w, mouseRect.h);

	/* Render to screen */
	renderer.updateWindow();
}

void deinit()
{
	SDL_Quit();
}
