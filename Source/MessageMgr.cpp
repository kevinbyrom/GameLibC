#include <windows.h>
#include "..\Header\Gamelib.h"



MESSAGE_MGR::MESSAGE_MGR()
	{
	FirstQueued	= NULL;
	LastQueued	= NULL;

	NumTargets = 0;
	}





MESSAGE_MGR::~MESSAGE_MGR()
	{
	RemoveAllQueuedMessages();
	}





void MESSAGE_MGR::Update()
	{

	//////////////////////////////////////////////////////////////////////////
	//																		//
	// Update function which decrements each queued messages' time by 1 and //
	// dispatches that message if the time is equal or less than zero	    //
	//																		//
	//////////////////////////////////////////////////////////////////////////


	MESSAGE * msg, * next;

	
	// Scroll through the list and decrement the time for each message by one

	msg = FirstQueued;

	while(msg)
		{
		next = msg->Next;

		msg->Time--;


		// If the time for a message is done, then dispatch the message to the proper target
		// and remove the message from the list 

		if (msg->Time <= 0)
			{
			DispatchMessage(msg);
			RemoveQueuedMessage(msg);
			}

		msg = next;
		}
	}





/////////////////////////
// REGISTERING METHODS //
/////////////////////////





int MESSAGE_MGR::RegisterTarget(MESSAGE_TARGET * target, int group)
	{

	///////////////////////////////////////////////////////
	//													 //
	// Used to register a Message Target to this manager //
	//													 //
	///////////////////////////////////////////////////////


	// Return -1 if there are no more target slots available 

	if (NumTargets >= MAX_MESSAGE_TARGETS)
		return -1;


	// Add the new target 

	Target[NumTargets].Target	= target;
	Target[NumTargets].GroupID	= group;

	NumTargets++;

	return (NumTargets - 1);
	}





void MESSAGE_MGR::RemoveTargets()
	{

	////////////////////////////////////////
	//									  //
	// Used to remove all Message Targets //
	//									  //
	////////////////////////////////////////


	RemoveAllQueuedMessages();
	NumTargets = 0;
	}





/////////////////////////////
// MESSAGE SENDING METHODS //
/////////////////////////////





DWORD MESSAGE_MGR::SendMessage(int from, int to, int msg, int time, DWORD data1, DWORD data2)
	{

	///////////////////////////////////////////////
	//											 //
	// Used to send a message to a single target //
	//											 //
	///////////////////////////////////////////////


	MESSAGE newmsg;


	// Set the message parameters 

	newmsg.DestType	= MESSAGE_DEST_SINGLE;
	newmsg.From		= from;
	newmsg.To		= to;
	newmsg.Msg		= msg;
	newmsg.Time		= time;
	newmsg.Data1	= data1;
	newmsg.Data2	= data2;


	// If the time is equal or less than zero, dispatch it now

	if (time <= 0)
		return DispatchMessage(&newmsg);

	// Otherwise, queue it

	else
		AddQueuedMessage(&newmsg);


	// Return -1 for queued messages

	return MESSAGE_QUEUED;
	}




DWORD MESSAGE_MGR::SendMessageToGroup(int from, int togroup, int msg, int time, DWORD data1, DWORD data2)
	{

	///////////////////////////////////////////////
	//											 //
	// Used to send a message to a single target //
	//											 //
	///////////////////////////////////////////////


	MESSAGE newmsg;


	// Set the message parameters 

	if (togroup == MESSAGE_DEST_ALL)
		newmsg.DestType	= MESSAGE_DEST_ALL;
	else
		newmsg.DestType	= MESSAGE_DEST_GROUP;

	newmsg.From			= from;
	newmsg.To			= togroup;
	newmsg.Msg			= msg;
	newmsg.Time			= time;
	newmsg.Data1		= data1;
	newmsg.Data2		= data2;


	// If the time is equal or less than zero, dispatch it now

	if (time <= 0)
		return DispatchMessage(&newmsg);

	// Otherwise, queue it

	else
		AddQueuedMessage(&newmsg);


	// Return -1 for queued messages

	return MESSAGE_QUEUED;
	}





//////////////////////////////////////
// MESSAGE FORCING/FLUSHING METHODS //
//////////////////////////////////////





void MESSAGE_MGR::ForceMessages(int id, int sentby)
	{

	/////////////////////////////////////////////////////////////////////////////
	//																		   //
	// Used to force all messages to be sent going to a specific single target //
	//																		   //
	/////////////////////////////////////////////////////////////////////////////


	MESSAGE * msg, * next;

	
	// Scroll through the list of queued messages, forcing any messages
	// that are designated for the target specied by id

	msg = FirstQueued;

	while(msg)
		{
		next = msg->Next;

		// Dispatch & Remove a message designated for the target specified by id

		if (msg->DestType == MESSAGE_DEST_SINGLE && msg->To == id && (sentby != -1 ? msg->From == sentby : TRUE))
			{
			DispatchMessage(msg);
			RemoveQueuedMessage(msg);
			}

		msg = next;
		}
	}





void MESSAGE_MGR::ForceGroupMessages(int id, int sentby)
	{

	/////////////////////////////////////////////////////////////////////
	//																   //
	// Used to force all messages to be sent going to a specific group //
	//														           //
	/////////////////////////////////////////////////////////////////////


	MESSAGE * msg, * next;

	
	// Scroll through the list of queued messages, forcing any messages
	// that are designated for the target group specied by id

	msg = FirstQueued;

	while(msg)
		{
		next = msg->Next;

		// Dispatch & Remove a message designated for the target group specified by id

		if (msg->DestType == MESSAGE_DEST_GROUP && msg->To == id && (sentby != -1 ? msg->From == sentby : TRUE))
			{
			DispatchMessage(msg);
			RemoveQueuedMessage(msg);
			}

		msg = next;
		}
	}





void MESSAGE_MGR::FlushMessages(int id, int sentby)
	{

	/////////////////////////////////////////////////////////////////////////////
	//																		   //
	// Used to flush all messages to be sent going to a specific single target //
	//																           //
	/////////////////////////////////////////////////////////////////////////////


	MESSAGE * msg, * next;

	
	// Scroll through the list of queued messages, flushing any messages
	// that are designated for the target specied by id

	msg = FirstQueued;

	while(msg)
		{
		next = msg->Next;

		// Remove a message designated for the target specified by id

		if (msg->DestType == MESSAGE_DEST_SINGLE && msg->To == id && (sentby != -1 ? msg->From == sentby : TRUE))
			RemoveQueuedMessage(msg);

		msg = next;
		}
	}





void MESSAGE_MGR::FlushGroupMessages(int id, int sentby)
	{

	/////////////////////////////////////////////////////////////////////
	//																   //
	// Used to flush all messages to be sent going to a specific group //
	//																   //
	/////////////////////////////////////////////////////////////////////


	MESSAGE * msg, * next;

	
	// Scroll through the list of queued messages, flushing any messages
	// that are designated for the target group specied by id

	msg = FirstQueued;

	while(msg)
		{
		next = msg->Next;

		// Remove a message designated for the target group specified by id

		if (msg->DestType == MESSAGE_DEST_GROUP && msg->To == id && (sentby != -1 ? msg->From == sentby : TRUE))
			RemoveQueuedMessage(msg);

		msg = next;
		}
	}





///////////////////////////
// MESSAGE QUEUE METHODS //
///////////////////////////





void MESSAGE_MGR::AddQueuedMessage(MESSAGE * msg)
	{

	/////////////////////////////////////////////////////////////
	//														   //
	// Used to add a queued message to the queued message list //
	//														   //
	/////////////////////////////////////////////////////////////


	// If there not a First item, then allocate this item at the front of the list

	if (!FirstQueued)
		{
		FirstQueued = new MESSAGE;
		FirstQueued->Prev = NULL;

		LastQueued = FirstQueued;
		}

	// Otherwise, allocate the item at the end of the list

	else
		{
		LastQueued->Next = new MESSAGE;
		LastQueued->Next->Prev = LastQueued;

		LastQueued = LastQueued->Next;
		}


	// Set the item values

	LastQueued->DestType	= msg->DestType;
	LastQueued->From		= msg->From;
	LastQueued->To			= msg->To;
	LastQueued->Msg			= msg->Msg;
	LastQueued->Time		= msg->Time;
	LastQueued->Data1		= msg->Data1;
	LastQueued->Data2		= msg->Data2;
	LastQueued->Next = NULL;
	}




void MESSAGE_MGR::RemoveQueuedMessage(MESSAGE * msg)
	{

	//////////////////////////////////////////////////////////////////
	//															    //
	// Used to remove a queued message from the queued message list //
	//															    //
	//////////////////////////////////////////////////////////////////


	// Return if the msg points to NULL 

	if (!msg)
		return;


	// If this is the first in the list, then set the new First pointer

	if (FirstQueued == msg)
		FirstQueued = msg->Next;


	// If this is the last in the list, then set the new Last pointer

	if (LastQueued == msg)
		LastQueued = msg->Prev;


	// Set the new Next pointer of the previous item 

	if (msg->Next)	
		msg->Next->Prev = msg->Prev;


	// Set the new Prev pointer of the next item

	if (msg->Prev)
		msg->Prev->Next = msg->Next;


	// Delete the node

	delete msg;
	}




void MESSAGE_MGR::RemoveAllQueuedMessages()
	{

	/////////////////////////////////////////////////////////////////////
	//																   //
	// Used to remove all queued messages from the queued message list //
	//																   //
	/////////////////////////////////////////////////////////////////////


	MESSAGE * msg, * next;

	
	// Scroll through the list and remove each item 

	msg = FirstQueued;

	while(msg)
		{
		next = msg->Next;

		delete msg;

		msg = next;
		}


	// Set the first and last pointers to NULL

	FirstQueued = NULL;
	LastQueued	= NULL;
	}





//////////////////////////////
// MESSAGE DISPATCH METHODS //
//////////////////////////////





DWORD MESSAGE_MGR::DispatchMessage(MESSAGE * msg)
	{
	
	///////////////////////////////////////////////////////////////
	//															 //
	// Used to dispatch a queued message to the proper target(s) //
	//															 //
	///////////////////////////////////////////////////////////////


	// Call the proper Dispatch function based on the destination type

	switch(msg->DestType)
		{

		// Dispatch the message to a single target

		case MESSAGE_DEST_SINGLE:
			return DispatchMessageToSingle(msg);


		// Dispatch the message to a group of targets

		case MESSAGE_DEST_GROUP:
			return DispatchMessageToGroup(msg);


		// Dispatch the message to all targets

		case MESSAGE_DEST_ALL:
			return DispatchMessageToAll(msg);
		}


	return -1;
	}





DWORD MESSAGE_MGR::DispatchMessageToSingle(MESSAGE * msg)
	{

	///////////////////////////////////////////////////
	//												 //
	// Used to dispatch a message to a single target //
	//												 //
	///////////////////////////////////////////////////


	// Just return if the message is designated for an invalid target

	if (msg->To < 0 || msg->To >= NumTargets)
		return -1;


	// Dispatch the message to the target 

	return Target[msg->To].Target->OnMessage(msg->From, msg->Msg, msg->Data1, msg->Data2);
	}





DWORD MESSAGE_MGR::DispatchMessageToGroup(MESSAGE * msg)
	{

	//////////////////////////////////////////////////////
	//													//
	// Used to dispatch a message to a group of targets //
	//													//
	//////////////////////////////////////////////////////


	// Dispatch the message to targets that have the same GroupID as that of msg->To

	for (int t = 0; t < NumTargets; t++)
		if (Target[t].GroupID == msg->To)
			Target[t].Target->OnMessage(msg->From, msg->Msg, msg->Data1, msg->Data2);

	return -1;
	}





DWORD MESSAGE_MGR::DispatchMessageToAll(MESSAGE * msg)
	{

	///////////////////////////////////////////////
	//											 //
	// Used to dispatch a message to all targets //
	//											 //
	///////////////////////////////////////////////


	// Dispatch the message to all targets

	for (int t = 0; t < NumTargets; t++)
		Target[t].Target->OnMessage(msg->From, msg->Msg, msg->Data1, msg->Data2);

	return -1;
	}