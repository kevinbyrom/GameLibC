#include <windows.h>
#include "..\Header\Gamelib.h"



SURFACE_PIXEL_FORMAT::SURFACE_PIXEL_FORMAT()
	{
	BitCount = 0;
	}





SURFACE_PIXEL_FORMAT::~SURFACE_PIXEL_FORMAT()
	{
	}





BOOL SURFACE_PIXEL_FORMAT::Create(SURFACE * surface)
	{	

	///////////////////////////////////
	//								 //
	// Used to create from a surface //
	//								 //
	///////////////////////////////////



	// Clear out the pixel format structure and set the size

	memset(&DDPixFormat, 0, sizeof(DDPixFormat));
	DDPixFormat.dwSize = sizeof(DDPixFormat);


	// Get the dd pixel format from the surface 

	DDPixFormat = surface->GetDDPixelFormat();


	// Create from the dd pixel format

	return Create(&DDPixFormat);
	}





BOOL SURFACE_PIXEL_FORMAT::Create(DDPIXELFORMAT * pixelformat)
	{

	/////////////////////////////////////////
	//									   //
	// Used to create from a DDPIXELFORMAT //
	//									   //
	/////////////////////////////////////////


	// Set the value of DDPixFormat if it has not been done yet 

	if (&DDPixFormat != pixelformat) 
		DDPixFormat = *pixelformat;


	// Set the bit count 

	BitCount = DDPixFormat.dwRGBBitCount;


	// Set the color masks

	RedMask		= DDPixFormat.dwRBitMask;
	GreenMask	= DDPixFormat.dwGBitMask;
	BlueMask	= DDPixFormat.dwBBitMask;


	// Set the color bases

	RedBase		= FindBase(RedMask);
	GreenBase	= FindBase(GreenMask);
	BlueBase	= FindBase(BlueMask);


	// Set the missing bits

	RedMissingBits		= 8 - FindBitCount(RedMask);
	GreenMissingBits	= 8 - FindBitCount(GreenMask);
	BlueMissingBits		= 8 - FindBitCount(BlueMask);


	// Set the alpha values

	AlphaMask			= DDPixFormat.dwRGBAlphaBitMask;
	AlphaBitCount		= FindBitCount(AlphaMask);
	AlphaMissingBits	= 8 - AlphaBitCount;
	AlphaBase			= FindBase(AlphaMask);


	// Set the word masks

	WordRedMask		= __int64(RedMask) + (__int64(RedMask) << 16) + (__int64(RedMask) << 32) + (__int64(RedMask) << 48);
	WordGreenMask	= __int64(GreenMask) + (__int64(GreenMask) << 16) + (__int64(GreenMask) << 32) + (__int64(GreenMask) << 48);
	WordBlueMask	= __int64(BlueMask) + (__int64(BlueMask) << 16) + (__int64(BlueMask) << 32) + (__int64(BlueMask) << 48);


	// If all word masks are not zero, set the color mods

	if(RedMask !=0 && GreenMask !=0 && BlueMask!=0)
		{
		RedMod		= (255<<16) / ((1 << FindBitCount(RedMask)) -1);
		GreenMod	= (255<<16) / ((1 << FindBitCount(GreenMask)) -1);
		BlueMod		= (255<<16) / ((1 << FindBitCount(BlueMask)) -1);
		}


	return TRUE;
	}





DWORD SURFACE_PIXEL_FORMAT::ToRGB(DWORD r, DWORD g, DWORD b)
	{

	///////////////////////////////////////////////////
	//												 //
	// Used to get an RGB value from the given r,g,b //
	//												 //
	///////////////////////////////////////////////////


	return (((r>>RedMissingBits)<<RedBase)|((g>>GreenMissingBits)<<GreenBase)|((b>>BlueMissingBits)<<BlueBase));
	}





DWORD SURFACE_PIXEL_FORMAT::ToRGBA(DWORD r, DWORD g, DWORD b,DWORD a)
	{

	//////////////////////////////////////////////////////
	//													//
	// Used to get an RGBA value from the given r,g,b,a //
	//													//
	//////////////////////////////////////////////////////


	return (((r>>RedMissingBits)<<RedBase)|((g>>GreenMissingBits)<<GreenBase)|((b>>BlueMissingBits)<<BlueBase)|((a>>AlphaMissingBits)<<AlphaBase));
	}





DWORD SURFACE_PIXEL_FORMAT::ToAlpha(DWORD a)
	{

	/////////////////////////////////////////////////
	//											   //
	// Used to get an alpha value from the given a //
	//											   //
	/////////////////////////////////////////////////


	return ((a >> AlphaMissingBits) << AlphaBase);
	}





DWORD SURFACE_PIXEL_FORMAT::GetRedFast(DWORD c)
	{

	//////////////////////////////////////////////////////////////////
	//																//
	// Used to extract a red value from a color (fast & inaccurate) //
	//																//
	//////////////////////////////////////////////////////////////////


	return (((c & RedMask) >> RedBase) << RedMissingBits);
	}





DWORD SURFACE_PIXEL_FORMAT::GetRed(DWORD c)
	{

	//////////////////////////////////////////////
	//											//
	// Used to extract a red value from a color //
	//											//
	//////////////////////////////////////////////


	return ((c & RedMask) == RedMask) ? 255 : ((((c & RedMask) >> RedBase) * RedMod) >> 16);
	}





DWORD SURFACE_PIXEL_FORMAT::GetGreenFast(DWORD c)
	{

	////////////////////////////////////////////////////////////////////
	//																  //
	// Used to extract a green value from a color (fast & inaccurate) //
	//																  //
	////////////////////////////////////////////////////////////////////


	return (((c & GreenMask) >> GreenBase) << GreenMissingBits);
	}





DWORD SURFACE_PIXEL_FORMAT::GetGreen(DWORD c)
	{

	////////////////////////////////////////////////
	//											  //
	// Used to extract a green value from a color //
	//											  //
	////////////////////////////////////////////////


	return ((c & GreenMask) == GreenMask) ? 255 : ((((c & GreenMask) >> GreenBase) * GreenMod) >> 16);
	}





DWORD SURFACE_PIXEL_FORMAT::GetBlueFast(DWORD c)
	{

	///////////////////////////////////////////////////////////////////
	//																 //
	// Used to extract a blue value from a color (fast & inaccurate) //
	//																 //
	///////////////////////////////////////////////////////////////////


	return (((c & BlueMask) >> BlueBase) << BlueMissingBits);
	}





DWORD SURFACE_PIXEL_FORMAT::GetBlue(DWORD c)
	{

	///////////////////////////////////////////////
	//											 //
	// Used to extract a blue value from a color //
	//											 //
	///////////////////////////////////////////////


	return ((c & BlueMask) == BlueMask) ? 255 : ((((c & BlueMask) >> BlueBase) * BlueMod) >> 16);
	}





DWORD SURFACE_PIXEL_FORMAT::FindBase(DWORD mask)
	{

	/////////////////////////////////////
	//								   //
	// Used to find a base for a color //
	//								   //
	/////////////////////////////////////


	for(int i = 0; i < 32; i++)
		{
		if((mask >> i) & 1)
			return i;
		}

	return 0;
	}





DWORD SURFACE_PIXEL_FORMAT::FindBitCount(DWORD mask)
	{

	//////////////////////////////////////////
	//										//
	// Used to find a bit count for a color //
	//										//
	//////////////////////////////////////////


	int bitcount = 0;


	for(int i = 0; i < 32; i++)
		{
		if((mask >> i) & 1)
			bitcount++;
		}

	return bitcount;
	}