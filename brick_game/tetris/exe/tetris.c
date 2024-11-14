#include "../libs/frontend.h"

int main() {
  TetrisShape_t shapes;
  GameInfo_t game;
  CurrentShape shape;
  gettimeofday(&last_update, NULL);
  initscr();
  initColors();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  int next_figure;
  bool GameOn = TRUE;
  startGame(&game, &shapes, &shape, &next_figure, &GameOn);
  while (GameOn) {
    saveScore(&game);
    setHighScore(&game);
    UserAction_t action = Start;
    bool hold = false;
    userInput(&action, &hold);
    processUserAction(action, &game, &shapes, &shape, hold, &next_figure,
                      &GameOn);
    if (hasToUpdate(&game)) {
      UserAction_t action = Down;
      processUserAction(action, &game, &shapes, &shape, hold, &next_figure,
                        &GameOn);
      gettimeofday(&last_update, NULL);
    }
    PrintTable(&game, &shape);
    PrintNextFigure(&game);
    printStat(&game);
  }
  freeMemory(&game, &shape);
  endwin();
  return 0;
}