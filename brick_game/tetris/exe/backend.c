#include "../libs/backend.h"

struct timeval last_update;

void initShape(CurrentShape *shape) {
  shape->matrix = (int **)calloc(MAX_UNITS, sizeof(int *));
  for (int i = 0; i < MAX_UNITS; i++) {
    shape->matrix[i] = (int *)calloc(MAX_UNITS, sizeof(int));
  }
}

void startGame(GameInfo_t *game, TetrisShape_t *shapes, CurrentShape *shape,
               int *next_figure, bool *GameOn) {
  srand(time(NULL));
  game->score = 0;
  game->high_score = 0;
  game->level = 1;
  game->speed = 700000;
  game->pause = 0;
  initField(game);
  initFigures(shapes);
  initShape(shape);
  *next_figure = rand() % FIGURES;
  SetNewRandomShape(shapes, shape, game, next_figure, GameOn);
}

CurrentShape getShape(CurrentShape shape, Coordinates_t coords[MAX_UNITS]) {
  CurrentShape new_shape = shape;
  if (shape.matrix != NULL) {
    for (int i = 0; i < MAX_UNITS; i++) {
      free(shape.matrix[i]);
    }
    free(shape.matrix);
    shape.matrix = NULL;
  }
  initShape(&new_shape);
  for (int i = 0; i < MAX_UNITS; i++) {
    int x = coords[i].x;
    int y = coords[i].y;
    new_shape.matrix[x][y] = 1;
  }
  return new_shape;
}

void CopyShapeToNext(GameInfo_t *game, TetrisShape_t *shapes, int next_figure) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      game->next[i][j] = 0;
    }
  }
  int min_x = 4, min_y = 4;
  for (int k = 0; k < MAX_UNITS; k++) {
    int x = shapes->figures[next_figure].coords[0][k].x;
    int y = shapes->figures[next_figure].coords[0][k].y;
    if (x < min_x) min_x = x;
    if (y < min_y) min_y = y;
  }
  for (int k = 0; k < MAX_UNITS; k++) {
    int x = shapes->figures[next_figure].coords[0][k].x - min_x;
    int y = shapes->figures[next_figure].coords[0][k].y - min_y;
    game->next[x][y] = next_figure + 1;
  }
}

void SetNewRandomShape(TetrisShape_t *shapes, CurrentShape *shape,
                       GameInfo_t *game, int *next_figure, bool *GameOn) {
  *shape = getShape(*shape, shapes->figures[*next_figure].coords[0]);
  shape->col = COLUMNS / 2 - 1;
  shape->row = 0;
  shape->figure_index = *next_figure;
  shape->rotation_index = 0;
  if (!isPositionValid(game->field, shape, shape->row, shape->col)) {
    *GameOn = FALSE;
    endGame(game, shapes, shape, next_figure, GameOn);
  }
  *next_figure = rand() % FIGURES;
  CopyShapeToNext(game, shapes, *next_figure);
}

void initField(GameInfo_t *game) {
  game->field = (int **)calloc(ROWS, sizeof(int *));
  for (int i = 0; i < ROWS; i++) {
    game->field[i] = (int *)calloc(COLUMNS, sizeof(int));
  }
  game->next = (int **)calloc(4, sizeof(int *));
  for (int i = 0; i < 4; i++) {
    game->next[i] = (int *)calloc(4, sizeof(int));
  }
}

void initFigures(TetrisShape_t *shapes) {
  TetrisFigure_t I = {{{{1, 0}, {1, 1}, {1, 2}, {1, 3}},
                       {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
                       {{2, 0}, {2, 1}, {2, 2}, {2, 3}},
                       {{0, 1}, {1, 1}, {2, 1}, {3, 1}}}};
  shapes->figures[0] = I;

  TetrisFigure_t J = {{{{0, 2}, {1, 0}, {1, 1}, {1, 2}},
                       {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
                       {{1, 0}, {1, 1}, {1, 2}, {2, 0}},
                       {{0, 0}, {0, 1}, {1, 1}, {2, 1}}}};
  shapes->figures[1] = J;

  TetrisFigure_t L = {{{{0, 0}, {1, 0}, {1, 1}, {1, 2}},
                       {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
                       {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
                       {{0, 1}, {1, 1}, {2, 0}, {2, 1}}}};
  shapes->figures[2] = L;

  TetrisFigure_t O = {{{{0, 1}, {0, 2}, {1, 1}, {1, 2}},
                       {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
                       {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
                       {{0, 1}, {0, 2}, {1, 1}, {1, 2}}}};
  shapes->figures[3] = O;

  TetrisFigure_t S = {{{{0, 1}, {0, 2}, {1, 0}, {1, 1}},
                       {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
                       {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
                       {{0, 0}, {1, 0}, {1, 1}, {2, 1}}}};
  shapes->figures[4] = S;

  TetrisFigure_t T = {{{{0, 1}, {1, 0}, {1, 1}, {1, 2}},
                       {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
                       {{1, 0}, {1, 1}, {1, 2}, {2, 1}},
                       {{0, 1}, {1, 0}, {1, 1}, {2, 1}}}};
  shapes->figures[5] = T;

  TetrisFigure_t Z = {{{{0, 0}, {0, 1}, {1, 1}, {1, 2}},
                       {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
                       {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
                       {{0, 1}, {1, 0}, {1, 1}, {2, 0}}}};
  shapes->figures[6] = Z;
}

bool isPositionValid(int **field, CurrentShape *shape, int new_row,
                     int new_col) {
  bool valid_position = TRUE;
  for (int i = 0; i < MAX_UNITS; i++) {
    for (int j = 0; j < MAX_UNITS; j++) {
      if (shape->matrix[i][j] == 1) {
        int x = new_row + i;
        int y = new_col + j;
        if (x < 0 || x >= ROWS || y < 0 || y >= COLUMNS || field[x][y] != 0) {
          valid_position = FALSE;
        }
      }
    }
  }
  return valid_position;
}

void mergeShapeToField(GameInfo_t *game, CurrentShape *shape) {
  for (int i = 0; i < MAX_UNITS; i++) {
    for (int j = 0; j < MAX_UNITS; j++) {
      if (shape->matrix[i][j] == 1) {
        int x = shape->row + i;
        int y = shape->col + j;
        if (x >= 0 && x < ROWS && y >= 0 && y < COLUMNS) {
          game->field[x][y] = shape->figure_index + 1;
        }
      }
    }
  }
}

int clearFullLines(GameInfo_t *game) {
  int line_counter = 0;
  for (int i = 0; i < ROWS; i++) {
    bool fullLine = true;
    for (int j = 0; j < COLUMNS; j++) {
      if (game->field[i][j] == 0) {
        fullLine = false;
        break;
      }
    }
    if (fullLine) {
      for (int k = i; k > 0; k--) {
        for (int j = 0; j < COLUMNS; j++) {
          game->field[k][j] = game->field[k - 1][j];
        }
      }
      for (int j = 0; j < COLUMNS; j++) {
        game->field[0][j] = 0;
      }
      line_counter += 1;
    }
  }
  return line_counter;
}

void saveScore(GameInfo_t *game) {
  if (game->score > game->high_score) {
    FILE *f = fopen("high_score.txt", "w");
    if (f) {
      fprintf(f, "%d", game->score);
      fclose(f);
    }
  }
}

void setHighScore(GameInfo_t *game) {
  FILE *f = fopen("high_score.txt", "r");
  if (f) {
    fscanf(f, "%d", &game->high_score);
    fclose(f);
  }
}

void scoring(GameInfo_t *game, int points) {
  if (points == 1) {
    game->score += 100;
  } else if (points == 2) {
    game->score += 300;
  } else if (points == 3) {
    game->score += 700;
  } else if (points == 4) {
    game->score += 1500;
  }
  levels(game);
}

void figRotation(GameInfo_t *game, TetrisShape_t *shapes, CurrentShape *shape) {
  int next_rotation = (shape->rotation_index + 1) % ROTATIONS;
  CurrentShape rotated_shape = getShape(
      *shape, shapes->figures[shape->figure_index].coords[next_rotation]);
  int new_row = shape->row;
  int new_col = shape->col;
  if (!isPositionValid(game->field, &rotated_shape, new_row, new_col)) {
    while (new_col < 0) new_col++;
    while (new_col + MAX_UNITS > COLUMNS) new_col--;
    while (new_row < 0) new_row++;
    while (new_row + MAX_UNITS > ROWS) new_row--;
  }
  if (isPositionValid(game->field, &rotated_shape, new_row, new_col)) {
    *shape = rotated_shape;
    shape->rotation_index = next_rotation;
    shape->row = new_row;
    shape->col = new_col;
  }
}

void mergeOldInitNew(GameInfo_t *game, TetrisShape_t *shapes,
                     CurrentShape *shape, int *next_figure, bool *GameOn) {
  mergeShapeToField(game, shape);
  for (int i = 0; i < MAX_UNITS; i++) {
    free(shape->matrix[i]);
  }
  free(shape->matrix);
  shape->matrix = NULL;
  int points = clearFullLines(game);
  scoring(game, points);
  SetNewRandomShape(shapes, shape, game, next_figure, GameOn);
}

void levels(GameInfo_t *game) {
  const int lvlTable[10] = {0,    600,  1200, 1800, 2400,
                            3000, 3600, 4200, 4800, 5400};
  while (game->level < 10 && game->score >= lvlTable[game->level]) {
    game->level++;
    game->speed -= 60000;
  }
}

bool hasToUpdate(GameInfo_t *game) {
  struct timeval now;
  gettimeofday(&now, NULL);
  int elapsed = (now.tv_sec - last_update.tv_sec) * 1000000 +
                (now.tv_usec - last_update.tv_usec);
  return elapsed >= game->speed;
}

void freeMemory(GameInfo_t *game, CurrentShape *shape) {
  for (int i = 0; i < ROWS; i++) {
    free(game->field[i]);
  }
  free(game->field);
  for (int i = 0; i < MAX_UNITS; i++) {
    free(game->next[i]);
    free(shape->matrix[i]);
  }
  free(game->next);
  free(shape->matrix);
}

void exitGame(GameInfo_t *game, TetrisShape_t *shapes, CurrentShape *shape,
              int *next_figure, bool *GameOn) {
  clear();
  int start_row = 7;
  int start_col = 20;
  mvprintw(start_row, start_col,
           "The game is over, you have %d points. Want to start over?[y/n]",
           game->score);
  refresh();
  char action;
  while (true) {
    action = getch();
    if (action == 'y' || action == 'Y') {
      *GameOn = TRUE;
      for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
          game->field[i][j] = 0;
          clear();
        }
      }
      startGame(game, shapes, shape, next_figure, GameOn);
      break;
    } else if (action == 'n' || action == 'N') {
      clear();
      UserAction_t action = Terminate;
      processUserAction(action, game, shapes, shape, false, next_figure,
                        GameOn);
      break;
    }
  }
}