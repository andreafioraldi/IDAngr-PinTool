#include "pin.H"

#include <iostream>
#include <sstream>

//from https://stackoverflow.com/questions/42711201/output-a-c-string-including-all-escape-characters
std::ostream& stringify(std::ostream& out, std::string const& s)
{
	for (auto ch : s)
	{
		switch (ch)
		{
		case '\'':
			out << "\\'";
			break;
		case '\"':
			out << "\\\"";
			break;
		case '\?':
			out << "\\?";
			break;
		case '\\':
			out << "\\\\";
			break;
		case '\a':
			out << "\\a";
			break;
		case '\b':
			out << "\\b";
			break;
		case '\f':
			out << "\\f";
			break;
		case '\n':
			out << "\\n";
			break;
		case '\r':
			out << "\\r";
			break;
		case '\t':
			out << "\\t";
			break;
		case '\v':
			out << "\\v";
			break;
		default:
			out << ch;
		}
	}
	return out;
}

BOOL GetStrPid(string* result)
{
	std::ostringstream ss;
	ss << PIN_GetPid();
	*result = ss.str();
	return TRUE;
}

#ifdef _WIN32

namespace WINH {
#include "WinHeaderStub.h"
}

#define SEG_PROT_R 4
#define SEG_PROT_W 2
#define SEG_PROT_X 1

using namespace std;

BOOL VMMap(string* result)
{
	WINH::LPBYTE base = NULL;
	WINH::MEMORY_BASIC_INFORMATION mbi;
	ostringstream ss;

	BOOL has_map = FALSE;

	while (WINH::VirtualQuery(base, &mbi, sizeof(WINH::MEMORY_BASIC_INFORMATION)) > 0)
	{
		has_map = TRUE;
		ss << "[" << (size_t) mbi.BaseAddress << "," << (size_t) mbi.BaseAddress + mbi.RegionSize  << ",";

		int mapperm = 0;
		if (mbi.Protect & PAGE_EXECUTE)
			mapperm = SEG_PROT_X;
		else if (mbi.Protect & PAGE_EXECUTE_READ)
			mapperm = SEG_PROT_X | SEG_PROT_R;
		else if (mbi.Protect & PAGE_EXECUTE_READWRITE)
			mapperm = SEG_PROT_X | SEG_PROT_R | SEG_PROT_W;
		else if (mbi.Protect &  PAGE_EXECUTE_WRITECOPY)
			mapperm = SEG_PROT_X | SEG_PROT_R;
		//else if (mbi.Protect & PAGE_NOACCESS)
		//	mapperm = 0;
		else if (mbi.Protect & PAGE_READONLY)
			mapperm = SEG_PROT_R;
		else if (mbi.Protect & PAGE_READWRITE)
			mapperm = SEG_PROT_R | SEG_PROT_W;
		else if (mbi.Protect & PAGE_WRITECOPY)
			mapperm = SEG_PROT_R;

		ss << mapperm << ",";

		IMG img = IMG_FindByAddress((ADDRINT) mbi.BaseAddress);
		if (IMG_Valid(img))
		{
			ss << "\"";
			stringify(ss, IMG_Name(img));
			ss << "\"";
		}
		else
			ss << "\"<no name>\"";
		ss << "],";
		
		base += mbi.RegionSize;
	}

	*result = "[" + ss.str();
	result->pop_back();
	*result += "]";

	return has_map;
}

#else

#include <fstream>

BOOL VMMap(string* result)
{
	// TODO read /proc/pid/maps

	return FALSE;
}

#endif
