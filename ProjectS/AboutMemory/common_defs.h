/************************************************************************/
/*
* Author:      Clouderman
* Created:     2012-11-06
* Filename:    common_defs.h
* Purpose:     common defines.
* Note:
*/
/************************************************************************/
#ifndef RAINBOW_COMMON_DEFS_H_
#define RAINBOW_COMMON_DEFS_H_

//////////////////////////////////////////////////////////////////////////
// Always compiler using Unicode.
#ifndef UNICODE
#define UNICODE
#endif

// When using Unicode Windows functions, use Unicode C-Runtime functions too.
#ifdef UNICODE
#ifndef _UNICODE
#define _UNICODE
#endif
#endif

//////////////////////////////////////////////////////////////////////////
#ifndef STRICT
#define STRICT
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

//////////////////////////////////////////////////////////////////////////
// key words dummy

#ifndef NOEXCEPT
#define NOEXCEPT    throw()
#endif

#if _MSC_VER < 1600
#define nullptr		NULL
#endif

#ifdef __cplusplus
#define EXTERN_C    extern "C"
#else
#define EXTERN_C    
#endif

//////////////////////////////////////////////////////////////////////////
// Pragma message helper macro
/*
When the compiler sees a line like this:
#pragma MESSAGE(Fix this later)

it outputs a line like this:

c:\CD\CmnHdr.h(82):Fix this later

You can easily jump directly to this line and examine the surrounding code.
*/
#ifndef STR2
#define STR2(x) #x
#endif
#ifndef STR
#define STR(x)  STR2(x)
#endif
#ifndef MESSAGE
#define MESSAGE(desc) message(__FILE__ "(" STR(__LINE__) "):" #desc)
#endif

//////////////////////////////////////////////////////////////////////////
// Assert / Verify Macros

#ifndef ASSERT
#define ASSERT(expr)    assert(expr)
#endif

#ifndef VERIFY
#ifndef NDEBUG
#define VERIFY(expr)    ASSERT(expr)
#else
#define VERIFY(expr)    (expr)
#endif
#endif

//////////////////////////////////////////////////////////////////////////
// _countof helper

#ifndef _countof
#ifndef __cplusplus
#define _countof(a) (sizeof(a) / sizeof(a[0]))
#else
extern "C++"
{
	template <typename T, size_t size>
	char(*__countof_helper(T(&a)[size]))[size];

#define _countof(a) (sizeof(*__countof_helper(a)) + 0)
}
#endif // !__cplusplus
#endif // !_countof

//////////////////////////////////////////////////////////////////////////
// debugPrintf & debugBreak & TRACE helper

inline void debugPrintf(const char* format, ...) NOEXCEPT
{
	char buf[1024] = { 0 };
	va_list ap;

	va_start(ap, format);
	vsprintf_s(buf, _countof(buf), format, ap);
	OutputDebugStringA(buf);
	va_end(ap);
}

inline void debugPrintf(const wchar_t* format, ...) NOEXCEPT
{
	wchar_t buf[1024] = { 0 };
	va_list ap;

	va_start(ap, format);
	vswprintf_s(buf, _countof(buf), format, ap);
	OutputDebugStringW(buf);
	va_end(ap);
}

#ifdef _X86_
inline void debugBreak() NOEXCEPT{ _asm { int 3 } }
#else
inline void debugBreak() NOEXCEPT{ DebugBreak(); }
#endif

#ifndef TRACE
#if !defined(NDEBUG) || defined(RELEASE_TRACE)
#define TRACE(format, ...)  debugPrintf(format, __VA_ARGS__)
#else
#define TRACE(format, ...)  ((void)0)
#endif
#endif

#ifndef TRACE_IF
#if !defined(NDEBUG) || defined(RELEASE_TRACE)
#define TRACE_IF(expr, format, ...) \
	if (expr) { TRACE(format, __VA_ARGS__); }
#else
#define TRACE_IF(expr, format, ...) (expr)
#endif
#endif

#ifndef TRACE_EQ
#if !defined(NDEBUG) || defined(RELEASE_TRACE)
#define TRACE_EQ(expr1, expr2, format, ...) \
	if ((expr1) == (expr2)) { TRACE(format, __VA_ARGS__); }
#else
#define TRACE_EQ(expr1, expr2, format, ...)	(expr1)
#endif
#endif

#ifndef TRACE_NE
#if !defined(NDEBUG) || defined(RELEASE_TRACE)
#define TRACE_NE(expr1, expr2, format, ...) \
	if ((expr1) != (expr2)) { TRACE(format, __VA_ARGS__); }
#else
#define TRACE_NE(expr1, expr2, format, ...) (expr1)
#endif
#endif

inline bool _is_numeric(LPCTSTR dat) {
	if (dat == nullptr)
		return false;

	for (size_t i = (dat[0] == _T('-') ? 1 : 0); i < _tcslen(dat); i++) {
		if (dat[i] < _T('0') || dat[i] > _T('9'))
			return false;
	}

	return true;
};

//////////////////////////////////////////////////////////////////////////

#endif // !RAINBOW_COMMON_DEFS_H_
