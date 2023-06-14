/*
=============================================================================
 Name        : list.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Single-Linked-List
=============================================================================
*/

#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>
#include "element.h"

typedef struct List
{
	Element *head;
} List;

List *listCreate()
{
	if (appState == APP_EXIT)
	{
		return NULL;
	}
	List *pList = (List *)malloc(sizeof(List));
	if (pList == NULL)
	{
		appState = APP_EXIT;
		return NULL;
	}
	pList->head = NULL;
	return pList;
}

void listPush(List *list, int value)
{
	if (appState == APP_EXIT && list == NULL)
	{
		return;
	}
	Element *pNewElement = elementCreate(value);
	if (pNewElement == NULL)
	{
		appState = APP_EXIT;
		return;
	}
	if (list->head == NULL)
	{
		list->head = pNewElement;
		return;
	}
	pNewElement->pSuccessor = list->head;
	list->head = pNewElement;
}

Element *listPop(List *list)
{
	Element *pOldHead = list->head;
	list->head = list->head->pSuccessor;
	return pOldHead;
}

void listPrint(List *list)
{
	if (appState == APP_EXIT && list == NULL)
	{
		return;
	}
	int max_X = getmaxx(stdscr);
	int max_Y = getmaxy(stdscr);
	Element *pCurrent = list->head;
	if (pCurrent == NULL)
	{
		return;
	}
	while (pCurrent != NULL)
	{

		printf("%i", pCurrent->value);
		printf(" ");
		pCurrent = pCurrent->pSuccessor;
	}
	printf("\n");
}

Element *listFindElement(List *list, int value)
{
	if (appState == APP_EXIT && list == NULL && list->head == NULL)
	{
		return NULL;
	}
	Element *pCurrent = list->head;
	while (pCurrent != NULL)
	{
		if (pCurrent->value == value)
		{
			return pCurrent;
		}
		pCurrent = pCurrent->pSuccessor;
	}
	return NULL;
}

int listGetIndexOfElement(List *list, int value)
{
	if (appState == APP_EXIT && list == NULL && list->head == NULL)
	{
		return -1;
	}
	int counter = 0;
	Element *pCurrent = list->head;
	while (pCurrent != NULL)
	{
		if (pCurrent->value == value)
		{
			return counter;
		}
		pCurrent = pCurrent->pSuccessor;
		counter++;
	}
	return (-1);
}

Element *listGetElementAtIndex(List *list, int index)
{
	if (appState == APP_EXIT && list == NULL && list->head == NULL && index < 0)
	{
		return NULL;
	}
	int counter = 0;
	Element *pCurrent = list->head;
	while (pCurrent != NULL)
	{
		if (counter == index)
		{
			return pCurrent;
		}
		pCurrent = pCurrent->pSuccessor;
		counter++;
	}
	return NULL;
}

bool listSwapElements(List *list, int aIndex, int bIndex)
{
	if (appState == APP_EXIT && list == NULL && list->head == NULL && aIndex == bIndex && aIndex < 0 && bIndex < 0)
	{
		return FALSE;
	}

	Element *paIndex = listGetElementAtIndex(list, aIndex);
	Element *pbIndex = listGetElementAtIndex(list, bIndex);
	int temp = paIndex->value;
	paIndex->value = pbIndex->value;
	pbIndex->value = temp;
	return TRUE;
}

bool listDeleteElement(List *list, int value)
{
	if (appState == APP_EXIT && list == NULL && list->head == NULL)
	{
		return FALSE;
	}

	int IndexElement = listGetIndexOfElement(list, value);
	if (IndexElement == -1)
	{
		return FALSE;
	}

	Element *pPreviousElement = listGetElementAtIndex(list, IndexElement - 1);
	if (pPreviousElement == NULL)
	{
		Element *pDeletedElement = listPop(list);
		free(pDeletedElement);
		return TRUE;
	}

	Element *pDeletedElement = pPreviousElement->pSuccessor;
	pPreviousElement->pSuccessor = pDeletedElement->pSuccessor;
	free(pDeletedElement);
	return TRUE;
}

void listDelete(List *list)
{
	if (list == NULL && list->head == NULL)
	{
		return;
	}
	Element *pCurrent = list->head;
	while (pCurrent != NULL)
	{
		Element *pNext = pCurrent->pSuccessor;
		free(pCurrent);
		pCurrent = pNext;
	}
	free(list);
}

#endif /*LIST_H_*/
