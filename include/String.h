#pragma once
#include <stdint.h>
char *toCA(int a);
class String
{
private:
	uint32_t len;
	uint32_t capacity;

public:
	char *buffer;
	String(char *);
	String(int);
	String(char);
	uint32_t length();
	void append(String s);
	void append(char c);
	void append(char *s);
	void append(int n);
	String toString(int a);
	friend String operator+(String, String);
	friend bool operator==(String s, String s2);

	static bool strcmp(char *a, char *b);
	~String();
};

char *toCA(int a);
int strcmp(const char *lhs, const char *rhs);
char *strtok(char *s, const char *delim);
uint32_t strlen(const char *s);
char *UItoCA(uint32_t a);
char *itoha(uint32_t it);