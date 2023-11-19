#include <windows.h>
#include "..\Header\Gamelib.h"


TILE_MAP::TILE_MAP()
	{
	Sprite	= NULL;
	Tile	= NULL;
	SizeX	= 0;
	SizeY	= 0;
	}



TILE_MAP::~TILE_MAP()
	{
	Destroy();
	}




void TILE_MAP::Create(int numx, int numy, int resx, int resy)
	{

	//////////////////////////////
	//							//
	// Used to create a new map //
	//							//
	//////////////////////////////


	// Destroy the map if already allocated 

	if (Tile != NULL)
		Destroy();


	// Allocate a new tile map 

	Tile	= new unsigned char[numx * numy];

	SizeX	= numx;
	SizeY	= numy;
	ResX	= resx;
	ResY	= resy;


	// Fill the map

	memset(Tile, 0, SizeX * SizeY);
	}





void TILE_MAP::Create(unsigned char * tiles, int numx, int numy, int resx, int resy)
	{

	//////////////////////////////
	//							//
	// Used to create a new map //
	//							//
	//////////////////////////////


	// Create the map

	Create(numx, numy, resx, resy);
	

	// Fill the map with the tiles 

	memcpy(Tile, tiles, SizeX * SizeY);
	}





void TILE_MAP::Destroy()
	{

	///////////////////////////
	//						 //
	// Used to destroy a map //
	//						 //
	///////////////////////////


	if (Tile != NULL)
		delete[] Tile;

	SizeX = 0;
	SizeY = 0;
	}




void TILE_MAP::Resize(int numx, int numy)
	{

	////////////////////////////
	//						  //
	// Used to resize the map //
	//						  //
	////////////////////////////


	unsigned char * tileback;
	int smallwidth, smallheight;
 

	if (Tile == NULL)
		return;


	// Get the smallest width & height

	smallwidth	= numx < SizeX ? numx : SizeX;
	smallheight	= numy < SizeY ? numy : SizeY;

	
	// Allocate a new tile buffer for backup

	tileback = new unsigned char[SizeX * SizeY];


	// Destroy the current tile map

	Destroy();


	// Create one of new size 

	Create(numx, numy, ResX, ResY);


	// Copy the info from the backup

	for (int y = 0; y < smallheight; y++)
		memcpy(&Tile[y * smallwidth], &tileback[y * smallwidth], smallwidth);


	// Destroy the backup

	delete[] tileback;
	}





unsigned char TILE_MAP::GetTile(int x, int y)
	{

	////////////////////////////////////////
	//									  //
	// Used to return the value of a tile //
	//									  //
	////////////////////////////////////////


	if (x < 0 || x >= SizeX || y < 0 || y >= SizeY)
		return 0;

	return Tile[x + (y * SizeX)];
	}





void TILE_MAP::SetTile(int x, int y, unsigned char val)
	{

	////////////////////////////////
	//							  //
	// Used to set a tile's value //
	//							  //
	////////////////////////////////


	if (x >= 0 && x < SizeX && y >= 0 && y < SizeY)
		Tile[x + (y * SizeX)] = val;
	}



void TILE_MAP::SetRow(int row, unsigned char val)
	{

	///////////////////////////////
	//							 //
	// Used to set a row's value //
	//							 //
	///////////////////////////////


	if (row >= 0 && row < SizeY)
		for (int i = 0; i < SizeX; i++)
			Tile[i + (row * SizeX)] = val;
	}



void TILE_MAP::SetColumn(int col, unsigned char val)
	{

	//////////////////////////////////
	//								//
	// Used to set a column's value //
	//								//
	//////////////////////////////////


	if (col >= 0 && col < SizeX)
		for (int i = 0; i < SizeY; i++)
			Tile[col + (i * SizeX)] = val;
	}




void TILE_MAP::Fill(unsigned char val)
	{

	/////////////////////////////////
	//							   //
	// Used to fill the entire map //
	//							   //
	/////////////////////////////////

	
	for (int i = 0; i < SizeX * SizeY; i++)
		Tile[i] = val;
	}




void TILE_MAP::Draw(SURFACE * surface, int x, int y, int width, int height, int sx, int sy)
	{

	////////////////////////////////////////////
	//										  //
	// Used to draw the tile map to a surface //
	//										  //
	////////////////////////////////////////////


	int numtilesx, numtilesy;
	int firsttilex, firsttiley;
	int offsetx, offsety;
	int tx, ty;
	int tilex, tiley;


	if (Sprite == NULL)
		return;


	// Calculate how many tiles to draw x & y

	numtilesx = (width / ResX) + 1;
	numtilesy = (height / ResY) + 1;


	// Get the start x tile and start y tile 

	firsttilex = sx / ResX;
	firsttiley = sy / ResY;


	// Get the offset of the first tiles

	offsetx = (firsttilex * ResX) - sx;
	offsety = (firsttiley * ResY) - sy;


	// Draw each tile 

	for (ty = 0; ty < numtilesy; ty++)
		for (tx = 0; tx < numtilesx; tx++)
			{
			tilex = firsttilex + tx;
			tiley = firsttiley + ty;

			if (tilex >= 0 && tilex < SizeX && tiley >= 0 && tiley < SizeY)
				Sprite->Draw(surface, Tile[tilex + (tiley * SizeX)], x + (tx * ResX) + offsetx, y + (ty * ResY) + offsety);
			else
				Sprite->Draw(surface, 0, x + (tx * ResX) + offsetx, y + (ty * ResY) + offsety);
			}
	}