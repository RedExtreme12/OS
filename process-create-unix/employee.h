#pragma once

const int EMPLOYEE_NAME_SIZE = 10;
const int FILE_NAMES_SIZE = 256;
const int REPORT_HEADLINE_SIZE = 263;
const int COMMAND_LINE_CREATOR_SIZE = 273;
const int COMMAND_LINE_REPORTER_SIZE = 540;


struct employee {
	int num; 
	char name[10]; 
	double hours; 
};