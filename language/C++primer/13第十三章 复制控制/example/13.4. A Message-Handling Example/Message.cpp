#include "stdafx.h"
#include "Message.h"
/*****************************Message*****************************************/
Message::Message(const Message &m):
contents(m.contents), folders(m.folders)
{
	// add this Message to each Folder that points to m
	put_Msg_in_Folders(folders);
}

// add this Message to Folders that point to rhs
void Message::put_Msg_in_Folders(const set<Folder*> &rhs)
{
	for(std::set<Folder*>::const_iterator beg = rhs.begin();
		beg != rhs.end(); ++beg)
		(*beg)->addMsg(this);     // *beg points to a Folder
}

Message& Message::operator=(const Message &rhs)
{
	if (&rhs != this) {
		remove_Msg_from_Folders(); // update existing Folders
		contents = rhs.contents;   // copy contents from rhs
		folders = rhs.folders;     // copy Folder pointers from rhs
		// add this Message to each Folder in rhs
		put_Msg_in_Folders(rhs.folders);
	}
	return *this;
}

// remove this Message from corresponding Folders
void Message::remove_Msg_from_Folders()
{
	// remove this message from corresponding folders
	for(std::set<Folder*>::const_iterator beg =
		folders.begin (); beg != folders.end (); ++beg)
		(*beg)->remMsg(this); // *beg points to a Folder
}

Message::~Message()
{
	remove_Msg_from_Folders();
}

void  Message::save(Folder &fldr) 
{ 
	addFldr(&fldr); 
	fldr.addMsg( this ); 
} 

void  Message::remove(Folder & fldr) 
{ 
	remFldr(&fldr); 
	fldr.remMsg( this ); 
} 

void Message::addFldr( Folder* fldr ) 
{ 
	folders.insert(fldr); 
} 
void Message::remFldr(Folder* fldr) 
{ 
	folders.erase(fldr); 
} 



/************************************Folder*****************************************/
Folder::Folder( const Folder &f ):messages(f.messages) 
{ 
	put_Fldr_In_Messages(messages); 
} 

void  Folder::put_Fldr_In_Messages( const std::set<Message*> &rhs) 
{ 
	for(std::set<Message*>::const_iterator beg = 
		rhs.begin(); beg != rhs.end(); ++beg) 
		(*beg)->addFldr( this ); 
} 

Folder& Folder:: operator  =(const Folder &rhs) 
{ 
	if  (&rhs !=  this  ) 
	{ 
		remove_Fldr_Messages(); 
		messages = rhs.messages; 
		put_Fldr_In_Messages(rhs.messages); 
	} 
	return  * this ; 
} 

void  Folder::remove_Fldr_Messages() 
{ 
	for (std::set<Message*>::const_iterator beg = messages.begin(); 
		beg != messages.end(); ++beg ) 
	{ 
		(*beg)->remFldr( this ); 
	} 
} 

Folder::~Folder() 
{ 
	remove_Fldr_Messages(); 
} 

void  Folder::save(Message & msg) 
{ 
	addMsg(&msg); 
	msg.addFldr( this ); 
} 
void  Folder::remove(Message& msg) 
{ 
	remMsg(&msg); 
	msg.remFldr( this ); 
} 

void  Folder::addMsg(Message * msg) 
{ 
	messages.insert(msg); 
} 
void  Folder::remMsg(Message * msg) 
{ 
	messages.erase(msg); 
}





