#include "../libs/frontend.h"

void userInput(UserAction_t *action, bool *hold) {
  int input = getch();

  switch (input) {
    case KEY_LEFT:
      *action = Left;
      break;
    case KEY_RIGHT:
      *action = Right;
      break;
    case KEY_UP:
      *action = Up;
      break;
    case KEY_DOWN:
      *action = Down;
      break;
    case 'q':
      *action = Terminate;
      break;
    case ' ':
      *action = Action;
      break;
    case 'p':
      *action = Pause;
      break;
    default:
      *action = Start;
      break;
  }
  *hold = input != ERR;
}

void processUserAction(UserAction_t action, GameInfo_t *game,
                       TetrisShape_t *shapes, CurrentShape *shape, bool hold,
                       int *next_figure, bool *GameOn) {
  switch (action) {
    case Left:
      if (isPositionValid(game->field, shape, shape->row, shape->col - 1)) {
        shape->col--;
      }
      break;
    case Right:
      if (isPositionValid(game->field, shape, shape->row, shape->col + 1)) {
        shape->col++;
      }
      break;
    case Up: {
      figRotation(game, shapes, shape);
      break;
    }
    case Down:
      if (isPositionValid(game->field, shape, shape->row + 1, shape->col)) {
        shape->row++;
      } else {
        mergeOldInitNew(game, shapes, shape, next_figure, GameOn);
      }
      break;
    case Action:
      while (isPositionValid(game->field, shape, shape->row + 1, shape->col)) {
        shape->row++;
      }
      mergeOldInitNew(game, shapes, shape, next_figure, GameOn);
      break;
    case Pause:
      to_pause(game, shapes, shape, hold, next_figure, GameOn);
      break;
    case Terminate:
      endwin();
      exit(0);
      break;
    case Start:
    default:
      break;
  }
}