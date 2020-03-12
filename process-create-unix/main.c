#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "employee.h"
#include <stdlib.h>

pid_t fork(void);

void read_and_print_from_binary(int num_of_queries, FILE* input)
{
	for (int i = 0; i < num_of_queries; i++) {
		struct employee emp;
		fread(&emp, sizeof(struct employee), 1, input);

		printf("%d %s %lf\n", emp.num, emp.name, emp.hours);
	}
}

void procces_work(const char* procces_name, char* params[])
{
	pid_t pid = vfork();

	switch (pid)
	{
	case -1:
	{
		fprintf(stderr, "Unable to fork\n");
	}
	case 0:
	{
		if (execv(procces_name, params) == -1)
		{
			fprintf(stderr, "Unable to exec\n");
			break;
		}
	}
	default:
	{
		int status;
		waitpid(pid, &status, 0);
		int exit_code = WEXITSTATUS(status);

		if (exit_code) {
			printf("P> Exit code = %d\n", exit_code);
		}
		printf("\n");
		break;
	}
	}
}

int main() {
	char binary_file_name[FILE_NAMES_SIZE];

	printf("Enter binary file name: ");
	scanf("%s", binary_file_name, FILE_NAMES_SIZE);
	binary_file_name[FILE_NAMES_SIZE - 1] = 0;

	char num_of_queries[10];
	printf("Enter number of queries: ");
	scanf("%s", num_of_queries);

	// Creator
	char* creator_params[3];

	creator_params[0] = binary_file_name;
	creator_params[1] = num_of_queries;

	procces_work("Creator", creator_params);

	FILE* binary_input = fopen(binary_file_name, "rb");

	if (binary_input)
	{
		int num;
		fscanf(binary_input, "%d", &num);

		read_and_print_from_binary(num, binary_input);

		fclose(binary_input);
	}


	char report_file_name[FILE_NAMES_SIZE];

	printf("Enter report file name: ");
	scanf("%s", report_file_name, FILE_NAMES_SIZE);
	report_file_name[FILE_NAMES_SIZE - 1] = 0;

	char hourly_payment[15];

	printf("Enter hourly payment: ");
	scanf("%s", hourly_payment);

	// Reporter
	char* reporter_params[10];

	reporter_params[0] = binary_file_name;
	reporter_params[1] = report_file_name;
	reporter_params[2] = hourly_payment;

	procces_work("Reporter", reporter_params);

	FILE* report_file = fopen(report_file_name, "r");

	if (report_file)
	{
		if (report_file != 0)
		{
			char line[100];
			while (fgets(line, 100, report_file) != NULL)
				printf("%s", line);

			fclose(report_file);
		}
	}

	return 0;
}