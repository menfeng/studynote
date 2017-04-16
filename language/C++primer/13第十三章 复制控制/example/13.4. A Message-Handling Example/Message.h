#include "stdafx.h"

class Folder;
/*****************************Message***********************************/
class Message {
public:
	// folders is initialized to the empty set automatically
	Message(const std::string &str = ""):contents (str) { }
	  // copy control: we must manage pointers to this Message
	  // from the Folders pointed to by folders
	  Message(const Message&);
	  Message& operator=(const Message&);
	  ~Message();
	  // add/remove this Message from specified Folder's set of messages
	  void save (Folder&);
	  void remove(Folder&);
	  void addFldr( Folder* fldr);
	  void remFldr(Folder* fldr); 
private:
	std::string contents;      // actual message text
	std::set<Folder*> folders; // Folders that have this Message
	// Utility functions used by copy constructor, assignment, and destructor:
	// Add this Message to the Folders that point to the parameter
	void put_Msg_in_Folders(const std::set<Folder*>&);
	// remove this Message from every Folder in folders
	void remove_Msg_from_Folders();
};

/************************************Folder*****************************************/
class  Folder 
{ 
public: 
	Folder()    { } 
	Folder( const Folder&); 
	Folder&  operator =( const Folder&); 
	~Folder(); 

	void  save( Message &); 
	void  remove(Message&); 

	void  addMsg( Message *); 
	void  remMsg( Message *); 

private : 
	std::set<Message*> messages; 
	void  put_Fldr_In_Messages( const std::set<Message*>&); 
	void  remove_Fldr_Messages(); 
}; 

