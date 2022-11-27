#include <stdio.h>

void Log() {
	printf("\033[36m[log]: ");
}

void WarnLog() {
	printf("\033[33m[warning]: ");
}

void ErrLog() {
	printf("\033[31m[error]: ");
}

void EndLog() {
	printf("\033[0m\n");
}
