#include "../../brick_game/tetris/libs/frontend.h"

void PrintNextFigure(GameInfo_t *game) {
  int win_h = 6;
  int win_w = 25;
  move(1, COLUMNS * 2 + 10);
  printw("NEXT_SHAPE");
  WINDOW *next_win = newwin(win_h, win_w, 2, COLUMNS * 2 + 2);
  for (int i = 0; i < 4; i++) {
    wmove(next_win, 2 + i, 10);
    for (int j = 0; j < 4; j++) {
      if (game->next[i][j] != TEMPTY) {
        ADDBLOCK(next_win, game->next[i][j]);
      } else {
        ADDEMPTY(next_win);
      }
    }
  }
  box(next_win, 0, 0);
  wnoutrefresh(next_win);
  delwin(next_win);
}

void printStat(GameInfo_t *game) {
  curs_set(0);
  int h = 13;
  int w = 25;
  WINDOW *stats_win = newwin(h, w, 9, COLUMNS * 2 + 2);
  move(8, COLUMNS * 2 + 12);
  printw("STATS");
  wmove(stats_win, 5, 9);
  wprintw(stats_win, "Level: %d\n", game->level);
  wprintw(stats_win, "         Score: %d\n", game->score);
  wprintw(stats_win, "     High score: %d\n", game->high_score);
  box(stats_win, 0, 0);
  wrefresh(stats_win);
  delwin(stats_win);
}

void initColors() {
  start_color();
  if (can_change_color()) {
    init_color(COLOR_ORANGE, 1000, 500, 0);  // Определение оранжевого цвета
    init_color(COLOR_PURPLE, 500, 0, 500);  // Определение фиолетового цвета
  }
  init_pair(TCELLI, COLOR_RED, COLOR_BLACK);
  init_pair(TCELLJ, COLOR_YELLOW, COLOR_BLACK);
  init_pair(TCELLL, COLOR_ORANGE, COLOR_BLACK);
  init_pair(TCELLO, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(TCELLS, COLOR_GREEN, COLOR_BLACK);
  init_pair(TCELLT, COLOR_BLUE, COLOR_BLACK);
  init_pair(TCELLZ, COLOR_PURPLE, COLOR_BLACK);
}

void PrintTable(GameInfo_t *game, CurrentShape *shape) {
  WINDOW *game_win = newwin(ROWS + 2, COLUMNS * 2 + 2, 0, 0);
  box(game_win, 0, 0);
  for (int i = 0; i < ROWS; i++) {
    wmove(game_win, 1 + i, 1);
    for (int j = 0; j < COLUMNS; j++) {
      if (game->field[i][j] != TEMPTY) {
        ADDBLOCK(game_win, game->field[i][j]);
      } else {
        ADDEMPTY(game_win);
      }
    }
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (shape->matrix[i][j] == TEMPTY) {
        continue;
      }
      int r = shape->row + i;
      int c = shape->col + j;
      if (r >= 0) {
        wmove(game_win, 1 + r, 1 + c * 2);
        ADDBLOCK(game_win, shape->figure_index +
                               1);
      }
    }
  }
  wnoutrefresh(game_win);
  delwin(game_win);
}

void to_pause(GameInfo_t *game, TetrisShape_t *shapes, CurrentShape *shape,
              bool hold, int *next_figure, bool *GameOn) {
  clear();
  move(8, 22);
  printw("PAUSE\n             Press ENTER to continue");
  refresh();
  char action;
  while (true) {
    action = getch();
    if (action == '\n') {
      break;
    } else if (action == 'q') {
      processUserAction(Terminate, game, shapes, shape, hold, next_figure,
                        GameOn);
    }
  };
  clear();
}

void endGame(GameInfo_t *game, TetrisShape_t *shapes, CurrentShape *shape,
             int *next_figure, bool *GameOn) {
  clear();
  int start_row = 7;
  int start_col = 20;
  mvprintw(start_row, start_col,
           "  ________                        ________                 \n");
  mvprintw(
      start_row + 1, start_col,
      " /  _____/_____    _____   ____   \\_____  \\___  __ ___________ \n");
  mvprintw(start_row + 2, start_col,
           "/   \\  ___\\__  \\  /     \\_/ __ \\   /   |   \\  \\/ // __ \\_  "
           "__ \\\n");
  mvprintw(start_row + 3, start_col,
           "\\    \\_\\  \\/ __ \\|  Y Y  \\  ___/  /    |    \\   /\\  ___/|  "
           "| \\/\n");
  mvprintw(
      start_row + 4, start_col,
      " \\______  (____  /__|_|  /\\___  > \\_______  /\\_/  \\___  >__|   \n");
  mvprintw(start_row + 5, start_col,
           "        \\/     \\/      \\/     \\/          \\/          \\/     "
           "  \n");
  mvprintw(start_row + 7, start_col,
           "                  Press ENTER to continue\n");
  refresh();
  while (getch() != '\n')
    ;
  exitGame(game, shapes, shape, next_figure, GameOn);
}