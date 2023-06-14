/*
=============================================================================
 Name        : utilities.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Single-Linked-List
=============================================================================
*/

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <stdbool.h>
#include <ncurses.h>
#include <menu.h>

extern bool should_loop;
extern int error_line;
extern WINDOW *Display_Win;
#define BUFFER_SIZE 256
#define HISTORY_SIZE 5

typedef struct terminalInput
{
	char input[BUFFER_SIZE];
	int y[BUFFER_SIZE];
	int x[BUFFER_SIZE];
	int index_max;
} terminalInput;

extern terminalInput current_input;

typedef struct terminalHistory
{
	terminalInput history[HISTORY_SIZE];
	int index;
	int index_max;
} terminalHistory;

void error_message(const char *message);
void refresh_terminal();
void print_to_pad(const char *format, ...);
void scroll_up();
void scroll_down();
void func_push(int value);
void func_remove(int value);
bool func_find(int value);
void func_reverse();
void func_clear();
void func_quit();
void func_help();
void print_header();
bool app_init();
void app_exit();
void add_to_history();
void shift_elements(terminalInput *array, int start, int length, int direction);
void calculateCoordinates(int *x, int *y, int index, int maxX, int startX, int startY);
void initterminalInput(terminalInput *ti, int max_x, int start_x, int start_y);
void updateCoordinates(terminalInput *ti, int max_x, int start_x, int start_y);
void printterminalInput(terminalInput *ti, int max_x, int start_x, int start_y, int index);
void scanString();
bool str_to_int(char *str, int *value);
bool handle_push(char *buffer);
bool handle_remove(char *buffer);
bool handle_find(char *buffer);
void parser();

#endif /*UTILITIES_H_*/