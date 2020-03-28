#ifndef __GLOBAL_H__
#define __GLOBAL_H__

//----------------------------------------------
// Cac khai bao thong so
#define CPU_CLOCK_HZ		50000000
#define TICK_RATE_HZ		1000

#define NORMAL		0
#define ERROR			1
#define WARNING		2

#define DISABLE		0
#define ENABLE		1

//----------------------------------------------
// Cac khai bao function
//----------------------------------------------
#define TRUE	1
#define FALSE	0

#define True	1
#define False	0

#define true	1
#define false	0

typedef union
{
	unsigned long LONG;
	unsigned int INT[2];
	unsigned char BYTE[4];
} DWORD;

typedef union
{
	unsigned int INT;
	unsigned char BYTE[2];
} WORD;

typedef unsigned char boolean;


#define _clear(x,y)  	x &= ~(1UL<<y) 	
#define _set(x,y)		x |= (1UL<<y)
#define _toggle(x,y)		x ^= (1UL<<y)
#define _isset(x,y)		((x & (1UL<<y)) != 0)
#define _isclear(x,y)	((x & (1UL<<y)) == 0)

#endif
