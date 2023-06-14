/*
=============================================================================
 Name        : test.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Single-Linked-List
=============================================================================
*/

#ifndef TEST_H_
#define TEST_H_

#include "list.h"
#include <stdlib.h>

void listFillRandom(List *list, int anzahlWerte, int min, int max)
{
	srand(time(NULL));
	if (list == NULL)
	{
		printf("There is no List to Fill!");
		return;
	}

	for (int i = 0; i < anzahlWerte; i++)
	{
		listPush(list, (rand() % (max + 1 - min)) + min);
	}
}

void test_pushPrint()
{
	List *pList = listCreate();
	List *pListReverse = listCreate();
	if (pList == NULL || pListReverse == NULL)
	{
		printf("Unable to create List!\n");
		return;
	}
	listFillRandom(pList, 10, 1, 100);

	Element *pCurrent = pList->head;

	while (pCurrent != NULL)
	{
		listPush(pListReverse, pCurrent->value);
		pCurrent = pCurrent->pSuccessor;
	}

	listPrint(pList);
	listPrint(pListReverse);
	listDelete(pList);
}

void testListFindElement()
{
	List *pList = listCreate();
	Element *pSeeker = listFindElement(pList, 0);
	if (pSeeker == NULL)
	{
		printf("Correct\n");
	}
	else
	{
		printf("Wrong\n");
	}
	int numberOfElements = 10;
	for (int i = 0; i < numberOfElements; i++)
	{
		listPush(pList, 9 - i);
	}
	pSeeker = listFindElement(pList, 5);
	if (pSeeker != NULL)
	{
		printf("Correct\n");
	}
	else
	{
		printf("Wrong\n");
	}
	pSeeker = listFindElement(pList, 20);
	if (pSeeker == NULL)
	{
		printf("Correct\n");
	}
	else
	{
		printf("Wrong\n");
	}
	listDelete(pList);
}

void testListGetIndexOfElement()
{
	List *pList = listCreate();
	int seeker = listGetIndexOfElement(pList, 0);
	if (seeker == -1)
	{
		printf("Correct\n");
	}
	else
	{
		printf("Wrong\n");
	}
	int numberOfElements = 10;
	for (int i = 0; i < numberOfElements; i++)
	{
		listPush(pList, 10 - i);
	}
	seeker = listGetIndexOfElement(pList, 1);
	if (seeker == 0)
	{
		printf("Correct\n");
	}
	else
	{
		printf("Wrong\n");
	}
	seeker = listGetIndexOfElement(pList, 4);
	if (seeker == 3)
	{
		printf("Correct\n");
	}
	else
	{
		printf("Wrong\n");
	}
	seeker = listGetIndexOfElement(pList, 25);
	if (seeker == -1)
	{
		printf("Correct\n");
	}
	else
	{
		printf("Wrong\n");
	}
	listDelete(pList);
}

void testListGetElementAtIndex()
{
	List *pList = listCreate();
	Element *pSeeker = listGetElementAtIndex(pList, 5);
	if (pSeeker == NULL)
	{
		printf("Correct\n");
	}
	else
	{
		printf("Wrong\n");
	}
	int numberOfElements = 10;
	for (int i = 0; i < numberOfElements; i++)
	{
		listPush(pList, 10 - i);
	}
	pSeeker = listGetElementAtIndex(pList, 5);
	if (pSeeker->value == 6)
	{
		printf("Correct\n");
	}
	else
	{
		printf("Wrong\n");
	}
	pSeeker = listGetElementAtIndex(pList, 15);
	if (pSeeker == NULL)
	{
		printf("Correct\n");
	}
	else
	{
		printf("Wrong\n");
	}
	listDelete(pList);
}

#endif /*TEST_H_*/
