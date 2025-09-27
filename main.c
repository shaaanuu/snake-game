#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

struct Node {
  int x, y;
  struct Node* next;
} *head = NULL;

void add_segment(int x, int y) {
  struct Node* new_node = malloc(sizeof(struct Node));
  new_node->x = x;
  new_node->y = y;
  new_node->next = head;
  head = new_node;
}

void init_snake() {
  add_segment(5, 7);
  add_segment(6, 7);
  add_segment(7, 7);
  add_segment(8, 7);
}

void draw_everything(WINDOW* win) {
  wclear(win);
  box(win, 0, 0);

  struct Node* current = head;
  int first = 1;
  while(current != NULL) {
    // mvwaddch(WINDOW *win, int y, int x, const chtype ch);
    mvwaddch(win, current->y, current->x, first ? '@' : 'o');
    current = current->next;
    first = 0;
  }
}

void move_snake() {
  struct Node* current = head;
  while(current != NULL) {
    current->x = current->x+1;

    current = current->next;
  }
}

int main() {
  initscr();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  cbreak();

  refresh();

  WINDOW *win = newwin(20, 50, 1, 1);
  init_snake();

  draw_everything(win);

  //refresh();  // draw stdscr background
  wrefresh(win);

  // adding the body logic
  usleep(500000);
  add_segment(9,7);
  draw_everything(win);
  wrefresh(win);

  // moving logic
  while(true) {
    usleep(500000);
    move_snake();
    draw_everything(win);
    wrefresh(win);
  }

  getch();
  endwin();
  
  return 0;
}



