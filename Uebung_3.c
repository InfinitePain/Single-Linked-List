/*
 ============================================================================
 Name        : Übung3.c
 Author      : InfinitePain
 Version     : 0.2
 Copyright   : Hochschule Augsburg
 Description : Technische Informatik 1. Semester Programmieren 1 Übung 3
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myboolean.h"
#include "element.h"
#include "list.h"
#include "test.h"
int integerext(char* Number)
{
	//extracts the integer from a given char as a positive number
	char ch;
	int i, digit1, number1 = 0, Fractionfinder, Fractionfinder1 = 0;
	//to detect the number
	for (i = 0; i < strlen(Number); i++)
	{
		ch = Number[i];
		if (ch >= '0' && ch <= '9') //to confirm it's a digit 
		{
			digit1 = ch - '0';
			number1 = number1 * 10 + digit1;
		}
		else if (ch == 46 || ch == 44) //if reached to "." or "," end it
		{
			i = strlen(Number);
		}
	}
	return number1;
}
int fractionext(char* Number)
{
	//extracts the fraction from a given char as a positive number
	//only for two digit fractions
	char ch;
	int i, digit2, number2 = 0, Fractionfinder, Fractionfinder1 = 0;
	//to detect the number
	for (i = 0; i < strlen(Number); i++)
	{
		ch = Number[i];
		// to detect the fraction part
		if (ch != 46 && ch != 44)
		{
			if (ch >= '0' && ch <= '9') //to confirm it's a digit 
			{
				Fractionfinder = ch - '0';
				Fractionfinder1 = Fractionfinder1 * 10 + Fractionfinder1;
			}
		}
		else
		{
			for (int egal = i; egal < strlen(Number); egal++)
			{
				ch = Number[egal];
				if (ch >= '0' && ch <= '9') //to confirm it's a digit ,
				{
					digit2 = ch - '0';
					number2 = number2 * 10 + digit2;
				}
			}
			int ZeroDetector = i + 2;
			ch = Number[ZeroDetector];
			if (ch >= '0' && ch <= '9')
			{
				break;
			}
			else number2 = number2 * 10;
		}
	}
	return number2;
}
void FunctionCall(int TaskNumber, int StartText);
void Logic();
int RepeatQuestion();
void TaskEnde();
void Task1();

int main(void)
{
	printf("Technische Informatik 1. Semester \x9A \bbung 3\n");
	printf("\n");
	int repeat = 1;
	do
	{
		Logic();
		repeat = 2;
	} while (repeat == 1);
	printf("Das Program wird ausgeschaltet.\n");
	return EXIT_SUCCESS;
}

void Logic()
{
	int StartText = 0, TaskNumber = 1, RepeatAnswer;
	printf("\n");
	if (TaskNumber > 0 && TaskNumber < 6)
	{
		do
		{
			FunctionCall(TaskNumber, StartText);
			TaskEnde();
			RepeatAnswer = RepeatQuestion();
			StartText = RepeatAnswer;
		} while (RepeatAnswer == 1);
	}
	else
	{
		printf("Ung\x81 \bltig Zahl!!!\n");
	}
}
void FunctionCall(int TaskNumber, int StartText)
{
	switch (TaskNumber)
	{
	case 1:
		if (StartText != 1)
		{
			printf("============================================================================\n");
			printf("\n");
			printf("Verkettete Liste\n");
			printf("\n");
			//dieser Teil gibt es nicht in der Übungsblatt
			printf("Testing the Function for Element Finder\n");
			testListFindElement();
			printf("\n");

			printf("Testing the Function Index Finder \n");
			testListGetIndexOfElement();
			printf("\n");

			printf("Testing the Function for Element Finder from a given Index\n");
			testListGetElementAtIndex();
			printf("\n");

			printf("Testing the Push and Add Function\n");
			test_pushPrint();
			printf("\n");
		}
		Task1();
		break;
	}
}
int RepeatQuestion()
{
	int repeat;
	do
	{
		printf("M\x94 \bchten Sie noch mal probieren? (1 f\x81 \br Ja; 2 f\x81 \br Nein)\n");
		scanf("%i", &repeat);
	} while (repeat > 0 && repeat > 2);
	return repeat;
}
void TaskEnde()
{
	printf("\n");
	printf("============================================================================\n");
	printf("\n");
	printf("\n");
}

void Task1()
{

}