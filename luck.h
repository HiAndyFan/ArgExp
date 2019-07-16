#ifndef __LUCK_H__
#define __LUCK_H__
#include <stdio.h>
#define __LUCK_H_VERSION_1
//May the bug be with you.

//typo
#define ture true
#define flase false
#define viod void
#define TRUE true
#define FALSE false
#define mian main

//short type name
#define u_int unsigned int
#define u_long unsigned long
#define u_short unsigned short
#define byte char
#define NULL 0

inline const char* strBool(bool b)
{
	if (b)
	{
		return "True";
	}
	return "False";
}

#if _DEBUG
#define isDebug true
#define isRelease false
#else
#define isDebug false
#define isRelease true
#endif

inline int debug(const char* str)
{
#if _DEBUG
	printf("debug:");
	printf("%s", str);
	printf("\n");
#endif
	return 0;
}

inline int debug(const int i)
{
#if _DEBUG
	printf("debug:");
	printf("%d", i);
	printf("\n");
#endif
	return 0;
}

inline int debug(const double doe)
{
#if _DEBUG
	printf("debug:");
	printf("%lf", doe);
	printf("\n");
#endif
	return 0;
}



#endif