/******************************************************************************
** Student name: 	Sri Sai Teja Paturu
** Student number: 	s3644335
** Course: 			Advanced Programming Techniques - S2 2018
******************************************************************************/

#include "utility.h"

void *safeMalloc(size_t size)
{
    void *p = malloc(size);

    if(p == NULL)
    {
        perror(NULL);
        exit(EXIT_FAILURE);
    }

    return p;
}
