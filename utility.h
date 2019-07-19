/******************************************************************************
** Student name: 	Sri Sai Teja Paturu
** Student number: 	s3644335
** Course: 			Advanced Programming Techniques - S2 2018
******************************************************************************/

#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef enum boolean
{
    FALSE = 0,
    TRUE
} Boolean;

/* Malloc with no memory leaks */
void *safeMalloc(size_t size);

#endif
