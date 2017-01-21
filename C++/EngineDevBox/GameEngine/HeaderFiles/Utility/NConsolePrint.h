#pragma once

#ifndef __ENGINE__NCONSOLEPRINT__H__
#define __ENGINE__NCONSOLEPRINT__H__

namespace Illehc
{
	void ConsolePrint(const char * i_pFmt, const char * i_pFile, unsigned int i_Line, ...);
} // namespace Illehc

// This is how we handle variable argument pre-processor macros
// This is only supported by compilers that support the C99 standard
#if defined(_DEBUG)  &&  !defined(DISABLE_DEBUG_PRINT)
#define DEBUG_PRINT(fmt,...) Illehc::ConsolePrint((fmt),__FILE__,__LINE__,__VA_ARGS__)
#else
// This is one way we create a do nothing (NO OP) macro that doesn't
// generate a compiler warning or error
#define DEBUG_PRINT(fmt,...) void(0)
#endif

#endif // !__ENGINE__NCONSOLEPRINT__H__