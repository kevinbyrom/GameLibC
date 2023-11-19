#include <windows.h>
#include "..\Header\Gamelib.h"


SPRITE::SPRITE()
	{
	SprRect		= NULL;
	NumSprX		= 0;
	NumSprY		= 0;
	NumSprites	= 0;
	SprWidth	= 0;
	SprHeight	= 0;
	}

	


SPRITE::~SPRITE()
	{
	Destroy();
	}




BOOL SPRITE::CreateFromBitmap(SCREEN * pScreen, const char * filename, int numx, int numy, int width, int height, int border, DWORD transcolor, int memtype)
	{

	///////////////////////////////////////////////////////
	//													 //
	// Used to create a new set of sprites from a bitmap //
	//													 //
	///////////////////////////////////////////////////////
	

	int x, y;



	if (numx < 0 || numy < 0 || width <= 0 || height <= 0)
		return FALSE;


	// Destroy the sprite if already initialized //

	if (Initialized)
		Destroy();


	// Create the surface from the bitmap //

	if (!SURFACE::CreateFromBitmap(pScreen, filename, memtype))
		{
		Destroy();
		return FALSE;
		}


	// Build the rects //

	SprRect = new RECT[numx * numy];


	for (y = 0; y < numy; y++)
		for (x = 0; x < numx; x++)
			{
			SprRect[x + (y * numx)].left	= (x * width) + (border * (x + 1));
			SprRect[x + (y * numx)].top		= (y * height) + (border * (y + 1));
			SprRect[x + (y * numx)].right	= SprRect[x + (y * numx)].left + width;
			SprRect[x + (y * numx)].bottom	= SprRect[x + (y * numx)].top + height;

			// Validate the boundaries //
			}


	// Set the color key //

	SetColorKey(transcolor);


	// Set the properties //

	NumSprX		= numx;
	NumSprY		= numy;
	NumSprites	= numx * numy;
	SprWidth	= width;
	SprHeight	= height;


	return TRUE; 
	}




void SPRITE::Destroy()
	{

	///////////////////////////////////////////
	//										 //
	// Used to deallocate all loaded sprites //
	//										 //
	///////////////////////////////////////////


	SURFACE::Destroy();

	if (SprRect)
		delete[] SprRect;

	SprRect		= NULL;
	NumSprX		= 0;
	NumSprY		= 0;
	NumSprites	= 0;
	SprWidth	= 0;
	SprHeight	= 0;
	}




void SPRITE::SetRect(int num, int left, int top, int right, int bottom)
	{

	//////////////////////////////////////////////////////
	//													//
	// Used to set the dimensions of a sprite rectangle //
	//													//
	//////////////////////////////////////////////////////


	// Return if trying to set an invalid rect 

	if (num < 0 || num >= NumSprites)
		return;


	// Set the rect dimensions

	SprRect[num].left	= left;
	SprRect[num].top	= top;
	SprRect[num].right	= right;
	SprRect[num].bottom = bottom;
	}




void SPRITE::Draw(SURFACE * surf, int index, int x, int y, BOOL trans)
	{

	////////////////////////////////////////
	//									  //
	// Used to draw a sprite to a surface //
	//									  //
	////////////////////////////////////////


	// Validate the index //

	if (index < 0 || index >= NumSprites)
		return;


	// Draw the sprite, either transparent or not 

	surf->BlitFast((SURFACE *)this, x, y, &SprRect[index], trans);
	}




void SPRITE::DrawScaled(SURFACE * surf, int index, int x, int y, float scale, int trans)
	{

	//////////////////////////////////
	//								//
	// Used to draw a scaled sprite //
	//								//
	//////////////////////////////////


	// Validate the index //

	if (index < 0 || index >= NumSprites)
		return;


	// Draw the sprite, either transparent or not 

	surf->BlitFastScaled((SURFACE *)this, x, y, &SprRect[index], scale, trans);
	}




void SPRITE::DrawText(SURFACE * surf, char * text, int x, int y, int trans)
	{

	////////////////////////////////////
	//								  //
	// Used to draw text to a surface //
	//								  //
	////////////////////////////////////

	int pos, length, rect;


	// Get the text length 

	length = strlen(text);

	if (length <= 0 || NumSprites == 0)
		return;


	// Draw each letter 

	for (pos = 0; pos < length; pos++)
		{

		// Get the rect index from the current character

		rect = (int)text[pos] - 33;


		// Validate the rect index

		if (rect < 0 || rect >= NumSprites)
			rect = 0;


		// Draw the sprite, either transparent or not 

		if (text[pos] != ' ')
			surf->BlitFast((SURFACE *)this, x, y, &SprRect[rect], trans);


		// Increment the x value

		x += SprRect[rect].right - SprRect[rect].left;
		}
	}




void SPRITE::DrawTextCentered(SURFACE * surf, char * text, int x, int y, int trans)
	{

	/////////////////////////////////////////////
	//										   //
	// Used to draw centered text to a surface //
	//										   //
	/////////////////////////////////////////////

	int pos, length, width, rect;


	// Get the text length 

	length = strlen(text);

	if (length <= 0 || NumSprites == 0)
		return;


	// Get the width each letter combined

	width = 0;

	for (pos = 0; pos < length; pos++)
		{

		// Get the rect index from the current character

		rect = (int)text[pos] - 33;


		// Validate the rect index

		if (rect < 0 || rect >= NumSprites)
			rect = 0;


		// Increment the x value

		width += SprRect[rect].right - SprRect[rect].left;
		}


	// Call the draw text function

	DrawText(surf, text, x - (width / 2), y, trans);
	}