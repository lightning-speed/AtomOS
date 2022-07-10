#include <String.h>
#include <Memory.h>

uint32_t String::length()
{
	return len;
}
void String::append(String s)
{
	if (capacity < len + s.len)
	{
		char *buff2 = malloc(s.len + len + 50);
		capacity = s.len + len + 50;
		memcpy(buff2, buffer, len);
		memcpy(buff2 + len, s.buffer, s.len);
		len = s.len + len;
		free(s.buffer);
		buffer = buff2;
	}
	else
	{
		memcpy(buffer + len, s.buffer, s.len);
		len = s.len + len;
	}
}
void String::append(char c)
{
	if (capacity <= len + 1)
	{
		char *buff2 = malloc(len + 50);
		capacity = len + 50;
		buff2[len] = c;
		memcpy(buff2, buffer, len);
		len++;
		free(buffer);
		buffer = buff2;
	}
	else
	{
		buffer[len] = c;
		len++;
	}
}
uint32_t clen(char *c)
{
	uint32_t i = 0;
	while (c[i] != 0)
		i++;
	return i;
}
void String::append(char *c)
{
	if (capacity < len + clen(c))
	{
		char *buff2 = malloc(clen(c) + len + 50);
		capacity = clen(c) + len + 50;
		memcpy(buff2, buffer, len);
		memcpy(buff2 + len, c, clen(c));
		len = clen(c) + len;
		free(c);
		buffer = buff2;
	}
	else
	{
		memcpy(buffer + len, c, clen(c));
		len = clen(c) + len;
	}
}
void String::append(int n)
{
	char *cr = toCA(n);
	append(cr);
	free(cr);
}
int abs(int a)
{
	if (a < 0)
		return -a;
	return a;
}
String::String(char *s)
{
	buffer = s;
	len = clen(s);
	capacity = len;
}
String::String(int n)
{
	len = 0;
	capacity = 0;
	append(n);
}
String::String(char c)
{
	len = 0;
	capacity = 0;
	append(c);
}
uint32_t strlen(const char *s)
{
	for (uint32_t i = 0;; i++)
		if (s[i] == 0)
			return i;
}
char *toCA(int a)
{
	int im = a < 0;
	a = abs(a);
	char *out = (char *)(malloc(13) + 13);
	char *finalo = out;
	int i = 0;
	if (a == 0)
	{
		out[11] = '0';
		return out + 11;
	}
	for (i = 13; i >= 0; i--)
	{
		if (a > 0)
		{
			out[i] = '0' + (a % 10);
			a /= 10;
			out[i - 1] = 0;
		}
		else
			break;
	}
	if (im)
	{
		out[i] = '-';
		i--;
		out[i - 1] = 0;
	}

	out += i + 1;
	memcpy(finalo, out, 10);
	return finalo;
}
bool operator==(String s, String s2)
{
	if (s.len != s2.len)
	{
		return 0;
	}
	for (uint32_t i = 0; i < s.len; i++)
	{
		if (s2.buffer[i] != s.buffer[i])
			return 0;
	}
	return 1;
}
String::~String()
{
	free(buffer);
}
String operator+(String s, String s2)
{
	s.append(s2);
	return s;
}

bool String::strcmp(char *a, char *b)
{
	if (clen(a) != clen(b))
		return 0;
	for (int i = 0; a[i] != 0; i++)
	{
		if (a[i] != b[i])
			return 0;
	}
	return 1;
}

char *strchr(const char *s, int c)
{
	const char c_ = (char)c;
	do
	{
		if (*s == c_)
		{
			return (char *)s;
		}
	} while (*s++ != '\0');
	return nullptr;
}

int strcmp(const char *lhs, const char *rhs)
{
	for (; *lhs == *rhs; ++lhs, ++rhs)
	{
		if (*lhs == '\0')
		{
			return 0;
		}
	}
	const int res = *lhs - *rhs;
	return res;
}

uint32_t strcspn(const char *s1, const char *s2)
{
	uint32_t res = 0;
	while (*s1 != '\0')
	{
		if (strchr(s2, *s1) == nullptr)
		{
			++s1;
			++res;
		}
		else
		{
			return res;
		}
	}
	return res;
}

char *strtok(char *s, const char *delim)
{
	static char *last;

	if (s == nullptr)
	{
		s = last;
	}
	int ch;
	do
	{
		ch = *s++;
		if (ch == '\0')
		{
			return nullptr;
		}
	} while (strchr(delim, ch));
	--s;
	last = s + strcspn(s, delim);
	if (*last != '\0')
	{
		*last++ = '\0';
	}
	return s;
}

char *strcat(char *destination, const char *source)
{
	char *ptr = destination + strlen(destination);

	while (*source != '\0')
	{
		*ptr++ = *source++;
	}

	*ptr = '\0';

	return destination;
}