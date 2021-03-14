#include <iostream>
#include <Windows.h>
#include <ctime>

CRITICAL_SECTION csArray;

struct params
{
	int n;
	int* mas;
	int index;
	HANDLE hBeginWorkEvent;
	HANDLE hStopWorkEvent;
	HANDLE* hContinueOrTerminateWork;
};

void reset_elements_of_array_with_thread_index(params* parameters) 
{
	EnterCriticalSection(&csArray);

	for (int i = 0; i < parameters->n; i++)
	{
		if (parameters->mas[i] == (parameters->index + 1))
			parameters->mas[i] = 0;
	}

	LeaveCriticalSection(&csArray);
}

bool is_thread_terminated(params* parameters) 
{
	return WaitForSingleObject(parameters->hContinueOrTerminateWork[1], 0) == WAIT_OBJECT_0;
}

void continue_thread_work(params* parameters) 
{
	ResetEvent(parameters->hContinueOrTerminateWork[0]);
}

DWORD WINAPI marker(LPVOID param)
{
	params* parameters = (params*)param;
	int editsCount = 0;
	bool endWork = false;
	srand(parameters->index);

	WaitForSingleObject(parameters->hBeginWorkEvent, INFINITE); // Expect a signal from main.

	while (!endWork) 
	{
		int random_index = rand() % parameters->n;
		EnterCriticalSection(&csArray);

		if (parameters->mas[random_index] == 0) 
		{
			Sleep(5);
			parameters->mas[random_index] = parameters->index + 1; 
			LeaveCriticalSection(&csArray);

			editsCount++;
			Sleep(5);
		}

		else 
		{
			std::cout << "\nThread " << parameters->index + 1 << " marked total " << editsCount;
			std::cout << " elements, can`t mark element on " << random_index + 1 << " index\n";

			LeaveCriticalSection(&csArray);

			// Wait for a response to continue or exit from the thread.
			SetEvent(parameters->hStopWorkEvent);
			WaitForMultipleObjects(2, parameters->hContinueOrTerminateWork, FALSE, INFINITE);

			if (is_thread_terminated(parameters))
				endWork = true;
			else
				continue_thread_work(parameters);
		}
	}

	reset_elements_of_array_with_thread_index(parameters);

	return 0;
}

void print_array(int* mas, int n) {
	std::cout << "Array : \n";

	for (int i = 0; i < n; i++) {
		std::cout << mas[i] << " ";
	}

	std::cout << std::endl;
}

void fill_array_zeros(int* arr, int _size)
{
	for (int i = 0; i < _size; i++)
	{
		arr[i] = 0;
	}
}

void init_threads(params* parameters, HANDLE hBeginWorkEvent, HANDLE* hStopWorkEvents, 
				  HANDLE* hThreads, int number_of_markers, int* mas, int n, DWORD* IDThread)
{
	for (int i = 0; i < number_of_markers; i++) 
	{
		parameters[i].hBeginWorkEvent = hBeginWorkEvent;
		parameters[i].hStopWorkEvent = hStopWorkEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		parameters[i].index = i;
		parameters[i].hContinueOrTerminateWork = new HANDLE[2];
		parameters[i].mas = mas;
		parameters[i].n = n;

		for (int j = 0; j < 2; j++) 
		{
			parameters[i].hContinueOrTerminateWork[j] = CreateEvent(NULL, TRUE, FALSE, NULL);
		}

		hThreads[i] = CreateThread(NULL, 0, marker, (void*)&parameters[i], 0, &IDThread[i]);
	}
}

void terminate_thread(bool* terminated_threads, params* parameters, HANDLE* hThread, int threadIndexToTerminate, bool* is_thread_deleted)
{
	if (!terminated_threads[threadIndexToTerminate])
	{
		SetEvent(parameters[threadIndexToTerminate].hContinueOrTerminateWork[1]);
		WaitForSingleObject(hThread[threadIndexToTerminate], INFINITE);

		*is_thread_deleted = true;
		terminated_threads[threadIndexToTerminate] = true;
	}
	else
		std::cout << "This thread has already finished his work.";
}

void signal_to_continue_work_threads(params* parameters, int number_of_markers, bool* terminated_threads)
{
	for (int i = 0; i < number_of_markers; i++)
	{
		if (!terminated_threads[i])
		{
			ResetEvent(parameters[i].hStopWorkEvent);
			SetEvent(parameters[i].hContinueOrTerminateWork[0]);
		}
	}
}

int main() 
{
	InitializeCriticalSection(&csArray);
	int n;

	std::cout << "Enter the size of array: ";
	std::cin >> n;

	int* mas = new int[n];

	fill_array_zeros(mas, n);

	int number_of_markers;
	std::cout << "Enter the number of markers: ";
	std::cin >> number_of_markers;

	HANDLE* hThread = new HANDLE[number_of_markers];
	HANDLE* hStopWorkEvents = new HANDLE[number_of_markers];
	HANDLE hBeginWorkEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	DWORD* IDThread = new DWORD[number_of_markers];
	params* parameters = new params[number_of_markers];
	bool* terminated_threads = new bool[number_of_markers];

	for (int i = 0; i < number_of_markers; i++) 
	{
		terminated_threads[i] = false;
	}

	init_threads(parameters, hBeginWorkEvent, hStopWorkEvents, hThread, number_of_markers, mas, n, IDThread);

	if (hBeginWorkEvent)
		SetEvent(hBeginWorkEvent);
	else
		return 0;

	for (int i = 0; i < number_of_markers && i < n; i++) 
	{
		WaitForMultipleObjects(number_of_markers, hStopWorkEvents, TRUE, INFINITE); 
		print_array(mas, n);

		bool is_thread_deleted = false;

		while (!is_thread_deleted)
		{
			int threadIndexToTerminate;

			std::cout << "\nEnter the index of thread which you want to terminate (it starts from 1): ";
			std::cin >> threadIndexToTerminate;
			threadIndexToTerminate--;

			if (threadIndexToTerminate >= number_of_markers || threadIndexToTerminate < 0) 
			{
				std::cout << "This thread isn`t exist";
				continue;
			}

			terminate_thread(terminated_threads, parameters, hThread, threadIndexToTerminate, &is_thread_deleted);
		}

		print_array(mas, n);

		signal_to_continue_work_threads(parameters, number_of_markers, terminated_threads);
	}

	std::cout << "\nAll threads finished work\n";

	for (int i = 0; i < number_of_markers; i++)
	{
		if (hThread[i])
			CloseHandle(hThread[i]);
	}

	delete[] hThread;
	delete[] IDThread;
	delete[] mas;

	DeleteCriticalSection(&csArray);
	return 0;
}