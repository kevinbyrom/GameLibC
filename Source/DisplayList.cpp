#include <windows.h>
#include "..\Header\Gamelib.h"



DISPLAY_LIST::DISPLAY_LIST()
	{
	FirstItem	= NULL;
	LastItem	= NULL;
	}




DISPLAY_LIST::~DISPLAY_LIST()
	{
	RemoveAll();
	}




void DISPLAY_LIST::AddObject(OBJECT * object)
	{

	///////////////////////////////////////////////
	//											 //
	// Used to add an object to the display list //
	//											 //
	///////////////////////////////////////////////


	if (!FirstItem)
		{
		FirstItem = new DISPLAY_LIST_ITEM;

		FirstItem->Prev = NULL;

		LastItem = FirstItem;
		}
	else
		{
		LastItem->Next = new DISPLAY_LIST_ITEM;

		LastItem->Next->Prev = LastItem;

		LastItem = LastItem->Next;
		}

	LastItem->Object	= object;
	LastItem->Next		= NULL;

	ReorderDown(object);
	}



void DISPLAY_LIST::RemoveObject(OBJECT * object)
	{

	////////////////////////////////////////////////////
	//												  //
	// Used to remove an object from the display list //
	//												  //
	////////////////////////////////////////////////////


	DISPLAY_LIST_ITEM * Item;


	Item = GetDisplayListItemFromObject(object);

	if (Item != NULL)
		{	
		if (FirstItem == Item)
			FirstItem = Item->Next;

		if (LastItem == Item)
			LastItem = Item->Prev;

		if (Item->Next)
			Item->Next->Prev = Item->Prev;

		if (Item->Prev)
			Item->Prev->Next = Item->Next;

		delete Item;
		}
	}



void DISPLAY_LIST::RemoveAll()
	{

	///////////////////////////////////////////
	//										 //
	// Used to remove all display list items //
	//										 //
	///////////////////////////////////////////


	DISPLAY_LIST_ITEM * Item;
	DISPLAY_LIST_ITEM * Next;


	Item = FirstItem;

	while(Item)
		{
		Next = Item->Next;

		delete Item;

		Item = Next;
		}

	FirstItem	= NULL;
	LastItem	= NULL;
	}




DISPLAY_LIST_ITEM * DISPLAY_LIST::GetDisplayListItemFromObject(OBJECT * obj)
	{

	//////////////////////////////////////////////////////////////
	//															//
	// Used to get a pointer to a display list item which holds //
	// the specified object pointer							    //
	//															//
	//////////////////////////////////////////////////////////////


	DISPLAY_LIST_ITEM * Item = FirstItem;

	while(Item)
		{
		if (Item->Object == obj)
			return Item;

		Item = Item->Next;
		}

	return NULL;
	}




void DISPLAY_LIST::ReorderDown(OBJECT * obj)
	{

	////////////////////////////////////////////////////////
	//												      //
	// Used to reorder an object down in the display list //
	//												      //
	////////////////////////////////////////////////////////

	OBJECT * SwapObj;
	DISPLAY_LIST_ITEM * Item;


	Item = GetDisplayListItemFromObject(obj);

	if (Item == NULL)
		return;

	while(Item->Prev && Item->Prev->Object->Wld.Y >= Item->Object->Wld.Y)
		{
		SwapObj				= Item->Prev->Object;
		Item->Prev->Object	= Item->Object;
		Item->Object		= SwapObj;

		Item = Item->Prev;
		}
	}




void DISPLAY_LIST::ReorderUp(OBJECT * obj)
	{

	//////////////////////////////////////////////////////
	//							           			    //
	// Used to reorder an object up in the display list //
	//												    //
	//////////////////////////////////////////////////////


	OBJECT * SwapObj;
	DISPLAY_LIST_ITEM * Item;


	Item = GetDisplayListItemFromObject(obj);

	if (Item == NULL)
		return;

	while(Item->Next && Item->Next->Object->Wld.Y <= Item->Object->Wld.Y)
		{
		SwapObj				= Item->Next->Object;
		Item->Next->Object	= Item->Object;
		Item->Object		= SwapObj;

		Item = Item->Next;
		}
	}



void DISPLAY_LIST::DrawAll(SURFACE * surface)
	{
	DISPLAY_LIST_ITEM * Item;

	Item = FirstItem;

	while(Item)
		{
		Item->Object->Draw(surface);

		Item = Item->Next;
		}
	}