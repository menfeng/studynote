/*
 * Encapsulate a request as an object, 
 * thereby letting you parameterize clients with different requests, 
 * queue or log requests, and support undoable operations.
 */

#include "Receiver.h"
#include "Command.h"
#include "Invoker.h"

int main(int argc,char* argv[])
{
	Receiver *rec=new Receiver;
	Command *cmd=new SimpleCommand<Receiver>(rec,&Receiver::Action);
	Invoker *inv=new Invoker(cmd);
	inv->Invoke();
	return 0;
}
