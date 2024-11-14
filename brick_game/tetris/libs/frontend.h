#ifndef FRONTEND_H
#define FRONTEND_H

#include "backend.h"
#include "fsm.h"

typedef enum {
  TEMPTY,
  TCELLI,
  TCELLJ,
  TCELLL,
  TCELLO,
  TCELLS,
  TCELLT,
  TCELLZ
} Tcell;

#define ADDBLOCK(w, x)                          \
  waddch((w), ' ' | A_REVERSE | COLOR_PAIR(x)); \
  waddch((w), ' ' | A_REVERSE | COLOR_PAIR(x))
#define ADDEMPTY(w) \
  waddch((w), ' '); \
  waddch((w), ' ')

void PrintTable(GameInfo_t *game, CurrentShape *shape);
void printStat(GameInfo_t *game);
void PrintNextFigure(GameInfo_t *game);
void to_pause(GameInfo_t *game, TetrisShape_t *shapes, CurrentShape *shape,
              bool hold, int *next_figure, bool *GameOn);

#endif