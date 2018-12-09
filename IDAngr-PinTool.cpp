#include "pin.H"
#include <sstream>

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


static BOOL DebugInterpreter(THREADID, CONTEXT *ctxt, const string &cmd, string* result, VOID *)
{
    if (cmd == "getpid")
    {
        std::ostringstream ss;
        ss << PIN_GetPid();
        *result = ss.str();
        return TRUE;
    }
    return FALSE;
}
