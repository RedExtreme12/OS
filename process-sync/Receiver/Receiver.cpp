#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ios>
#include <stdio.h>

#include <Windows.h>
#include "Process.h"
#include "Event.h"
#include "Mutex.h"
#include "Semaphore.h"
#include "WinException.h"
#include "globals.h"

void enter_file_name(std::string& file_name)
{
    std::cout << "Enter file name: ";
    std::cin >> file_name;  
}

void enter_number_of_senders(int& number_of_senders)
{
    std::cout << "Enter number of senders: ";
    std::cin >> number_of_senders;
}

std::string concatenate(std::string process_exe_name, std::string file_name, int number)
{
    std::string lpCommandLine = process_exe_name;

    lpCommandLine.append(" ");
    lpCommandLine.append(file_name);
    lpCommandLine.append(" ");
    lpCommandLine.append(std::to_string(number));

    return lpCommandLine;
}

DWORD processes_work(const int number_of_senders, std::vector <HANDLE> &hEvents, const std::string binary_file_name)
{
    for (size_t i = 0; i < number_of_senders; i++)
    {
        std::string event_name = event_name_format + std::to_string(i);

        std::string command_line_creator = concatenate("Sender.exe", binary_file_name, i);
        Process process = Process(command_line_creator);

        try
        {
            Event _event = Event(FALSE, FALSE, event_name);
            hEvents[i] = _event.get_event_handle();
        }
        catch (WinException &ex)
        {
            std::cout << ex.what() << " with error code " << ex.get_error_code() << std::endl;
            return 2;
        }

        try
        {
            process.work();
        }
        catch (WinException & ex)
        {
            std::cout << ex.what() << " with error code " << ex.get_error_code() << std::endl;
            return 3;
        }
    }

    return 0;
}   

void wait_events(std::vector <HANDLE> hEvents)
{
    std::cout << "Waiting for signals..." << std::endl;

    if (WaitForMultipleObjects(hEvents.size(), hEvents.data(), TRUE, INFINITE) == WAIT_FAILED)
    {
        throw WinException("Waiting Events failed", GetLastError());
    }

    std::cout << "Received all signals" << std::endl;
}

int consumer_producers(FILE* binary_file, Semaphore full_semaphore, Semaphore empty_semaphore, Mutex mutex)
{
    char operation;
    char message[message_length];

    int front = 0, rear = 0;
    if (binary_file)
    {
        fwrite(&rear, sizeof(int), 1, binary_file);
        fflush(binary_file);

        for (;;)
        {
            std::cout << "Input 'r' to read message from file or 'q' to quit: " << std::endl;
            std::cin >> operation;

            if (operation == 'r')
            {
                full_semaphore.wait();
                mutex.acquire();

                // Get product
                fseek(binary_file, sizeof(int) + sizeof(char) * message_length * front, SEEK_SET);
                fread(message, sizeof(char), message_length, binary_file);

                front++;
                front %= number_of_messages;

                mutex.release();
                empty_semaphore.signal();

                // Consume product
                printf("\nMessage: %s\n\n", message);
            }
            else if (operation == 'q')
                break;
        }
    }
    else
        return 5;
}

int main()
{
    std::string binary_file_name;   
    enter_file_name(binary_file_name);

    // File
    FILE* binary_file;
    fopen_s(&binary_file, binary_file_name.c_str(), "w+b");

    int number_of_senders;
    enter_number_of_senders(number_of_senders);    

    std::vector <HANDLE> hEvents(number_of_senders);

    try
    {
        Mutex mutex = Mutex(mutex_name);
        Semaphore empty_semaphore = Semaphore((LONG)number_of_messages, (LONG)number_of_messages, empty_semaphore_name);
        Semaphore full_semaphore = Semaphore((LONG)0, (LONG)number_of_messages, full_semaphore_name);

        DWORD error_code;
        if ((error_code = processes_work(number_of_senders, hEvents, binary_file_name)) != 0)
        {
            return error_code;
        }

        wait_events(hEvents);

        if ((error_code = consumer_producers(binary_file, full_semaphore, empty_semaphore, mutex)) != 0)
        {
            return error_code;
        }
    }
    catch (WinException& ex)
    {
        std::cout << ex.what() << " with error code " << ex.get_error_code() << std::endl;
        return 1;
    }

    return 0;
}