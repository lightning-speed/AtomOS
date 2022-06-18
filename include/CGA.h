#include <String.h>
namespace CGA
{
	extern char screenColor;
	extern int sy;
	extern int sx;
	void printChar(char c, int x, int y);
	void print(String s);
	void print(String s, char color);
	void clearScreen();
	void printChar(char c);
	void setCursorPosition(int xe, int ye);
	void backSpace();
};
