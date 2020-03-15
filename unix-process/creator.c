#include "employee.h"
#include <stdio.h>
#include <stdlib.h>

void fill_struct_arr_from_console(struct employee* employees, int num_of_employees)
{
	for (int i = 0; i < num_of_employees; i++)
	{
		printf("Enter employee id, name and number of hours worked: ");
		scanf("%d %s %lf", &employees[i].num, employees[i].name, &employees[i].hours);
	}
}

void write_to_binary_file(struct employee* employees, int num_of_employees, FILE* output_file)
{
	fwrite(employees, sizeof(struct employee), num_of_employees, output_file);
}

void write_num_of_employees_to_binary_file(int num_of_employees, FILE* output_file)
{
	fprintf(output_file, "%d", num_of_employees);
}

int main(int argc, char* argv[]) {
	FILE* output_file = fopen(argv[0], "wb");

	int num_of_employees = atoi(argv[1]);
	if (output_file != 0) {
		write_num_of_employees_to_binary_file(num_of_employees, output_file);

		struct employee* employees = malloc(sizeof(struct employee) * num_of_employees);

		fill_struct_arr_from_console(employees, num_of_employees);

		write_to_binary_file(employees, num_of_employees, output_file);

		free(employees);
		fclose(output_file);
	}
	else
	{
		return 1;
	}

	return 0;
}