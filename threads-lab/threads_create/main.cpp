#include "average.h"
#include "min_max.h"
#include "process.h"

#include <iostream>

void enter_array(std::vector <int> &arr, const int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("Enter element %d = ", i);

        int temp;
        std::cin >> temp;

        arr.push_back(temp);
    }
}

void print_array(std::vector <int> arr)
{   
    for (int i = 0; i < arr.size(); i++)
    {
        printf("Element_%d = %d\n", i, arr[i]);
    }
}

DWORD thread_work(LPTHREAD_START_ROUTINE func, vect &data)
{
    HANDLE hThread;
    DWORD dwThread;

    hThread = CreateThread(NULL, 0, func, &data, 0, &dwThread);
    if (hThread == NULL)
        return GetLastError();

    if (WaitForSingleObject(hThread, INFINITE) == WAIT_FAILED)
        return GetLastError();

    CloseHandle(hThread);

    return 0;
}

void swap_min_max_average(vect &data)
{
    data.arr[data.min_id] = data.average;
    data.arr[data.max_id] = data.average;
}

int main()
{
    int size;

    vect mma_struct;

    mma_struct.min_id = 0;
    mma_struct.max_id = 0;

    std::cout << "Enter array size: " << std::endl;
    std::cin >> size;

    enter_array(mma_struct.arr, size);
    print_array(mma_struct.arr);

    DWORD error_code;
        
    if ((error_code = thread_work((LPTHREAD_START_ROUTINE)get_average, mma_struct)) != 0)
        return error_code;

    if ((error_code = thread_work((LPTHREAD_START_ROUTINE)min_max, mma_struct)) != 0)
        return error_code;
    
    swap_min_max_average(mma_struct);

    print_array(mma_struct.arr);

    return 0;
}