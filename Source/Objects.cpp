#include <windows.h>
#include <math.h> 
#include "..\Header\Gamelib.h"



//////////////////
// BASIC OBJECT //
//////////////////


OBJECT::OBJECT()
	{
	Friction		= 0.0f;

	Width			= 0;
	Height			= 0;

	Visible			= 0;
	
	Sprite			= NULL;
	Frame			= 0;

	Anim.Enabled	= FALSE;
	Anim.Frames		= NULL;
	Anim.Pos		= 0;
	Anim.Count		= 0;
	Anim.Speed		= 0;

	NumBounds		= 0;

	InputEnabled	= FALSE;
	}





void OBJECT::Draw(SURFACE * surface)
	{

	/////////////////////////////
	//						   //
	// Used to draw the object //
	//						   //
	/////////////////////////////


	// Do nothing if not visible or no sprite is attached

	if (!Visible || Sprite == NULL)
		return;


	// Draw the sprite 

	if (Scaled)
		Sprite->DrawScaled(surface, Frame, Scr.X + SprOffset.X, Scr.Y + SprOffset.Y, Scale); 
	else
		Sprite->Draw(surface, Frame, Scr.X + SprOffset.X, Scr.Y + SprOffset.Y);
	}





void OBJECT::Draw(SURFACE * surface, int frame, int x, int y)
	{

	/////////////////////////////
	//						   //
	// Used to draw the object //
	//						   //
	/////////////////////////////


	// Do nothing if not visible or no sprite is attached

	if (!Visible || Sprite == NULL)
		return;


	// Draw the sprite 

	if (Scaled)
		Sprite->DrawScaled(surface, frame, x, y, Scale); 
	else
		Sprite->Draw(surface, frame, x, y);
	}






void OBJECT::Update()
	{

	///////////////////////////////
	//							 //
	// Used to update the sprite //
	//							 //
	///////////////////////////////


	PreUpdate();
	UpdateAction();
	UpdateState();
	UpdateDeltas();
	UpdateMove(); 
	CheckCoordinates();
	UpdateAnimation();
	PostUpdate();
	}






void OBJECT::UpdateMove()
	{

	//////////////////////////////////////////
	//										//
	// Used to update the object's position //
	//										//
	//////////////////////////////////////////


	// Set the last wld position to the current position

	LastWld.X = Wld.X;
	LastWld.Y = Wld.Y;
	LastWld.Z = Wld.Z;



	// If locked to an object, just set the new position 

	if (Lock.Enabled && Lock.Object != NULL)
		{
		if (Lock.LockX)	Wld.X = Lock.Object->Wld.X + Lock.OffsetX;
		if (Lock.LockY) Wld.Y = Lock.Object->Wld.Y + Lock.OffsetY;
		if (Lock.LockZ) Wld.Z = Lock.Object->Wld.Z + Lock.OffsetZ;
		return;
		}


	// Change the X velocity based on acceleration 

	if (Vel.X < DestVel.X)	
		{
		Vel.X += Acc.X;// + Friction;

		if (Vel.X > DestVel.X)	
			Vel.X = DestVel.X;
		}
	else if (Vel.X > DestVel.X)
		{
		Vel.X -= Acc.X;// - Friction;

		if (Vel.X < 0.0f)		
			Vel.X = 0.0f;
		}


	// Change the Y velocity based on acceleration 

	if (Vel.Y < DestVel.Y)
		{
		Vel.Y += Acc.Y;
		//Vel.Y -= Friction;

		if (Vel.Y > DestVel.Y)	
			Vel.Y = DestVel.Y;
		}
	else if (Vel.Y > DestVel.Y)
		{
		Vel.Y -= Acc.Y;
		//Vel.Y += Friction;

		if (Vel.Y < 0.0f)		
			Vel.Y = 0.0f;
		}


	// Change the Z velocity based on acceleration 

	if (Vel.Z < DestVel.Z)
		{
		Vel.Z += Acc.Z;

		if (Vel.Z > DestVel.Z)	
			Vel.Z = DestVel.Z;
		}
	else
		{
		Vel.Z -= Acc.Z;

		if (Vel.Z < 0.0f)		
			Vel.Z = 0.0f;
		}


	// Calculate the total velocity

	TotalVel = sqrtf((Vel.X * Vel.X) + (Vel.Y * Vel.Y));


	// Move X,Y,Z value

	Wld.X += Delta.X;
	Wld.Y += Delta.Y;
	Wld.Z += Delta.Z;




	// Update the display list (if registered to one) 

	if (DisplayList)
		{
		if (Delta.Y < 0)	
			{
			DisplayList->ReorderDown(this);
			DisplayList->ReorderDown(this);
			}
		else if (Delta.Y > 0)	
			{
			DisplayList->ReorderUp(this);
			DisplayList->ReorderUp(this);
			}
		}
	}





void OBJECT::UpdateAnimation()
	{

	//////////////////////////////////
	//								//
	// Used to update the animation //
	//								//
	//////////////////////////////////


	// Return if animation is not enabled 

	if (!Anim.Enabled || Anim.Frames == NULL)
		{
		Anim.Frame = 0;
		return;
		}

	// Check if the next frame should be displayed

	Anim.Count++;


	if (Anim.Count >= Anim.Speed)
		{

		// Reset the counter 

		Anim.Count = 0;


		// Only update if the current frame isn't the ending frame 

		if (Anim.Frames[Anim.Pos] != ANIM_END)
			Anim.Pos++;


		// If the current frame is a loop frame, then set the new frame

		if (Anim.Frames[Anim.Pos] == ANIM_LOOP)
			Anim.Pos = Anim.Frames[Anim.Pos + 1];
		}


	// Set the new sprite frame 

	Anim.Frame = Anim.Frames[Anim.Pos];
	}





void OBJECT::UpdateDeltas()
	{

	///////////////////////////////////////////////
	//											 //
	// Used to calculate the new delta values by //
	// the move direction						 //
	//											 //
	///////////////////////////////////////////////


	Delta.X = MoveDir.X * Vel.X;
	Delta.Y = MoveDir.Y * Vel.Y;
	Delta.Z -= GRAVITY;
	}




void OBJECT::SetState(int state)
	{

	//////////////////////////////////
	//								//
	// Used to set the object state //
	//								//
	//////////////////////////////////


	// Do nothing if the state is already set

	if (State == state)
		return;


	// Call the exit function for the current state

	if (State != -1)
		DoState(EXIT_STATE);


	// Reset the state count & done flag

	StateCount	= 0;
	StateDone	= FALSE;


	// Call the enter function for the state

	State = state;
	DoState(ENTER_STATE);
	}




void OBJECT::UpdateState()
	{

	//////////////////////////////
	//							//
	// Used to update the state //
	//							//
	//////////////////////////////


	DoState(UPDATE_STATE);

	if (InputEnabled)
		DoState(INPUT_STATE);

	StateCount++;

	if (StateCount >= StateTime && StateTime != -1)
		StateDone = TRUE;
	}





void OBJECT::DoState(int reason)
	{

	///////////////////////////////
	//							 //
	// Used to process the state //
	//							 //
	///////////////////////////////


	// Get the message map item based on the current state index

	MESSAGE_MAP_ITERATOR item;

	item = StateMap.find(State);

	if (item == StateMap.end())
		{
		}
	else
		{
		MESSAGE_MAP_FUNCTION func;

		func = (MESSAGE_MAP_FUNCTION)((*item).second);

		if (func)
			(this->*func)(reason);
		}
	}





void OBJECT::UpdateAction()
	{

	///////////////////////////////
	//							 //
	// Used to update the action //
	//							 //
	///////////////////////////////


	// Do nothing if there are no actions

	if (Actions.GetNumActions() == 0)
		return;


	// Do the current action 

	if (!Actions.IsDelayed())
		{
		DoAction(UPDATE_ACTION);


		// If this is a time based action, and the time is met 
		// then do the next action 

		if (Actions.GetTime() != -1)
			{
			// Decrement the action time 

			Actions.DecrementTime(1);

			if (Actions.GetTime() <= 0)
				DoNextAction();
			}
		}
	else
		Actions.DecrementDelayTime(1);
	}





void OBJECT::DoAction(int reason)
	{

	////////////////////////////////////////
	//									  //
	// Used to perform the current action //
	//									  //
	////////////////////////////////////////



	// Do nothing if there are no actions

	if (Actions.GetNumActions() == 0)
		return;


	// Get the message map item based on the current action index

	MESSAGE_MAP_ITERATOR item;

	item = ActionMap.find(Actions.GetType());

	if (item == ActionMap.end())
		{
		}
	else
		{
		MESSAGE_MAP_FUNCTION func;

		func = (MESSAGE_MAP_FUNCTION)((*item).second);

		if (func)
			(this->*func)(reason);
		}
	}




void OBJECT::DoNextAction()
	{

	/////////////////////////////////////
	//								   //
	// Used to perform the next action //
	//								   //
	/////////////////////////////////////


	// Do nothing if there are no actions

	if (Actions.GetNumActions() == 0)
		return;


	// Increment the action position 

	DoAction(EXIT_ACTION);
	Actions.Pop();


	// If at a valid action then perform it  

	if (Actions.GetNumActions() > 0)
		DoAction(ENTER_ACTION);
	}





void OBJECT::LockObject(OBJECT * obj, BOOL lockx, BOOL locky, BOOL lockz, float offsetx, float offsety, float offsetz)
	{

	/////////////////////////////////////////
	//									   //
	// Used to lock this object to another //
	//									   //
	/////////////////////////////////////////


	Lock.Enabled	= TRUE;
	Lock.Object		= obj;
	Lock.LockX		= lockx;
	Lock.LockY		= locky;
	Lock.LockZ		= lockz;
	Lock.OffsetX	= offsetx;
	Lock.OffsetY	= offsety;
	Lock.OffsetZ	= offsetz;
	}





void OBJECT::UnlockObject()
	{

	/////////////////////////////////////////////
	//										   //
	// Used to unlock this object from another //
	//										   //
	/////////////////////////////////////////////


	Lock.Enabled = FALSE;
	}





void OBJECT::AddToDisplayList(DISPLAY_LIST * dlist)
	{

	////////////////////////////////////////////////////
	//												  //
	// Used to register this object to a display list //
	//												  //
	////////////////////////////////////////////////////


	DisplayList = dlist;

	if (DisplayList)
		DisplayList->AddObject(this);
	}





void OBJECT::SetBounds(int index, float radius, float xoffset, float yoffset, float zoffset)
	{

	////////////////////////////////////////////
	//										  //
	// Used to set the bounding sphere values //
	//										  //
	////////////////////////////////////////////


	Bound[index].Radius		= radius;
	Bound[index].Offset.X	= xoffset;
	Bound[index].Offset.Y	= yoffset;
	Bound[index].Offset.Z	= zoffset;
	}





BOOL OBJECT::Collision(OBJECT * object)
	{

	/////////////////////////////////////////////////////
	//												   //
	// Used to check for collision with another object //
	//												   //
	/////////////////////////////////////////////////////


	float xdiff, ydiff, zdiff, dist, rdist;


	for (int i = 0; i < NumBounds; i++)
		for (int ii = 0; ii < object->NumBounds; ii++)
			{
			// Calculate the distance between each coordinate

			xdiff = (float)fabs((object->Wld.X + object->Bound[ii].Offset.X) - (Wld.X + Bound[i].Offset.X));
			ydiff = (float)fabs((object->Wld.Y + object->Bound[ii].Offset.Y) - (Wld.Y + Bound[i].Offset.Y));
			zdiff = (float)fabs((object->Wld.Z + object->Bound[ii].Offset.Z) - (Wld.Z + Bound[i].Offset.Z));
			

			// Calculate the distance between the two sphere centers

			dist = (xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff);

			
			// Calculate the distance of the radius

			rdist = (object->Bound[ii].Radius + Bound[i].Radius) * (object->Bound[ii].Radius + Bound[i].Radius) * 3.0f;


			// Return true if the collision is found

			if (dist <= rdist)
				return TRUE;
			}


	return FALSE;
	}





int OBJECT::Roll(int sides)
	{

	/////////////////////////////////////
	//								   //
	// Used to perform a standard roll //
	//								   //
	/////////////////////////////////////


	return (rand() % sides) + 1;
	}





BOOL OBJECT::Roll(int sides, int modifier, int difficulty)
	{

	/////////////////////////////////////////////////////////
	//													   //
	// Used to roll a die with a specified amount of sides //
	// and added to the modifier						   //
	//													   //
	/////////////////////////////////////////////////////////


	int roll;

	
	// Perform the roll 

	roll = Roll(sides);


	// Return for instant failure or success 

	if (roll == 1)		return FALSE;
	if (roll == sides)	return TRUE;


	// Apply the modifier 
		
	roll += modifier;


	// Return for failure or success

	return (roll >= difficulty);
	}




