#include <windows.h>
#include "..\Header\Gamelib.h"



void ACTIONS::Pop()
	{

	/////////////////////////////////////////////////
	//											   //
	// Used to POP the top action off of the stack //
	//											   //
	/////////////////////////////////////////////////


	// Return if there are no actions 

	if (NumActions == 0)
		return;


	// Remove the first action by coping the others over it

	memcpy(Action, &Action[1], sizeof(ACTION) * NumActions - 1);

	
	// Decrement the amount of actions

	NumActions--;
	}




void ACTIONS::Push(int type, int time, float param1, float param2, float param3)
	{

	//////////////////////////////////////////////
	//											//
	// Used to PUSH a new action onto the stack //
	//											//
	//////////////////////////////////////////////


	// If there are existing actions, move then forward one space

	if (NumActions > 0)
		{
		if (NumActions == MAX_ACTIONS)	NumActions--;

		memcpy(&Action[1], Action, sizeof(ACTION) * NumActions);
		}


	// Set the first action's attributes

	Action[0].Type		= type;
	Action[0].Time		= time;
	Action[0].Param[0]	= param1;
	Action[0].Param[1]	= param2;
	Action[0].Param[2]	= param3;


	// Increment the amount of actions

	NumActions++;
	}




void ACTIONS::PushEnd(int type, int time, float param1, float param2, float param3)
	{

	/////////////////////////////////////////////////////////
	//													   //
	// Used to PUSH a new action onto the end of the stack //
	//													   //
	/////////////////////////////////////////////////////////


	// If all actions are full, just return 

	if (NumActions == MAX_ACTIONS)
		return;


	// Set the last action's attributes

	Action[NumActions].Type		= type;
	Action[NumActions].Time		= time;
	Action[NumActions].Param[0]	= param1;
	Action[NumActions].Param[1]	= param2;
	Action[NumActions].Param[2]	= param3;


	// Increment the amount of actions

	NumActions++;
	}




void ACTIONS::SetTime(int time)
	{

	////////////////////////////////////////////////
	//											  //
	// Used to set the time of the current action //
	//											  //
	////////////////////////////////////////////////


	// If there are no actions, just return 

	if (NumActions == 0)
		return;


	// Set the current action's time

	Action[0].Time = time;
	}