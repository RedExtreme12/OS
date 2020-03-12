#include "employee.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	FILE* output_file = fopen(argv[0], "wb");

	int num_of_employees = atoi(argv[1]);
	if (output_file != 0) {
		fprintf(output_file, "%d", num_of_employees);

		struct employee* employees = malloc(sizeof(struct employee) * num_of_employees);

		for (int i = 0; i < num_of_employees; i++) {
			printf("Enter employee id, name and number of hours worked: ");

			scanf("%d %s %lf", &employees[i].num, employees[i].name, &employees[i].hours);
		}

		fwrite(employees, sizeof(struct employee), num_of_employees, output_file);

		free(employees);
		fclose(output_file);
	}
	return 0;
}