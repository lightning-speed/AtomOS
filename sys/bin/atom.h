#pragma once
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#define ESC 27
#define DT_DIR 1
#define DT_REG 2

void set_screen(uint32_t type, uint32_t val);
extern uint32_t syscall(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx);

DIR *opendir(const char *path);
dirent *readdir(DIR *dir);
int close(DIR *dir);
int get_seconds();
int get_minutes();
int get_hours();
int get_days();
int get_months();
int get_years();
uint32_t fsize(FILE *f);
fnode *getChild(char *name, fnode *parent);
fnode *getChild(char *name, fnode *parent)
{
	for (uint32_t i = 0; i < parent->size; i++)
	{
		if (!strcmp(name, ((fnode *)parent->children[i])->name))
		{
			return (fnode *)(parent->children[i]);
		}
	}
	return (void *)0;
}

char *getenv(const char *of)
{
	if (!strcmp(of, "PWD"))
	{
		return ((char **)syscall(10, 0, 0, 0))[0];
	}
	if (!strcmp(of, "HOME"))
	{
		return ((char **)syscall(10, 0, 0, 0))[1];
	}
	if (!strcmp(of, "PATH"))
	{
		return ((char **)syscall(10, 0, 0, 0))[2];
	}
}
typedef struct
{
	uint16_t width;
	uint16_t height;
	uint16_t x;
	uint16_t y;
	char *name;
	uint32_t *buffer;
} window_t;

typedef window_t *Window;

typedef struct
{
	char *text;
	uint32_t color;
	uint32_t background;
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	char type;
} component_t;
typedef component_t *Component;

void swap_buffer(Window win);
void fillRect(Window w, int x, int y, int width, int height);
void setColor(uint32_t color);
void swap_buffer_pos(Window win, uint32_t x, uint32_t y, uint32_t w, uint32_t h);
void drawChar(Window w, char c, uint16_t x, uint16_t y);
void drawButton(Window w, char *name, int x, int y, int width, int height);
Component createButton(char *text, int x, int y);
void drawCompoenent(Window w, Component c);
Component createLabel(char *text, int x, int y, uint32_t color);
uint32_t toColor(uint8_t r, uint8_t g, uint8_t b);
int mouseX();
int mouseY();