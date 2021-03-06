#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include "../server/employee.h"

#define MESSAGE_SIZE 16

int main(int argc, char* argv[]) {
	HANDLE hPipe;
	char message[MESSAGE_SIZE];
	BOOL fSuccess = FALSE;

	const char* lpszPipenameFormat = "\\\\%s\\pipe\\employee_pipe";
	char pipename[80];

	if (argc > 1) {
		sprintf(pipename, lpszPipenameFormat, argv[1]);
	}
	else {
		sprintf(pipename, lpszPipenameFormat, ".");
	}

	if (!WaitNamedPipe(pipename, NMPWAIT_WAIT_FOREVER)) {
		printf("WaitNamedPipe failed, %d\n", GetLastError());
		return -1;
	}

	hPipe = CreateFile(
		pipename,
		GENERIC_READ |
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);

	if (hPipe == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile failed, %d\n", GetLastError());
		return -1;
	}
	
	for (;;) {
		char operation;

		printf("Enter 'w' to modify record, 'r' to read record, 'q' to quit: ");
		scanf("%c", &operation);
		getchar();

		if (operation == 'q') {
			if (!WriteFile(hPipe, "QUIT", 5, NULL, NULL)) {
				printf("WriteFile failed, %d\n", GetLastError());
				return -1;
			}

			break;
		}

		int numOfRecord;

		printf("Enter number of record: ");
		scanf("%d", &numOfRecord);
		getchar();

		if (operation == 'w') 
		{
			sprintf(message, "WRITE %d", numOfRecord);
		}
		else if (operation == 'r') 
		{
			sprintf(message, "READ %d", numOfRecord);
		}

		if (!WriteFile(hPipe, message, MESSAGE_SIZE * sizeof(char), NULL, NULL)) 
		{
			printf("WriteFile failed, %d\n", GetLastError());
			return -1;
		}

		struct Employee tp;

		if (!ReadFile(hPipe, &tp, sizeof(struct Employee), NULL, NULL)) 
		{
			printf("ReadFile failed, %d\n", GetLastError());
			return -1;
		}

		printf("Employee N%d { %d, %s, %lf }\n", numOfRecord, tp.num, tp.name, tp.hours);

		if (operation == 'w') 
		{
			printf("Enter employee id: ");
			scanf("%d", &tp.num);
			getchar();

			char name[11];

			printf("Enter employee name: ");
			fgets(name, 11, stdin);
			name[strlen(name) - 1] = 0;
			sprintf(tp.name, "%s", name);

			printf("Enter count of hours worked: "); // double
			scanf("%lf", &tp.hours);
			getchar();

			printf("Enter 's' to send new record: ");
			getchar();
			getchar();

			if (!WriteFile(hPipe, &tp, sizeof(struct Employee), NULL, NULL)) {
				printf("WriteFile failed, %d\n", GetLastError());
				return -1;
			}

			sprintf(message, "RELEASE WRITE");
		}
		else if (operation == 'r') 
		{
			sprintf(message, "RELEASE READ");
		}

		printf("Enter 'e' to end access to record: ");
		getchar();
		getchar();

		if (!WriteFile(hPipe, message, MESSAGE_SIZE * sizeof(char), NULL, NULL)) 
		{
			printf("WriteFile failed, %d\n", GetLastError());
			return -1;
		}
	}

	CloseHandle(hPipe);

	system("pause");

	return 0;
}