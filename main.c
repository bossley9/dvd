#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Any ascii symbol can be used here. The only requirement
 * is that each line uses the same string length.
 */
char *IMG[] = {
    "  @@@@@@@@@@      @@@@@@@@@  ", //
    "       @@@@@@    @@      @@@@", //
    " @@@   @@@@#@@ @@  @@@   @@@@", //
    "@@@@*#@@*   @@@*  @@@@*#@@*  ", //
    "            -@               ", //
    " -+*#***@+@*@@@**@#+****+-   ", //
    "--*##*@@@+@@@@@@@@#++*#*++-- ", //
};

/*
 * Measured in sec. Value must be greater than zero.
 */
float SPEED = 0.12;

/* returns image width */
int get_img_width() {
  int x;
  for (x = 0; IMG[0][x] != '\0'; ++x)
    ;
  return x;
}

/* returns image height */
int get_img_height() { return sizeof IMG / sizeof IMG[0]; }

/* prints image onto the screen */
void print_img_at(int x, int y) {
  int i;
  int img_height = get_img_height();

  for (i = 0; i < img_height; i++) {
    char *line = IMG[i];
    /* IMG is defined at compile time */
    /* disregard -Wformat-security */
    mvwprintw(stdscr, y + i, x, line);
  }
}

int clamp(int min, int val, int max) {
  int x = val;
  if (x < min)
    x = min;
  if (x > max)
    x = max;
  return x;
}

void setup_colors() {
  /* start coloring */
  start_color();
  /* enable default colors */
  /* this allows transparent backgrounds */
  use_default_colors();

  /* 1-based index because color pair 0 isn't valid */
  /* blue */
  init_color(1, 0, 0, 1000);
  /* yellow */
  init_color(2, 1000, 1000, 0);
  /* pink */
  init_color(3, 1000, 0, 500);
  /* magenta */
  init_color(4, 1000, 0, 1000);
  /* cyan */
  init_color(5, 0, 1000, 1000);
  /* orange */
  init_color(6, 1000, 500, 0);
  /* red */
  init_color(7, 1000, 0, 0);

  init_pair(1, 1, -1);
  init_pair(2, 2, -1);
  init_pair(3, 3, -1);
  init_pair(4, 4, -1);
  init_pair(5, 5, -1);
  init_pair(6, 6, -1);
  init_pair(7, 7, -1);
}

/* set a random foreground color */
void set_rand_color() { attron(COLOR_PAIR((rand() % 6) + 1)); }

/* if side is hit */
void on_bounce() { set_rand_color(); }

/* if corner is hit */
void on_corner_hit() {
  /* celebrate */
  /* or contemplate existence */
}

/* main running loop */
void run(int x, int y, int dx, int dy) {
  char c;
  struct timespec request;
  int bounce = 0;
  float speed_sec = (int)SPEED;

  request.tv_sec = speed_sec;
  request.tv_nsec = (int)((SPEED - speed_sec) * 1000000000);

  set_rand_color();

  while (1) {
    if (bounce > 0) {
      on_bounce();
    }
    if (bounce == 2) {
      on_corner_hit();
    }
    bounce = 0;

    /* print image */
    print_img_at(x, y);
    /* write imaginary contents to real screen */
    refresh();

    /* set updates for next loop */
    x = x + dx;
    y = y + dy;
    /* to prevent bugs, we reset positions */
    /* within the bounding box */
    x = clamp(0, x, COLS - get_img_width());
    y = clamp(0, y, LINES - get_img_height());

    if (x + get_img_width() == COLS || x <= 0) {
      bounce = bounce + 1;
      dx = dx * -1;
    }
    if (y + get_img_height() == LINES || y <= 0) {
      bounce = bounce + 1;
      dy = dy * -1;
    }

    if ((c = getch()) == 'q')
      break;

    /* sleep */
    nanosleep(&request, NULL);
    /* clear screen */
    werase(stdscr);
  }
}

/* setup screen */
void setup() {
  /* screen setup and memory allocation */
  initscr();
  /* interpret interupts and signals as normal */
  cbreak();
  /* disable echoing input */
  noecho();
  /* enable Fx keys... because I want to */
  keypad(stdscr, TRUE);
  /* prevent std input from blocking */
  /* this is essential to running the loop */
  nodelay(stdscr, TRUE);
  /* hide cursor */
  curs_set(0);
  /* setup colors */
  setup_colors();
  /* init random number generator */
  srand(time(NULL));
}

int main() {
  int initial_x, initial_y;
  int dx, dy;

  setup();

  /* find starting vector */
  initial_x = rand() % (COLS - get_img_width());
  initial_y = rand() % (LINES - get_img_height());

  dx = rand() % 2;
  if (!dx) {
    dx = -1;
  }
  dy = rand() % 2;
  if (!dy) {
    dy = -1;
  }

  run(initial_x, initial_y, dx, dy);

  /* teardown screen and free memory */
  endwin();
  return 0;
}
