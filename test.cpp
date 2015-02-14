#include <iostream>
#include <cmath>
#include <locale>
#include <unistd.h>
#include "graphit.h"

#define PI 3.14159265 

int main( int argc, char **argv ){
  locale::global(locale("en_US.utf8"));

  initscr();  
  cbreak();      
  curs_set(0);
  WINDOW *local_win;

  local_win = newwin(10, 60, 5, 5);
  box(local_win, 0 , 0);

  Graphit sl;

  vector<float> f;
  int ix, iy;
  int hei = 9;
  float val;

  for(iy = 0; iy < 10; iy++) {
    f.push_back((float)sin (iy*PI/18));
  }


  for(;;) {
    if(iy > 300) {
      f.erase(f.begin());
    }
    iy++;
    val = sin (iy * PI/18);
    f.push_back(val);

    sl.print_curses(f, local_win, -1.5, 1.5);


    /*
    hei = 7;
    wcout<<"\033[0;0f";

    sl.print(f, 
        50, hei--,
        -1.5, 1.5);

  
    sl.print(f, 
        70, (hei + 3) % 6 + 1,
        -1.5, 1.5);
        */

    usleep(60000);
  }

  return 0;
}
