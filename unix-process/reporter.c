#include "employee.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef double calculate_salary_t(const double, const int);

double calculate_salary(const double hourly_payment, const int hours)
{
	return hours * hourly_payment;
}

void write_to_textfile_employee_data(const struct employee* p, FILE* f, const int hourly_payment, calculate_salary_t func)
{
	double salary = func(hourly_payment, p->hours);
	fprintf(f, "%d %s %.3lf %.3lf\n", p->num, p->name, p->hours, salary);
}

void read_num_of_employees_from_binary(FILE* binary_input, int* num_of_employees)
{
	fscanf(binary_input, "%d", &num_of_employees);
}

void read_employees_from_binary(FILE* binary_input, struct employee* employees ,int num_of_employees)
{
	fread(employees, sizeof(struct employee), num_of_employees, binary_input);
}

int main(int argc, char** argv) {
	char binary_input_file_name[FILE_NAMES_SIZE];
	char report_file_name[FILE_NAMES_SIZE];

	sscanf(argv[0], "%s", binary_input_file_name, FILE_NAMES_SIZE);
	sscanf(argv[1], "%s", report_file_name, FILE_NAMES_SIZE);

	char report_headline[REPORT_HEADLINE_SIZE];
	snprintf(report_headline, sizeof(report_headline) - 1, "%s report\n", binary_input_file_name);

	double hourly_payment = strtod(argv[2], NULL);

	FILE* report_output = fopen(report_file_name, "w");
	FILE* binary_input = fopen(binary_input_file_name, "rb");

	if (report_output != 0 && binary_input != 0)
	{
		fwrite(report_headline, sizeof(char), strlen(report_headline), report_output);

		int num_of_employees;
		read_num_of_employees_from_binary(binary_input, &num_of_employees);

		struct employee* payments = malloc(sizeof(struct employee) * num_of_employees);
		read_employees_from_binary(binary_input, payments, num_of_employees);

		for (int i = 0; i < num_of_employees; i++) {
			write_to_textfile_employee_data(payments + i, report_output, hourly_payment, &calculate_salary);
		}

		free(payments);

		fclose(binary_input);
		fclose(report_output);
	}
	else
	{
		return 1;
	}

	return 0;
}