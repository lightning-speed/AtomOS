#pragma once
#include <stdint.h>
char *toCA(int a);
class string
{
private:
	uint32_t len;
	uint32_t capacity;

public:
	char *buffer;
	string(char *);
	string(int);
	string(char);
	string(uint32_t);

	uint32_t length();
	void append(string s);
	void append(char c);
	void append(char *s);
	void append(int n);
	void append(uint32_t n);

	void incCap(uint32_t by);
	string toString(int a);
	friend string operator+(string, string);
	friend bool operator==(string s, string s2);

	static bool strcmp(char *a, char *b);
	~string();
};

char *toCA(int a);
char *toCA(uint32_t a);
int strcmp(const char *lhs, const char *rhs);
char *strtok(char *s, const char *delim);
uint32_t strlen(const char *s);
char *UItoCA(uint32_t a);
char *itoha(uint32_t it);
char *strcat(char *destination, const char *source);