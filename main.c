#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

#define WIDTH 40
#define HEIGHT 20

struct Node {
  int x, y;
  struct Node* next;
} *head = NULL, *current, *new_node;

int old_x, old_y;
int temp_x, temp_y;
int first;
int apple_x = 35, apple_y = 10;
int score = 0;
int grow = 0;

void add_segment(int x, int y) {
  new_node = malloc(sizeof(struct Node));

  new_node->x = x;
  new_node->y = y;

  new_node->next = NULL;

  if (!head) {
    head = new_node;
    return;
  }

  current = head;
  while (current->next != NULL) current = current->next;
  current->next = new_node;

}

void init_snake() {
  add_segment(8, 7);
  add_segment(7, 7);
  add_segment(6, 7);
  add_segment(5, 7);
}

void draw_everything(WINDOW* win) {
  wclear(win);
  box(win, 0, 0);

  mvwprintw(win, apple_y, apple_x, "*");
  mvprintw(0, 15, "Score: %d", score, win);

  current = head;
  first = 1;
  while(current != NULL) {
    if(
        current->x == 0 || 
        current->y == 0 ||
        current->x == (WIDTH - 1) ||
        current->y == (HEIGHT - 1)
    ) {
      endwin();
      exit(0);
    }

    // mvwaddch(WINDOW *win, int y, int x, const chtype ch);
    mvwaddch(win, current->y, current->x, first ? '@' : 'o');

    // When snake eats the apple
    if(current->x == apple_x && current->y == apple_y){
      grow = 1;
      score++;
      apple_x = rand() % (WIDTH - 2) + 1;
      apple_y = rand() % (HEIGHT - 2) + 1;
    };

    current = current->next;
    first = 0;
  }

}

void snake_move(int x,int y){
    current = head;

    old_x = current->x;
    old_y = current->y;

    current->x += x;
    current->y += y;

    current = current->next;

    while(current != NULL){
        temp_x = current->x;
        temp_y = current->y;

        current->x = old_x;
        current->y = old_y;

        old_x = temp_x;
        old_y = temp_y;

        current = current->next;
    }

    if(grow) {
      add_segment(old_x, old_y);
      grow = 0;
    }
}

int main() {
  initscr();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  cbreak();
  refresh();
  nodelay(stdscr, TRUE);
  srand(time(NULL));

  WINDOW *win = newwin(HEIGHT, WIDTH, 1, 1);
  init_snake();
  draw_everything(win);
  wrefresh(win);

  // moving logic
  int dx = 1, dy = 0;
  while(true) {
    usleep(350000);

    int ch = getch();
    switch(ch) {
      case KEY_UP: dx=0; dy=-1; break;
      case KEY_DOWN: dx=0; dy=1;  break;
      case KEY_LEFT: dx=-1; dy=0; break;
      case KEY_RIGHT: dx=1; dy=0;  break;
      case 'q': endwin(); exit(0); break;
    }

    snake_move(dx, dy);
    draw_everything(win);

    wrefresh(win);
  }

  draw_everything(win);
  wrefresh(win);

  getch();
  endwin();

  return 0;
}
