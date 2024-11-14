#ifndef FSM_H
#define FSM_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define ROWS 20
#define COLUMNS 10
#define FIGURES 7
#define ROTATIONS 4
#define MAX_UNITS 4
#define COLOR_ORANGE 8
#define COLOR_PURPLE 9

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef struct {
  int **matrix;
  int row, col;
  int figure_index;
  int rotation_index;
} CurrentShape;

typedef struct {
  int x;
  int y;
} Coordinates_t;

typedef struct {
  Coordinates_t coords[ROTATIONS][MAX_UNITS];
} TetrisFigure_t;

typedef struct {
  TetrisFigure_t figures[FIGURES];
} TetrisShape_t;

extern struct timeval last_update;

void userInput(UserAction_t *action, bool *hold);
void processUserAction(UserAction_t action, GameInfo_t *game,
                       TetrisShape_t *shapes, CurrentShape *shape, bool hold,
                       int *next_figure, bool *GameOn);
void initColors();
GameInfo_t updateCurrentState();

#endif