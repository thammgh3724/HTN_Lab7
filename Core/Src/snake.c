#include "snake.h"
#include "lcd.h"
#include "button.h"
#include "limits.h"
#include "time.h"
#include "stdlib.h"
#include "global.h"

#define RIGHT 0
#define LEFT 1
#define DOWN 2
#define UP 3
#define rangeValidXLower 50
#define rangeValidXUpper 190
#define rangeValidYLower 50
#define rangeValidYUpper 270
#define snakeWidth 9
#define snakeStep 10
#define MAX_LENGTH 500
#define rangeXGenerate (rangeValidXUpper - rangeValidXLower) / snakeStep
#define rangeYGenerate (rangeValidYUpper - rangeValidYLower) / snakeStep

int rst = 0;

int xFruit = INT_MIN;
int yFruit = INT_MIN;
int flagEat = 1;
uint16_t buttonForSnake[6] = { 0 };
uint16_t gameover = 0;

typedef struct
{
	int x;
	int y;
} infoXYSnake;

typedef struct
{
	int x;
	int y;
	int vertical;
} infoXYWall;

typedef struct
{
	void (*GOLEFT)(void);
	void (*GORIGHT)(void);
	void (*GOUP)(void);
	void (*GODOWN)(void);
	void (*MOVE)(void);
	void (*CHECKHEAD)();
	int snakeDirectionHead;
	infoXYSnake infoTempSnakeTail;
	int snakeLength;
	int stopSnakeFlag;
	infoXYSnake infoSnake[MAX_LENGTH];
} SNAKE;

SNAKE snakeObject;

typedef struct
{
	infoXYWall infoWall[MAX_LENGTH];
	int amountWallElement;
	int vertical;
	int stopGenerateWallFlag;
} WALLs;

WALLs wallObject;

void drawHeadSnake()
{
	switch (snakeObject.snakeDirectionHead)
	{
	case UP:
		lcd_Fill(snakeObject.infoSnake[0].x, snakeObject.infoSnake[0].y,
				 snakeObject.infoSnake[0].x + snakeWidth, snakeObject.infoSnake[0].y + snakeWidth, RED);
		lcd_Fill(snakeObject.infoSnake[0].x, snakeObject.infoSnake[0].y,
				 snakeObject.infoSnake[0].x + 4, snakeObject.infoSnake[0].y + 4, BLACK);
		lcd_Fill(snakeObject.infoSnake[0].x + 5, snakeObject.infoSnake[0].y,
				 snakeObject.infoSnake[0].x + 9, snakeObject.infoSnake[0].y + 4, BLACK);
		break;
	case DOWN:
		lcd_Fill(snakeObject.infoSnake[0].x, snakeObject.infoSnake[0].y,
				 snakeObject.infoSnake[0].x + snakeWidth, snakeObject.infoSnake[0].y + snakeWidth, RED);
		lcd_Fill(snakeObject.infoSnake[0].x, snakeObject.infoSnake[0].y + 5,
				 snakeObject.infoSnake[0].x + 4, snakeObject.infoSnake[0].y + 9, BLACK);
		lcd_Fill(snakeObject.infoSnake[0].x + 5, snakeObject.infoSnake[0].y + 5,
				 snakeObject.infoSnake[0].x + 9, snakeObject.infoSnake[0].y + 9, BLACK);
		break;
	case LEFT:
		lcd_Fill(snakeObject.infoSnake[0].x, snakeObject.infoSnake[0].y,
				 snakeObject.infoSnake[0].x + snakeWidth, snakeObject.infoSnake[0].y + snakeWidth, RED);
		lcd_Fill(snakeObject.infoSnake[0].x, snakeObject.infoSnake[0].y,
				 snakeObject.infoSnake[0].x + 4, snakeObject.infoSnake[0].y + 4, BLACK);
		lcd_Fill(snakeObject.infoSnake[0].x, snakeObject.infoSnake[0].y + 5,
				 snakeObject.infoSnake[0].x + 4, snakeObject.infoSnake[0].y + 9, BLACK);
		break;
	case RIGHT:
		lcd_Fill(snakeObject.infoSnake[0].x, snakeObject.infoSnake[0].y,
				 snakeObject.infoSnake[0].x + snakeWidth, snakeObject.infoSnake[0].y + snakeWidth, RED);
		lcd_Fill(snakeObject.infoSnake[0].x + 5, snakeObject.infoSnake[0].y,
				 snakeObject.infoSnake[0].x + 9, snakeObject.infoSnake[0].y + 4, BLACK);
		lcd_Fill(snakeObject.infoSnake[0].x + 5, snakeObject.infoSnake[0].y + 5,
				 snakeObject.infoSnake[0].x + 9, snakeObject.infoSnake[0].y + 9, BLACK);
		break;
	default:
		break;
	}
}

void goLeft()
{
	if (snakeObject.stopSnakeFlag == 0)
	{
		switch (snakeObject.snakeDirectionHead)
		{
		case RIGHT:
			(*snakeObject.GORIGHT)();
			break;
		default:
			snakeObject.snakeDirectionHead = LEFT;
			infoXYSnake tempSnake1;
			infoXYSnake tempSnake2;
			for (int i = 0; i < snakeObject.snakeLength; i++)
			{
				if (i == 0)
				{
					tempSnake1 = snakeObject.infoSnake[i];
					snakeObject.infoSnake[i].x -= snakeStep;
					snakeObject.infoSnake[i].y = snakeObject.infoSnake[i].y;
					drawHeadSnake();
				}
				else
				{
					tempSnake2 = snakeObject.infoSnake[i];
					snakeObject.infoSnake[i] = tempSnake1;
					tempSnake1 = tempSnake2;
					if (i == 1)
						lcd_Fill(snakeObject.infoSnake[i].x, snakeObject.infoSnake[i].y,
								 snakeObject.infoSnake[i].x + snakeWidth, snakeObject.infoSnake[i].y + snakeWidth, BLACK);
					if (i == snakeObject.snakeLength - 1)
					{
						snakeObject.infoTempSnakeTail = tempSnake2;
						lcd_Fill(tempSnake2.x, tempSnake2.y, tempSnake2.x + snakeWidth, tempSnake2.y + snakeWidth, WHITE);
					}
				}
			}
			break;
		}
	}
}

void goRight()
{
	if (snakeObject.stopSnakeFlag == 0)
	{
		switch (snakeObject.snakeDirectionHead)
		{
		case LEFT:
			(*snakeObject.GOLEFT)();
			break;
		default:
			snakeObject.snakeDirectionHead = RIGHT;
			infoXYSnake tempSnake1;
			infoXYSnake tempSnake2;
			for (int i = 0; i < snakeObject.snakeLength; i++)
			{
				if (i == 0)
				{
					tempSnake1 = snakeObject.infoSnake[i];
					snakeObject.infoSnake[i].x += snakeStep;
					snakeObject.infoSnake[i].y = snakeObject.infoSnake[i].y;
					drawHeadSnake();
				}
				else
				{
					tempSnake2 = snakeObject.infoSnake[i];
					snakeObject.infoSnake[i] = tempSnake1;
					tempSnake1 = tempSnake2;
					if (i == 1)
						lcd_Fill(snakeObject.infoSnake[i].x, snakeObject.infoSnake[i].y,
								 snakeObject.infoSnake[i].x + snakeWidth, snakeObject.infoSnake[i].y + snakeWidth, BLACK);
					if (i == snakeObject.snakeLength - 1)
					{
						snakeObject.infoTempSnakeTail = tempSnake2;
						lcd_Fill(tempSnake2.x, tempSnake2.y, tempSnake2.x + snakeWidth, tempSnake2.y + snakeWidth, WHITE);
					}
				}
			}
			break;
		}
	}
}

void goUp()
{
	if (snakeObject.stopSnakeFlag == 0)
	{
		switch (snakeObject.snakeDirectionHead)
		{
		case DOWN:
			(*snakeObject.GODOWN)();
			break;
		default:
			snakeObject.snakeDirectionHead = UP;
			infoXYSnake tempSnake1;
			infoXYSnake tempSnake2;
			for (int i = 0; i < snakeObject.snakeLength; i++)
			{
				if (i == 0)
				{
					tempSnake1 = snakeObject.infoSnake[i];
					snakeObject.infoSnake[i].x = snakeObject.infoSnake[i].x;
					snakeObject.infoSnake[i].y -= snakeStep;
					drawHeadSnake();
				}
				else
				{
					tempSnake2 = snakeObject.infoSnake[i];
					snakeObject.infoSnake[i] = tempSnake1;
					tempSnake1 = tempSnake2;
					if (i == 1)
						lcd_Fill(snakeObject.infoSnake[i].x, snakeObject.infoSnake[i].y,
								 snakeObject.infoSnake[i].x + snakeWidth, snakeObject.infoSnake[i].y + snakeWidth, BLACK);
					if (i == snakeObject.snakeLength - 1)
					{
						snakeObject.infoTempSnakeTail = tempSnake2;
						lcd_Fill(tempSnake2.x, tempSnake2.y, tempSnake2.x + snakeWidth, tempSnake2.y + snakeWidth, WHITE);
					}
				}
			}
			break;
		}
	}
}

void goDown()
{
	if (snakeObject.stopSnakeFlag == 0)
	{
		switch (snakeObject.snakeDirectionHead)
		{
		case UP:
			(*snakeObject.GOUP)();
			break;
		default:
			snakeObject.snakeDirectionHead = DOWN;
			infoXYSnake tempSnake1;
			infoXYSnake tempSnake2;
			for (int i = 0; i < snakeObject.snakeLength; i++)
			{
				if (i == 0)
				{
					tempSnake1 = snakeObject.infoSnake[i];
					snakeObject.infoSnake[i].x = snakeObject.infoSnake[i].x;
					snakeObject.infoSnake[i].y += snakeStep;
					drawHeadSnake();
				}
				else
				{
					tempSnake2 = snakeObject.infoSnake[i];
					snakeObject.infoSnake[i] = tempSnake1;
					tempSnake1 = tempSnake2;
					if (i == 1)
						lcd_Fill(snakeObject.infoSnake[i].x, snakeObject.infoSnake[i].y,
								 snakeObject.infoSnake[i].x + snakeWidth, snakeObject.infoSnake[i].y + snakeWidth, BLACK);
					if (i == snakeObject.snakeLength - 1)
					{
						snakeObject.infoTempSnakeTail = tempSnake2;
						lcd_Fill(tempSnake2.x, tempSnake2.y, tempSnake2.x + snakeWidth, tempSnake2.y + snakeWidth, WHITE);
					}
				}
			}
			break;
		}
	}
}
int checkOutOfValidRange()
{
	if ((snakeObject.infoSnake[0].x < 45) || (snakeObject.infoSnake[0].x > 195) || (snakeObject.infoSnake[0].y < 45) || (snakeObject.infoSnake[0].y > 275))
		return 1;
	return 0;
}

int checkEatBody()
{
	for (int i = 1; i < snakeObject.snakeLength; i++)
		if (snakeObject.infoSnake[0].x == snakeObject.infoSnake[i].x && snakeObject.infoSnake[0].y == snakeObject.infoSnake[i].y)
			return 1;
	return 0;
}

int checkEatFruit()
{
	if (snakeObject.infoSnake[0].x == xFruit && snakeObject.infoSnake[0].y == yFruit)
		return 1;
	return 0;
}


void checkHead()
{
	if (checkOutOfValidRange())
	{
		gameover = 1;
		return;
	}
	if (checkEatBody())
	{
		gameover = 1;
		return;
	}
	if (checkEatFruit())
	{
		xFruit = INT_MIN;
		yFruit = INT_MIN;
		snakeObject.snakeLength += 1;
		snakeObject.infoSnake[snakeObject.snakeLength - 1].x = snakeObject.infoTempSnakeTail.x;
		snakeObject.infoSnake[snakeObject.snakeLength - 1].y = snakeObject.infoTempSnakeTail.y;
		int x = snakeObject.infoSnake[snakeObject.snakeLength - 1].x;
		int y = snakeObject.infoSnake[snakeObject.snakeLength - 1].y;
		lcd_Fill(x, y, x + snakeWidth, y + snakeWidth, BLACK);
		flagEat = 1;
		return;
	}
}

void move()
{
	switch (snakeObject.snakeDirectionHead)
	{
	case DOWN:
		(*snakeObject.GODOWN)();
		break;
	case UP:
		(*snakeObject.GOUP)();
		break;
	case RIGHT:
		(*snakeObject.GORIGHT)();
		break;
	case LEFT:
		(*snakeObject.GOLEFT)();
		break;
	default:
		break;
	}
}

void snakeRun()
{
	if (buttonForSnake[0])
	{
		buttonForSnake[0]= 0;
		(*snakeObject.GOUP)();
		(*snakeObject.CHECKHEAD)();
	}
	else if (buttonForSnake[1])
	{
		buttonForSnake[1]= 0;
		(*snakeObject.GOLEFT)();
		(*snakeObject.CHECKHEAD)();
	}
	else if (buttonForSnake[3])
	{
		buttonForSnake[3]= 0;
		(*snakeObject.GORIGHT)();
		(*snakeObject.CHECKHEAD)();
	}
	else if (buttonForSnake[5])
	{
		buttonForSnake[5]= 0;
		(*snakeObject.GODOWN)();
		(*snakeObject.CHECKHEAD)();
	}
	else
	{
		(*snakeObject.MOVE)();
		(*snakeObject.CHECKHEAD)();
	}
}


void snakeInit()
{
	srand(rst);
	rst++;
	flagEat = 1;
	snakeObject.stopSnakeFlag = 0;
	snakeObject.infoSnake[0].x = 70;
	snakeObject.infoSnake[0].y = rangeValidYLower;
	snakeObject.infoSnake[1].x = 60;
	snakeObject.infoSnake[1].y = rangeValidYLower;
	snakeObject.infoSnake[2].x = 50;
	snakeObject.infoSnake[2].y = rangeValidYLower;
	for (int i = 3; i < MAX_LENGTH; i++)
	{
		snakeObject.infoSnake[i].x = 0;
		snakeObject.infoSnake[i].y = 0;
	}
	snakeObject.GODOWN = goDown;
	snakeObject.GOLEFT = goLeft;
	snakeObject.GOUP = goUp;
	snakeObject.GORIGHT = goRight;
	snakeObject.MOVE = move;
	snakeObject.CHECKHEAD = checkHead;
	snakeObject.snakeLength = 3;
	snakeObject.snakeDirectionHead = RIGHT;
	for (int i = 0; i < 3; i++)
	{
		if (i == 0)
			drawHeadSnake();
		else
			lcd_Fill(snakeObject.infoSnake[i].x, snakeObject.infoSnake[i].y,
					 snakeObject.infoSnake[i].x + snakeWidth, snakeObject.infoSnake[i].y + snakeWidth, BLACK);
	}
}

void generateFruit()
{
	if (flagEat == 1)
	{
		while (1)
		{
			int remind = 0;
			xFruit = (rand() % (rangeXGenerate -9)) * snakeStep + 60;
//			yFruit = (rand() % (rangeYGenerate) + 7) * snakeStep + 60;
			yFruit = (rand() % (rangeYGenerate - 9)) * snakeStep + 60;
			for (int i = 0; i < snakeObject.snakeLength; i++)
			{
				if (xFruit == snakeObject.infoSnake[i].x && yFruit == snakeObject.infoSnake[i].y)
				{
					xFruit = INT_MIN;
					yFruit = INT_MIN;
					remind = 1;
					break;
				}
			}
			if (remind)
				continue;
			// for (int i = 0; i < wallObject.amountWallElement; i++)
			// {
			// 	if (wallObject.infoWall[i].vertical == 1)
			// 	{
			// 		if ((wallObject.infoWall[i].y == yFruit ||
			// 			 wallObject.infoWall[i].y + snakeStep == yFruit ||
			// 			 wallObject.infoWall[i].y - snakeStep == yFruit) &&
			// 			(wallObject.infoWall[i].x == xFruit))
			// 		{
			// 			remind = 1;
			// 			break;
			// 		}
			// 	}
			// 	else
			// 	{
			// 		if ((wallObject.infoWall[i].x == xFruit ||
			// 			wallObject.infoWall[i].x + snakeStep == xFruit ||
			// 			wallObject.infoWall[i].x - snakeStep == xFruit) &&
			// 			(wallObject.infoWall[i].y == yFruit))
			// 		{
			// 			remind = 1;
			// 			break;
			// 		}
			// 	}
			// }
			// if (remind)
			// 	continue;
			lcd_Fill(xFruit, yFruit, xFruit + snakeWidth, yFruit + snakeWidth, DARKBLUE);
			flagEat = 0;
			break;
		}
	}
}



void stopSnake()
{
	snakeObject.stopSnakeFlag = 1;
}



