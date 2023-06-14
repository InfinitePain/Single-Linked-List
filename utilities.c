/*
=============================================================================
 Name        : utilities.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Single-Linked-List
=============================================================================
*/

#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>

bool should_loop = true;

WINDOW *Display_Win = NULL;
WINDOW *Terminal_Win = NULL;

#define MAX_PAD_LINES 50
int TERMINAL_OFFSET_Y = 0;
int TERMINAL_OFFSET_X = 0;
int TERMINAL_HEIGHT = 0;
int TERMINAL_WIDTH = 0;
int PAD_MAX_Y = 0;
int PAD_CURR_Y = 0;
terminalHistory command_history;
terminalInput current_input;

int error_line = 0;

void error_message(const char *message)
{
	error_line == 0 ? clear() : 0;
	char buf[BUFSIZ];
	snprintf(buf, sizeof(buf), "%s: %s", message, strerror(errno));
	strerror(errno);
	mvprintw(error_line, 0, "%s", buf);
	refresh();
	error_line++;
	sleep(1);
}

void refresh_terminal()
{
	prefresh(Terminal_Win, PAD_CURR_Y, 0, TERMINAL_OFFSET_Y, TERMINAL_OFFSET_X, TERMINAL_OFFSET_Y + TERMINAL_HEIGHT - 1, TERMINAL_WIDTH);
}

void print_to_pad(const char *format, ...)
{
	int cols = getmaxx(Terminal_Win);
	int x = getcurx(Terminal_Win);
	va_list args;
	va_start(args, format);
	char buffer[cols * MAX_PAD_LINES];
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);

	int lines = 0;
	int curr_line_length = x;
	for (char *c = buffer; *c != '\0'; c++)
	{
		if (*c == '\n')
		{
			lines++;
			curr_line_length = 0;
		}
		else
		{
			curr_line_length++;
			if (curr_line_length >= cols && *(c + 1) != '\n')
			{
				lines++;
				curr_line_length = 0;
			}
		}
	}

	if (PAD_MAX_Y + lines > MAX_PAD_LINES - TERMINAL_HEIGHT)
	{
		int shift = PAD_MAX_Y + lines - (MAX_PAD_LINES - TERMINAL_HEIGHT);
		wscrl(Terminal_Win, shift);
		PAD_MAX_Y -= shift;
		PAD_CURR_Y -= shift;
	}

	wmove(Terminal_Win, PAD_MAX_Y, x);
	vw_printw(Terminal_Win, format, args);
	PAD_MAX_Y += lines;

	if (PAD_MAX_Y - PAD_CURR_Y > TERMINAL_HEIGHT - 1)
	{
		PAD_CURR_Y++;
	}
	refresh_terminal();
}

void scroll_up()
{
	if (PAD_CURR_Y > 0)
	{
		PAD_CURR_Y--;
	}
	refresh_terminal();
}

void scroll_down()
{
	if (PAD_CURR_Y < PAD_MAX_Y - 1)
	{
		PAD_CURR_Y++;
	}
	refresh_terminal();
}

void func_help()
{
	print_to_pad("Usage: command [arguments]\n\n");
	print_to_pad("Commands:\n");
	print_to_pad("  push VALUE    Push an element with VALUE to the list.\n");
	print_to_pad("  remove VALUE  Remove the first element with VALUE from the list.\n");
	print_to_pad("  find VALUE    Find the first element with VALUE in the list and print its index.\n");
	print_to_pad("  reverse       Reverse the order of the elements in the list.\n");
	print_to_pad("  clear         Clear all elements from the list.\n");
	print_to_pad("  quit          Quit the program.\n");
	print_to_pad("  help          Display this help message.\n\n");
	print_to_pad("Examples:\n");
	print_to_pad("  push 10\n");
	print_to_pad("  remove 10\n");
	print_to_pad("  find 10\n");
	print_to_pad("  reverse\n");
	print_to_pad("  clear\n");
	print_to_pad("  quit\n");
}

void print_header()
{
	int string_length = strlen("TI 1. Semester Prog 1 Uebungsblatt 1");
	int x_start = COLS / 2 - string_length / 2;
	mvprintw(1, x_start, "TI 1. Semester Prog 1 Uebungsblatt 1");
	mvwaddch(stdscr, 2, 0, ACS_LTEE);
	mvwhline(stdscr, 2, 1, ACS_HLINE, COLS - 2);
	mvwaddch(stdscr, 2, COLS - 1, ACS_RTEE);
	refresh();
}

bool app_init()
{
	initscr();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);	// Color pair for numbers
	init_pair(2, COLOR_GREEN, COLOR_BLACK); // Color pair for arrows
	init_pair(3, COLOR_BLUE, COLOR_BLACK);	// Color pair for brackets
	init_pair(4, COLOR_RED, COLOR_WHITE);	// Color pair for numbers
	

	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, FALSE);
	curs_set(0);
	srand(time(NULL));

	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);
	max_x -= 2;
	max_y -= 5;
	int display_height = max_y / 5 * 2;
	int terminal_height = max_y - display_height;
	int offset = 3;
	Display_Win = newwin(display_height, max_x, offset, 1);
	refresh();
	Terminal_Win = newpad(MAX_PAD_LINES, max_x);
	TERMINAL_OFFSET_Y = display_height + offset + 1;
	TERMINAL_OFFSET_X = 1;
	TERMINAL_HEIGHT = terminal_height;
	TERMINAL_WIDTH = max_x;
	if (Display_Win == NULL || Terminal_Win == NULL)
	{
		free(Display_Win);
		free(Terminal_Win);
		error_message("Error creating windows");
		return false;
	}
	box(stdscr, 0, 0);
	mvwaddch(stdscr, display_height + offset, 0, ACS_LTEE);
	mvwhline(stdscr, display_height + offset, 1, ACS_HLINE, COLS - 2);
	mvwaddch(stdscr, display_height + offset, COLS - 1, ACS_RTEE);
	print_header();
	scrollok(Terminal_Win, TRUE);
	command_history.index = -1;
	command_history.index_max = -1;

	print_to_pad("Welcome to the list program!\n");
	print_to_pad("Type 'help' to display a list of commands.\n\n");

	return true;
}

void app_exit()
{
	delwin(Display_Win);
	delwin(Terminal_Win);
	endwin();
}

void shift_terminalHistory()
{
	for (int i = 0; i < HISTORY_SIZE - 1; i++)
	{
		command_history.history[i] = command_history.history[i + 1];
	}
	command_history.index_max--;
}

void add_to_history()
{
	if (command_history.index_max >= HISTORY_SIZE - 1)
	{
		shift_terminalHistory();
	}
	command_history.history[++command_history.index_max] = current_input;
	command_history.index = command_history.index_max;
}

void shift_elements(terminalInput *array, int start, int length, int direction)
{
	if (direction > 0) // Shift to the right
	{
		for (int i = length - 1; i >= start; i--)
		{
			array->input[i + 1] = array->input[i];
		}
	}
	else if (direction < 0) // Shift to the left
	{
		for (int i = start; i < length; i++)
		{
			array->input[i] = array->input[i + 1];
		}
	}
}

void calculateCoordinates(int *x, int *y, int index, int maxX, int startX, int startY)
{
	int remainingIndices = index - (maxX - startX);
	if (index < maxX - startX)
	{
		*x = index + startX;
		*y = startY;
	}
	else
	{
		*x = (remainingIndices % (maxX));
		*y = startY + (remainingIndices / maxX) + 1;
	}
}

void initterminalInput(terminalInput *ti, int max_x, int start_x, int start_y)
{
	int temp_line = start_y;
	int temp_col = start_x;

	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		if ((i >= max_x - start_x && temp_line == start_y) || (i >= max_x - start_x && (i - (max_x - start_x)) % max_x == 0))
		{
			temp_line++;
			temp_col = 0;
		}
		ti->input[i] = ' ';
		ti->x[i] = temp_col++;
		ti->y[i] = temp_line;
	}
	ti->input[BUFFER_SIZE - 1] = '\0';
	ti->index_max = 0;
}

void updateCoordinates(terminalInput *ti, int max_x, int start_x, int start_y)
{
	int temp_line = start_y;
	int temp_col = start_x;

	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		if ((i >= max_x - start_x && temp_line == start_y) || (i >= max_x - start_x && (i - (max_x - start_x)) % max_x == 0))
		{
			temp_line++;
			temp_col = 0;
		}
		ti->x[i] = temp_col++;
		ti->y[i] = temp_line;
	}
}

void printterminalInput(terminalInput *ti, int max_x, int start_x, int start_y, int index)
{
	int x, y;
	char temp[BUFFER_SIZE];
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		if (i < ti->index_max)
		{
			temp[i] = ti->input[i];
		}
		else
		{
			temp[i] = '\0';
			break;
		}
	}
	wmove(Terminal_Win, start_y, start_x);
	print_to_pad("%s", temp);
	PAD_MAX_Y = start_y;
	refresh_terminal();
	calculateCoordinates(&x, &y, index, max_x, start_x, start_y);
	wmove(Terminal_Win, y, x);
	int temp_y = y;
	int temp_x = x;
	calculateCoordinates(&x, &y, BUFFER_SIZE - 1, max_x, start_x, start_y);
	if (temp_y < y)
	{
		if (temp_y == start_y)
		{
			wmove(Terminal_Win, temp_y, ti->x[ti->index_max]);
			wclrtoeol(Terminal_Win);
		}
		for (int i = temp_y + 1; i < y; i++)
		{
			wmove(Terminal_Win, i, 0);
			wclrtoeol(Terminal_Win);
		}
	}
	else
	{
		wmove(Terminal_Win, y, x);
		wclrtoeol(Terminal_Win);
	}
	wmove(Terminal_Win, temp_y, temp_x);
	refresh_terminal();
}

void scanString()
{
	curs_set(1);

	int max_y, max_x, start_y, start_x, x, y;
	getmaxyx(Terminal_Win, max_y, max_x);
	getyx(Terminal_Win, start_y, start_x);
	int index = 0;
	terminalInput temp_input;
	initterminalInput(&current_input, max_x, start_x, start_y);
	temp_input = current_input;

	while (1)
	{
		int ch = getch();

		if (ch == KEY_UP || ch == KEY_DOWN)
		{
			if (ch == KEY_UP)
			{
				if (command_history.index > 0)
				{
					temp_input = command_history.history[command_history.index--];
				}
				else if (command_history.index == 0)
				{
					temp_input = command_history.history[command_history.index];
				}
			}
			else if (ch == KEY_DOWN)
			{
				if (command_history.index < command_history.index_max)
				{
					temp_input = command_history.history[++command_history.index];
				}
				else if (command_history.index == command_history.index_max)
				{
					index = current_input.index_max;
					updateCoordinates(&current_input, max_x, start_x, start_y);
					printterminalInput(&current_input, max_x, start_x, start_y, current_input.index_max);
					continue;
				}
			}
			index = temp_input.index_max;
			updateCoordinates(&temp_input, max_x, start_x, start_y);
			printterminalInput(&temp_input, max_x, start_x, start_y, temp_input.index_max);
			continue;
		}
		current_input = temp_input;

		if (ch >= ' ' && ch <= '~' && index < BUFFER_SIZE - 2)
		{
			shift_elements(&current_input, index, BUFFER_SIZE - 2, 1);
			current_input.input[index++] = ch;
			current_input.index_max++;
			wmove(Terminal_Win, current_input.y[current_input.index_max], current_input.x[current_input.index_max]);
			printterminalInput(&current_input, max_x, start_x, start_y, index);
		}
		else if (ch == KEY_BACKSPACE || ch == '\b' || ch == 127)
		{
			if (index > 0)
			{
				shift_elements(&current_input, --index, current_input.index_max, -1);
				current_input.input[current_input.index_max--] = ' ';
				printterminalInput(&current_input, max_x, start_x, start_y, index);
			}
			else
				beep();
		}
		else if (ch == KEY_DC)
		{
			if (index < current_input.index_max)
			{
				shift_elements(&current_input, index, BUFFER_SIZE - 2, -1);
				current_input.input[current_input.index_max--] = ' ';
				printterminalInput(&current_input, max_x, start_x, start_y, index);
			}
			else
				beep();
		}
		else if (ch == KEY_LEFT && index > 0)
		{
			index--;
			calculateCoordinates(&x, &y, index, max_x, start_x, start_y);
			wmove(Terminal_Win, y, x);
		}
		else if (ch == KEY_RIGHT && index < current_input.index_max)
		{
			index++;
			calculateCoordinates(&x, &y, index, max_x, start_x, start_y);
			wmove(Terminal_Win, y, x);
		}
		else if (ch == '\n')
		{
			calculateCoordinates(&x, &y, index, max_x, start_x, start_y);
			wmove(Terminal_Win, current_input.y[0], current_input.x[0]);
			char buffer[current_input.index_max + 2];
			for (int i = 0; i < current_input.index_max; i++)
			{
				buffer[i] = current_input.input[i];
			}
			buffer[current_input.index_max] = '\n';
			buffer[current_input.index_max + 1] = '\0';
			print_to_pad("%s", buffer);
			add_to_history();
			break;
		}
		else if (ch == KEY_PPAGE)
		{
			scroll_up();
			continue;
		}
		else if (ch == KEY_NPAGE)
		{
			scroll_down();
			continue;
		}
		else
		{
			beep();
		}
		temp_input = current_input;
		refresh_terminal(0);
	}

	curs_set(0);
}

bool str_to_int(char *str, int *value)
{
	char *endptr;
	long val = strtol(str, &endptr, 10);
	if (endptr == str || *endptr != '\0' || val > INT_MAX || val < INT_MIN)
	{
		return false;
	}
	*value = (int)val;
	return true;
}

bool handle_push(char *token)
{
	if (token == NULL)
	{
		print_to_pad("$ > Error: 'push' command requires a value. Example: 'push 10'.\n");
		return false;
	}
	int value;
	if (!str_to_int(token, &value))
	{
		print_to_pad("$ > Error: '%s' is not a valid integer.\n", token);
		return false;
	}
	func_push(value);
	return true;
}

bool handle_remove(char *token)
{
	if (token == NULL)
	{
		print_to_pad("$ > Error: 'remove' command requires a value. Example: 'remove 10'.\n");
		return false;
	}
	int value;
	if (!str_to_int(token, &value))
	{
		print_to_pad("$ > Error: '%s' is not a valid integer.\n", token);
		return false;
	}
	func_remove(value);
	return true;
}

bool handle_find(char *token)
{
	if (token == NULL)
	{
		print_to_pad("$ > Error: 'find' command requires a value. Example: 'find 10'.\n");
		return false;
	}
	int value;
	if (!str_to_int(token, &value))
	{
		print_to_pad("$ > Error: '%s' is not a valid integer.\n", token);
		return false;
	}
	if(!func_find(value)) 
	{
		print_to_pad("$ > Error: '%d' is not in the list.\n", value);
		return false;
	}
	return true;
}

void parser()
{
	char buffer[BUFFER_SIZE];
	for (int i = 0; i < current_input.index_max; i++)
	{
		buffer[i] = current_input.input[i];
	}
	buffer[current_input.index_max] = '\0';
	char *token = strtok(buffer, " ");
	while (token != NULL)
	{
		if (strcmp(token, "push") == 0)
		{
			token = strtok(NULL, " ");
			if (!handle_push(token))
			{
				return;
			}
		}
		else if (strcmp(token, "remove") == 0)
		{
			token = strtok(NULL, " ");
			if (!handle_remove(token))
			{
				return;
			}
		}
		else if (strcmp(token, "find") == 0)
		{
			token = strtok(NULL, " ");
			handle_find(token);
		}
		else if (strcmp(token, "reverse") == 0)
		{
			func_reverse();
		}
		else if (strcmp(token, "clear") == 0)
		{
			func_clear();
		}
		else if (strcmp(token, "quit") == 0)
		{
			func_quit();
			return;
		}
		else if (strcmp(token, "help") == 0)
		{
			func_help();
		}
		else
		{
			print_to_pad("$ > Invalid command: '%s'. Type 'help' to see the list of available commands.\n", token);
			return;
		}
		token = strtok(NULL, " ");
	}
}
