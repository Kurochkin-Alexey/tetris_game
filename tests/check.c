#include <check.h>

#include "../brick_game/tetris/libs/backend.h"

void initGame(GameInfo_t *game) {
  game->score = 0;
  game->high_score = 0;
  game->level = 0;
  game->speed = 1000000;
  initField(game);
  game->next = malloc(MAX_UNITS * sizeof(int *));
  for (int i = 0; i < MAX_UNITS; i++) {
    game->next[i] = malloc(MAX_UNITS * sizeof(int));
    for (int j = 0; j < MAX_UNITS; j++) {
      game->next[i][j] = 0;
    }
  }
}

START_TEST(test_initShape) {
  CurrentShape shape;
  initShape(&shape);
  for (int i = 0; i < MAX_UNITS; i++) {
    for (int j = 0; j < MAX_UNITS; j++) {
      ck_assert_int_eq(shape.matrix[i][j], 0);
    }
  }
  for (int i = 0; i < MAX_UNITS; i++) {
    free(shape.matrix[i]);
  }
  free(shape.matrix);
}
END_TEST

START_TEST(test_startGame) {
  GameInfo_t game;
  TetrisShape_t shapes;
  CurrentShape shape;
  int next_figure;
  bool GameOn = true;

  startGame(&game, &shapes, &shape, &next_figure, &GameOn);

  ck_assert_int_eq(game.score, 0);
  ck_assert_int_eq(game.high_score, 0);
  ck_assert_int_eq(game.level, 1);
  ck_assert_int_eq(game.speed, 700000);
  ck_assert_int_eq(game.pause, 0);
  ck_assert_int_ne(next_figure, -1);
  ck_assert(GameOn);

  for (int i = 0; i < ROWS; i++) {
    free(game.field[i]);
  }
  free(game.field);
  for (int i = 0; i < 4; i++) {
    free(game.next[i]);
  }
  free(game.next);
  for (int i = 0; i < MAX_UNITS; i++) {
    free(shape.matrix[i]);
  }
  free(shape.matrix);
}
END_TEST

START_TEST(test_getShape) {
  CurrentShape shape;
  initShape(&shape);
  Coordinates_t coords[MAX_UNITS] = {{0, 1}, {1, 1}, {2, 1}, {3, 1}};

  shape = getShape(shape, coords);

  for (int i = 0; i < MAX_UNITS; i++) {
    for (int j = 0; j < MAX_UNITS; j++) {
      if ((i == 0 && j == 1) || (i == 1 && j == 1) || (i == 2 && j == 1) ||
          (i == 3 && j == 1)) {
        ck_assert_int_eq(shape.matrix[i][j], 1);
      } else {
        ck_assert_int_eq(shape.matrix[i][j], 0);
      }
    }
  }

  for (int i = 0; i < MAX_UNITS; i++) {
    free(shape.matrix[i]);
  }
  free(shape.matrix);
}
END_TEST

START_TEST(test_CopyShapeToNext) {
  GameInfo_t game;
  TetrisShape_t shapes;
  int next_figure = 0;

  initFigures(&shapes);
  initField(&game);

  CopyShapeToNext(&game, &shapes, next_figure);

  bool found = false;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game.next[i][j] != 0) {
        found = true;
        break;
      }
    }
    if (found) break;
  }
  ck_assert(found);

  for (int i = 0; i < ROWS; i++) {
    free(game.field[i]);
  }
  free(game.field);
  for (int i = 0; i < 4; i++) {
    free(game.next[i]);
  }
  free(game.next);
}
END_TEST

START_TEST(test_SetNewRandomShape) {
  GameInfo_t game;
  TetrisShape_t shapes;
  CurrentShape shape;
  int next_figure;
  bool GameOn = true;

  initFigures(&shapes);
  startGame(&game, &shapes, &shape, &next_figure, &GameOn);
  SetNewRandomShape(&shapes, &shape, &game, &next_figure, &GameOn);

  ck_assert_int_eq(shape.col, COLUMNS / 2 - 1);
  ck_assert_int_eq(shape.row, 0);

  for (int i = 0; i < ROWS; i++) {
    free(game.field[i]);
  }
  free(game.field);
  for (int i = 0; i < 4; i++) {
    free(game.next[i]);
  }
  free(game.next);
  for (int i = 0; i < MAX_UNITS; i++) {
    free(shape.matrix[i]);
  }
  free(shape.matrix);
}
END_TEST

START_TEST(test_mergeShapeToField) {
  GameInfo_t game;
  CurrentShape shape;

  initField(&game);
  shape.row = 0;
  shape.col = 0;
  shape.figure_index = 1;
  shape.matrix = (int **)malloc(MAX_UNITS * sizeof(int *));
  for (int i = 0; i < MAX_UNITS; i++) {
    shape.matrix[i] = (int *)malloc(MAX_UNITS * sizeof(int));
    for (int j = 0; j < MAX_UNITS; j++) {
      shape.matrix[i][j] = 0;
    }
  }
  shape.matrix[0][0] = 1;

  mergeShapeToField(&game, &shape);

  ck_assert_int_eq(game.field[0][0], 2);
  for (int i = 0; i < ROWS; i++) {
    free(game.field[i]);
  }
  free(game.field);
  for (int i = 0; i < MAX_UNITS; i++) {
    free(shape.matrix[i]);
  }
  free(shape.matrix);
}
END_TEST

START_TEST(test_clearFullLines) {
  GameInfo_t game;

  initField(&game);

  for (int j = 0; j < COLUMNS; j++) {
    game.field[0][j] = 1;
  }

  int cleared_lines = clearFullLines(&game);

  ck_assert_int_eq(cleared_lines, 1);
  for (int j = 0; j < COLUMNS; j++) {
    ck_assert_int_eq(game.field[0][j], 0);
  }
  for (int i = 0; i < ROWS; i++) {
    free(game.field[i]);
  }
  free(game.field);
}
END_TEST

START_TEST(test_saveScore_and_setHighScore) {
  GameInfo_t game;

  initField(&game);
  game.score = 2000;
  game.high_score = 1000;

  saveScore(&game);

  game.score = 0;
  game.high_score = 0;

  setHighScore(&game);

  ck_assert_int_eq(game.high_score, 2000);
  for (int i = 0; i < ROWS; i++) {
    free(game.field[i]);
  }
  free(game.field);
}
END_TEST

START_TEST(test_scoring) {
  GameInfo_t game;
  initGame(&game);

  scoring(&game, 1);
  ck_assert_int_eq(game.score, 100);

  scoring(&game, 2);
  ck_assert_int_eq(game.score, 400);

  scoring(&game, 3);
  ck_assert_int_eq(game.score, 1100);

  scoring(&game, 4);
  ck_assert_int_eq(game.score, 2600);
}
END_TEST

START_TEST(test_figRotation) {
  GameInfo_t game;
  TetrisShape_t shapes;
  CurrentShape shape;
  int next_figure;
  bool GameOn = true;

  initFigures(&shapes);
  startGame(&game, &shapes, &shape, &next_figure, &GameOn);
  SetNewRandomShape(&shapes, &shape, &game, &next_figure, &GameOn);
  figRotation(&game, &shapes, &shape);
  ck_assert(memcmp(&shape, &shapes.figures[next_figure].coords[1],
                   sizeof(CurrentShape)) != 0);
  ck_assert(isPositionValid(game.field, &shape, shape.row, shape.col));

  for (int i = 0; i < ROWS; i++) {
    free(game.field[i]);
  }
  free(game.field);
  for (int i = 0; i < MAX_UNITS; i++) {
    free(shape.matrix[i]);
  }
  free(shape.matrix);
}
END_TEST

START_TEST(test_levels) {
  GameInfo_t game;
  initGame(&game);

  ck_assert_int_eq(game.level, 0);

  game.score = 600;
  levels(&game);
  ck_assert_int_eq(game.level, 2);

  game.score = 1200;
  levels(&game);
  ck_assert_int_eq(game.level, 3);

  game.score = 5400;
  levels(&game);
  ck_assert_int_eq(game.level, 10);
}
END_TEST

START_TEST(test_hasToUpdate) {
  GameInfo_t game;
  game.speed = 1000000;

  gettimeofday(&last_update, NULL);

  ck_assert(!hasToUpdate(&game));

  sleep(1.1);

  ck_assert(hasToUpdate(&game));
}
END_TEST

START_TEST(test_freeMemory) {
  GameInfo_t game;
  CurrentShape shape;
  TetrisShape_t shapes;
  int next_figure = 0;
  bool GameOn = true;

  initFigures(&shapes);
  startGame(&game, &shapes, &shape, &next_figure, &GameOn);

  freeMemory(&game, &shape);

  ck_assert_ptr_ne(game.field, NULL);
  ck_assert_ptr_ne(shape.matrix, NULL);
}
END_TEST

Suite *tetris_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Tetris");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_initShape);
  tcase_add_test(tc_core, test_startGame);
  tcase_add_test(tc_core, test_getShape);
  tcase_add_test(tc_core, test_CopyShapeToNext);
  tcase_add_test(tc_core, test_SetNewRandomShape);
  tcase_add_test(tc_core, test_mergeShapeToField);
  tcase_add_test(tc_core, test_clearFullLines);
  tcase_add_test(tc_core, test_saveScore_and_setHighScore);
  tcase_add_test(tc_core, test_scoring);
  tcase_add_test(tc_core, test_figRotation);
  tcase_add_test(tc_core, test_levels);
  tcase_add_test(tc_core, test_hasToUpdate);
  tcase_add_test(tc_core, test_freeMemory);

  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = tetris_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}