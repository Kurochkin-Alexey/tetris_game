#ifndef BACKEND_H
#define BACKEND_H

#include "fsm.h"

void startGame(GameInfo_t *game, TetrisShape_t *shapes, CurrentShape *shape,
               int *next_figure, bool *GameOn);
void initShape(CurrentShape *shape);
CurrentShape getShape(CurrentShape shape, Coordinates_t coords[MAX_UNITS]);
void CopyShapeToNext(GameInfo_t *game, TetrisShape_t *shapes, int next_figure);
void SetNewRandomShape(TetrisShape_t *shapes, CurrentShape *shape,
                       GameInfo_t *game, int *next_figure, bool *GameOn);
void initField(GameInfo_t *game);
void initFigures(TetrisShape_t *shapes);
bool isPositionValid(int **field, CurrentShape *shape, int new_row,
                     int new_col);
void endGame(GameInfo_t *game, TetrisShape_t *shapes, CurrentShape *shape,
             int *next_figure, bool *GameOn);
void mergeShapeToField(GameInfo_t *game, CurrentShape *shape);
int clearFullLines(GameInfo_t *game);
void saveScore(GameInfo_t *game);
void setHighScore(GameInfo_t *game);
void scoring(GameInfo_t *game, int points);
void levels(GameInfo_t *game);
void figRotation(GameInfo_t *game, TetrisShape_t *shapes, CurrentShape *shape);
void mergeOldInitNew(GameInfo_t *game, TetrisShape_t *shapes,
                     CurrentShape *shape, int *next_figure, bool *GameOn);
bool hasToUpdate(GameInfo_t *game);
void freeMemory(GameInfo_t *game, CurrentShape *shape);
void exitGame(GameInfo_t *game, TetrisShape_t *shapes, CurrentShape *shape,
              int *next_figure, bool *GameOn);

#endif