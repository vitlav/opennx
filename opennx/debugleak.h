#ifdef DEBUG
#ifndef _DEBUGLEAK_H_
#ifdef __WXMSW__
#define _DEBUGLEAK_H_
#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#else
#define DEBUG_NEW new
#endif
#define new DEBUG_NEW
#endif
#endif
#endif