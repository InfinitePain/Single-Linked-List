/*
=============================================================================
 Name        : main.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Single-Linked-List
=============================================================================
*/

#include "utilities.h"
#include "time.h"
#include <stdlib.h>
#include "test.h"
#include "string.h"

List *list = NULL;
int highlight_index = -1;

void displayList(WINDOW *win, Element *head)
{
    wclear(win);

    int highlight_start_x = -1;
    int highlight_end_x = -1;
    int highlight_y = -1;
    int offset = 0;
    int max_y, max_x;
    getmaxyx(win, max_y, max_x);

    int buffer_size = max_y * max_x;
    char buffer[buffer_size];
    memset(buffer, 0, sizeof(buffer));

    if (head == NULL)
    {
        snprintf(buffer, buffer_size, "Empty list");
        int x = (max_x - strlen(buffer)) / 2;
        int y = (max_y - 1) / 2;
        mvwprintw(win, y, x, buffer);
        wrefresh(win);
        return;
    }

    Element *curr = head;
    int index = 0;
    do
    {
        char temp[16];
        snprintf(temp, sizeof(temp), "[ %d ]", curr->value);
        strncat(buffer, temp, buffer_size - strlen(buffer) - 1);

        char *last_newline = strrchr(buffer, '\n');
        char *line_start = last_newline ? last_newline + 1 : buffer;
        if (curr->pSuccessor != NULL && strlen(line_start) + 7 + snprintf(NULL, 0, "%d", curr->pSuccessor->value) > max_x)
        {
            strncat(buffer, "\n", buffer_size - strlen(buffer) - 1);
        }
        else if (curr->pSuccessor != NULL)
        {
            strncat(buffer, " r ", buffer_size - strlen(buffer) - 1);
            offset = 3;
        }
        else
        {
            offset = 0;
        }
        if (index == highlight_index)
        {
            highlight_start_x = strlen(line_start) - strlen(temp) - offset;
            highlight_end_x = strlen(line_start) - offset;
            highlight_y = 0;
            for (char *c = buffer; c <= last_newline + 1; c++)
                highlight_y += (*c == '\n');
            highlight_index = -1;
        }

        curr = curr->pSuccessor;
        index++;
    } while (curr != NULL && strlen(buffer) < buffer_size - 1);

    int width = strchr(buffer, '\n') == NULL ? strlen(buffer) : strchr(buffer, '\n') - buffer;
    int height = 0;
    for (char *c = buffer; *c; c++)
        height += (*c == '\n');

    int x = (height == 0) ? (max_x - width) / 2 : 0;
    int y = (max_y - height) / 2;
    highlight_start_x += x;
    highlight_end_x += x;
    highlight_y += y;

    for (char *c = buffer; *c; c++)
    {
        if (y >= max_y)
            break;

        switch (*c)
        {
        case 'r':
            wattron(win, COLOR_PAIR(2));
            mvwaddch(win, y, x++, ACS_RARROW);
            wattroff(win, COLOR_PAIR(2));
            break;
        case '\n':
            y++;
            x = 0;
            break;
        case '[':
        case ']':
            wattron(win, COLOR_PAIR(3));
            mvwaddch(win, y, x++, *c);
            wattroff(win, COLOR_PAIR(3));
            break;
        default:
            if (y == highlight_y && x > highlight_start_x && x < highlight_end_x)
            {
                wattron(win, COLOR_PAIR(4));
                mvwaddch(win, y, x++, *c);
                wattroff(win, COLOR_PAIR(4));
                break;
            }
            else
            {
                wattron(win, COLOR_PAIR(1));
                mvwaddch(win, y, x++, *c);
                wattroff(win, COLOR_PAIR(1));
                break;
            }
        }
    }

    wrefresh(win);
}

int test_all()
{
    printf("Testing the push and print functionality...\n");
    test_pushPrint();
    printf("Testing the find element functionality. . .\n");
    testListFindElement();
    printf("Testing the get index of element functionality. . .\n");
    testListGetIndexOfElement();
    printf("Testing the get element at index functionality. . .\n");
    testListGetElementAtIndex();
    printf("Press enter to exit. . .\n");
    getchar();
    return 0;
}

void func_push(int value)
{
    if (appState == APP_EXIT)
        return;
    listPush(list, value);
}

void func_remove(int value)
{
    if (appState == APP_EXIT)
        return;
    listDeleteElement(list, value);
}

bool func_find(int value)
{
    if (appState == APP_EXIT)
        return false;
    highlight_index = listGetIndexOfElement(list, value);
    return highlight_index != -1;
}

void func_reverse()
{
    if (appState == APP_EXIT)
        return;
    List* reversed = listCreate();
    if (reversed == NULL)
    {
        appState = APP_EXIT;
        return;
    }
    Element* curr = list->head;
    while (curr != NULL)
    {
        listPush(reversed, curr->value);
        curr = curr->pSuccessor;
    }
    listDelete(list);
    list = reversed;
}

void func_clear()
{
    if (appState == APP_EXIT)
        return;
    listDelete(list);
    list = listCreate();
    if (list == NULL)
    {
        appState = APP_EXIT;
        return;
    }
}

void func_quit()
{
    app_exit();
    should_loop = false;
}

int main(int argc, char *argv[])
{
    bool run_tests = false;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "test") == 0)
        {
            return test_all();
        }
    }

    bool should_quit = false;
    if (!app_init())
    {
        mvprintw(error_line, 0, "Error initializing the application. Press any key to exit.");
        getch();
        endwin();
        return 1;
    }
    if (COLS < 60 || LINES < 19)
    {
        func_quit();
        endwin();
        printf("Terminal size is too small. Please ensure that the terminal is at least 60x19.\n");
        printf("Press enter to exit.");
        getchar();
        return 1;
    }
    int key;
    list = listCreate();
    while (should_loop)
    {
        if (appState == APP_EXIT)
        {
            break;
        }
        displayList(Display_Win, list->head);
        print_to_pad("$ > ");
        scanString();
        parser();
    }
    if (appState == APP_EXIT)
    {
        func_quit();
        mvprintw(error_line, 0, "A critical error has occurred. Press any key to exit.");
        getch();
        endwin();
        return 1;
    }
    endwin();
    return 0;
}
