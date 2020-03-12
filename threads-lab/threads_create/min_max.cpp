#include "min_max.h"
#include <stdio.h>

DWORD WINAPI min_max(vect &our_vect)
{
	int min = our_vect.arr[0];
	int max = our_vect.arr[0];

	for (int i = 0; i < our_vect.arr.size(); i++)
	{
		if (our_vect.arr[i] > max)
		{
			max = our_vect.arr[i];
			our_vect.max_id = i;
		}			
		if (our_vect.arr[i] < min)
		{
			min = our_vect.arr[i];
			our_vect.min_id = i;
		}			

		Sleep(7);
	}

	printf("Min element = %d; Max element = %d\n", min, max);

	return 0;
}