#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>

#define PLUS '+'
#define MINUS '-'
#define LEFT '<'
#define RIGHT '>'
#define LOOP_START '['
#define LOOP_END ']'
#define PRINT '.'
int syntaxCheaker(FILE *fp);
int execute(FILE *code, int startPoint, char *tape, char *ptr);

int main(int argc, char *argv[])
{
	FILE *fp;
	char *filename;
	static char tape[30000] = {0};
	char *ptr = tape;
	if (argc < 2)
	{
		fprintf(stderr, "File not found. i.e brainf code.bf");
		return 1;
	}
	filename = argv[1];
	fp = fopen(filename, "r");
	if (fp)
	{
		//sytax cheker
		if (syntaxCheaker(fp) != 0)
		{
			printf("Sytax Error mising ] or [");
			return 1;
		}
		//execute code
		if (execute(fp, 0, tape, ptr) == -1)
		{
			printf("index out of bound ");
		}
	}
	else
	{
		printf("Failed to open the file\n");
	}
	fclose(fp);
	return 0;
}
/*
Name: syntaxCheaker
Input: file descriptor
Output: 0 if no syntax error.
Algo: for each [ the count go up and for each ] it's go down if the sum is not equl to zero the code is wrong.
*/
int syntaxCheaker(FILE *fp)
{
	char ch;
	int loopCheck = 0;
	fseek(fp, 0, SEEK_SET);
	while ((ch = fgetc(fp)) != EOF)
	{
		//set if there are equal number
		if (ch == LOOP_START)
		{
			loopCheck++;
		}
		if (ch == LOOP_END)
		{
			loopCheck--;
		}
	}
	return loopCheck;
}
/*
Name:
Input
Output:
Algo:
*/
int execute(FILE *code, int startPoint, char *tape, char *startPtr)
{

	//use pointer to mange the tape
	int startAt = startPoint;
	int currentIndex;
	int stopAt;
	// cmd the current letter of command
	char cmd;
	char *ptr = startPtr;

	fseek(code, startAt, SEEK_SET);
	while ((cmd = fgetc(code)) != EOF)
	{
		switch (cmd)
		{
		case PLUS:
			(*ptr)++;
			break;
		case MINUS:
			(*ptr)--;
			break;
		case LEFT:
			if ((--ptr - tape) < 0)
			{
				printf("if ((--ptr - tape) < 0)\n");
				return -1;
			}
			break;
		case RIGHT:
			if ((++ptr - tape) > 3000)
			{
				printf("if ((++ptr - tape) > 30000)\n");
				return -1;
			}
			break;
		case LOOP_START:
			currentIndex = ftell(code);
			stopAt = execute(code, currentIndex, tape, ptr);
			if (stopAt == -1)
			{
				printf("if (stopAt == -1)\n");
				return -1;
			}
			fseek(code, stopAt, SEEK_SET);
			break;
		case LOOP_END:
			if (*ptr == 0)
			{
				currentIndex = ftell(code);
				return currentIndex;
			}
			else
			{
				fseek(code, startAt, SEEK_SET);
			}
			break;
		case PRINT:
			printf("%c", *ptr);
			break;
		}
	}
	return 1;
}
