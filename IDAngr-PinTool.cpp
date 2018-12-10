#include "pin.H"
#include "IDAngr-Pin.h"

static BOOL DebugInterpreter(THREADID, CONTEXT *, const string &, string *, VOID *);

int main(int argc, char *argv[])
{
	PIN_InitSymbols();
	if( PIN_Init(argc,argv) ) {
		return 1;
	}

	PIN_AddDebugInterpreter(DebugInterpreter, 0);

	PIN_StartProgram();
	return 0;
}


static BOOL DebugInterpreter(THREADID tid, CONTEXT *ctxt, const string &cmd, string* result, VOID *)
{
	if (cmd == "idangr_getpid")
	{
		return GetStrPid(result);
	}
	if (cmd == "idangr_vmmap")
	{
		return VMMap(result);
	}
	return FALSE;
}
