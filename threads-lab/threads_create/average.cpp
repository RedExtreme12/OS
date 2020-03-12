#include "average.h"
#include <stdio.h>


DWORD WINAPI get_average(vect &our_vect)
{
	our_vect.average = 0;

	for (int i = 0; i < our_vect.arr.size(); i++)
	{
		our_vect.average += our_vect.arr[i];
		Sleep(12);
	}

	if (our_vect.arr.size() != 0)
		our_vect.average /= our_vect.arr.size();

	printf("The average: %lf \n", our_vect.average);

	return 0;
}