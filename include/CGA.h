#include <String.h>
namespace CGA
{
	extern char screenColor;
	extern int sy;
	extern int sx;
	void printChar(char c, int x, int y);
	void print(string s);
	void print(string s, char color);
	void clearScreen();
	void printChar(char c);
	void setCursorPosition(int xe, int ye);
	void backSpace();
	void init();
};
