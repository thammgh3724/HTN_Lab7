#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#define INIT 0
#define STARTMODE 1
#define PICKMODE 2
#define NORMALMODE 3
#define TIMINGMODE 4
#define ADVANCEMODE 5
#define STOPMODE 6
#define OVERMODE 7
#define SETUPTIMEMODE 8
#define SETUPADVANCEMODE 9
#define EXITMODE 10

#define ARROWX1 180
#define ARROWX2 220
#define ARROWNORMALY1 210
#define ARROWNORMALY2 190
#define ARROWTIMINGY1 260
#define ARROWTIMINGY2 240
#define ARROWADVANCEY1 260
#define ARROWADVANCEY2 240

#define NEWGAME 0
#define EXIT 1
#define RESUME 2
#define YES 3
#define NO 4

#define NOTIMING 999

#include "main.h"
extern uint16_t ID;
extern uint16_t TIMING;
extern uint16_t SCORE;
extern uint16_t WALL;

extern int arrowX1;
extern int arrowY1;
extern int arrowX2;
extern int arrowY2;
extern int arrowMode;

extern int wallX[4];
extern int wallY[4];
extern int wallSize;

extern int overWallX[4];
extern int overWallY[4];
extern int arrowOverMode;

extern int arrowStopMode;
extern int arrowExitMode;

extern int flagUpDOwn;
extern int flagChange;

extern int idX;
extern int idY;
extern int timingX;
extern int timingY;
extern int scoreX;
extern int scoreY;
extern int logoBKX;
extern int logoBKY;
extern int logoBKHeight;
extern int logoBKWidth;


extern int upper[2];
extern int lower[2];

extern int statusGame;
extern int oldStatusGame;

extern int flagOver;
extern int flagStop;
extern int flagExit;

extern uint16_t buttonForSnake[6];
extern uint16_t gameover;

extern uint16_t oldTiming;
extern uint16_t OVERMESSAGE;
extern uint16_t TOTALTIME;

extern uint32_t TOTALSCORE;
extern uint32_t TOTALTIMEPLAY;
extern uint32_t TOTALPLAY;
#endif /* INC_GLOBAL_H_ */
