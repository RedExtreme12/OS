#include <iostream>
#include <Windows.h>

#include "../Receiver/Event.h"
#include "../Receiver/Process.h"
#include "../Receiver/Semaphore.h"
#include "../Receiver/globals.h"
#include "../Receiver/Mutex.h"
#include "../Receiver/WinException.h"

void consumer_producers(FILE* binary_file, Semaphore full_semaphore, Semaphore empty_semaphore, Mutex mutex)
{
	char operation;
	char message[message_length];

	int rear;
	if (binary_file)
	{
		for (;;)
		{
			printf("Input 's' to send message or 'q' to quit: ");
			std::cin >> operation;

			if (operation == 's')
			{
				printf("Input message (less than 20 symbols): ");
				fgets(message, message_length, stdin);
				message[strlen(message) - 1] = 0;

				empty_semaphore.wait();
				mutex.acquire();

				//putProduct(); 
				rewind(binary_file);
				fread(&rear, sizeof(int), 1, binary_file);

				fseek(binary_file, sizeof(int) + sizeof(char) * message_length * rear, SEEK_SET);
				fwrite(message, sizeof(char), message_length, binary_file);

				rear++;
				rear %= number_of_messages;

				rewind(binary_file);
				fwrite(&rear, sizeof(int), 1, binary_file);

				fflush(binary_file);

				mutex.release();
				full_semaphore.signal();
			}
			else if (operation == 'q')
				break;
		}
	}
}

int main(int argc, char* argv[])
{
    FILE* binary_file;
    fopen_s(&binary_file, argv[1], "r+b");

    int process_number = atoi(argv[2]);

    std::string event_name = event_name_format + std::to_string(process_number);

	try
	{
		Event event = Event((DWORD)EVENT_ALL_ACCESS, FALSE, event_name);
		event.set_event();

		Mutex mutex = Mutex(SYNCHRONIZE, FALSE, mutex_name);
		Semaphore empty_semaphore = Semaphore((DWORD)SYNCHRONIZE, FALSE, empty_semaphore_name);
		Semaphore full_semaphore = Semaphore((DWORD)SEMAPHORE_MODIFY_STATE, FALSE, full_semaphore_name);

		consumer_producers(binary_file, full_semaphore, empty_semaphore, mutex);
	}
	catch (WinException& ex)
	{
		std::cout << ex.what() << " with error code " << ex.get_error_code() << std::endl;
		return 1;
	}

	fclose(binary_file);
    system("pause");
}