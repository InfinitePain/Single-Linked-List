/*
=============================================================================
 Name        : element.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Single-Linked-List
=============================================================================
*/

#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <stdlib.h>

typedef enum _appState
{
	APP_RUNNING,
	APP_EXIT
} _appState;

_appState appState = APP_RUNNING;

typedef struct Element
{
	int value;
	struct Element* pSuccessor;
} Element;

Element *elementCreate(int value)
{
	if (appState == APP_EXIT)
	{
		return NULL;
	}
	Element *pElement = (Element *)malloc(sizeof(Element));
	if (pElement == NULL)
	{
		return NULL;
	}
	pElement->value = value;
	pElement->pSuccessor = NULL;
	return pElement;
}

#endif /*ELEMENT_H_*/
	