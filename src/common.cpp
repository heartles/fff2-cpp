#include "common.h"

#ifdef WINDOWS
#include <Windows.h>
#endif

using namespace std;

void
Log(string s)
{
#ifdef WINDOWS
    OutputDebugString(s.c_str());
#endif
    printf("%s\n", s.c_str());
}
