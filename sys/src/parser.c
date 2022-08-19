#include "art.h"
#include <string.h>
#include <stdlib.h>
typedef struct
{
	char name[32];
	uint32_t val;
	//IN CASE DOUBLE VAL
	double dv;
	char type;
} var;
char *tokens[512][4096];
char tm[4096];
uint32_t token_count[512];
var vars[512];
uint32_t var_count = 0;
uint32_t *stack;
int sp;
int ss = 4096;
//BREAK TOKENS IN LINE
char *break_tokens(char *token, int i)
{
	token_count[i] = 0;
	token = strtok(token, " ");
	while (token != NULL)
	{
		for (int i = 0; i < strlen(token); i++)
		{
			if (token[i] == 2)
				token[i] = ' ';
		}
		tokens[i][token_count[i]] = token;
		token_count[i]++;
		token = strtok(NULL, " ");
	}
}

void regiser_var(char *n)
{
	var *vari = (var *)&vars[var_count];
	memcpy(vari->name, n, 32);
	vari->type = 0;
	vari->val = 0;
	var_count++;
}

//GET VARIABLE INDEX
uint32_t gvi(char *n)
{
	for (uint32_t i = 0; i < var_count; i++)
	{
		if (!strcmp(n, vars[i].name))
		{
			return i;
		}
	}
	printf("lol");
	return 0;
}
void wv(char *n, uint32_t v)
{
	vars[gvi(n)].val = v;
}

int call(int lr, int line)
{
	int out = lr;
	stack[sp] = line;
	sp++;
	if (sp > 4096)
	{
		printf("\nStack Overflow\n");
		return -1;
	}
	return out;
}
int ret()
{
	if (sp == 0)
	{
		printf("\nStack Underflow\n");
		return -1;
	}
	sp--;
	uint32_t out = stack[sp];

	return out;
}
char *tsr(char *sr, int r)
{
	char *temp;
	if (r == 1)
		temp = malloc(4096);
	else
		temp = tm;
	int n = strlen(sr);
	if (sr[0] == '"')
	{
		memcpy(temp, sr + 1, n - 2);
		temp[n - 2] = 0;
	}
	else if (sr[0] <= '9' && sr[0] >= '0')
	{
		return sr;
	}
	else
	{
		var v = vars[gvi(sr)];
		if (v.type == 0)
		{
			itoa(v.val, temp, 10);
		}
		else if (v.type == 1)
		{
			strcpy(temp, (char *)v.val);
		}
	}
	return temp;
}
int startParsing(char *code)
{
	var_count = 0;
	sp = 0;
	stack = (uint32_t *)malloc(1024 * 4 * 4);
	char ins = 0;
	for (uint32_t i = 0; i < fsize; i++)
	{
		if (code[i] == '"')
			ins = !ins;
		if (code[i] == ' ' && ins)
			code[i] = 2;
		else if (code[i] == '\t')
			code[i] = ' ';
		else if (code[i] == '\r')
			code[i] = ' ';
	}

	code[fsize] = 0;
	code = strtok(code, "\n");

	int lines = 0;

	while (code != NULL)
	{
		((char **)tokens)[lines] = code;
		code = strtok(NULL, "\n");
		lines++;
	}

	for (int i = 0; i < lines; i++)
	{
		break_tokens(((char **)tokens)[i], i);
	}
	for (int i = 1; i < lines + 1; i++)
	{
		for (int j = 0; j < token_count[i]; j++)

		{

			if (!strcmp(tokens[i][j], "print"))
			{
				for (j++; j < token_count[i]; j++)
					printf(tsr(tokens[i][j], 0));
			}
			else if (!strcmp(tokens[i][j], "call"))
			{
				i = call(vars[gvi(tokens[i][j + 1])].val, i);
				if (i == -1)
					return -1;
				i--;
			}
			else if (!strcmp(tokens[i][j], "ret"))
			{
				i = ret();
				if (i == -1)
					return -1;
			}
			else if (!strcmp(tokens[i][j], "var"))
			{
				regiser_var(tokens[i][j + 1]);
			}
			else if (!strcmp(tokens[i][j], "write"))
			{
				if (tokens[i][j + 2][0] <= '9' && tokens[i][j + 2][0] >= '0')
					wv(tokens[i][j + 1], atoi(tokens[i][j + 2]));
				else
				{
					wv(tokens[i][j + 1], (uint32_t)tsr(tokens[i][j + 2], 1));
					//string
					vars[gvi(tokens[i][j + 1])].type = 1;
				}
			}
			else if (!strcmp(tokens[i][j], "func"))
			{
				regiser_var(tokens[i][j + 1]);
				wv(tokens[i][j + 1], i + 2);
			}
			else if (!strcmp(tokens[i][j], "sys"))
			{
			}
		}
		//printf("END");
	}
	free(stack);
	return 0;
}