#include <iostream>
#include <cmath>
#include <locale>
#include <unistd.h>
#include "graphit.h"
#define PI 3.14159265 

int main( int argc, char **argv ){
 
  // Mainly just set up utf-8, so wcout won't narrow our characters.
  locale::global(locale("en_US.utf8"));

  Graphit sl;

  vector<float> f;

  int ix, iy;
  int hei = 9;
  float val;
  for(iy = 0; iy < 5; iy++) {
    f.push_back((float)sin (iy*PI/18));
  }


  for(;;) {
    if(iy > 300) {
      f.erase(f.begin());
    }
    iy++;
    val = sin (iy*PI/18);
    f.push_back(val);

    hei = 7;

    wcout<<"\033[0;0f";
    for(ix = 80; ix <= 80; ix+=30) {
      //sl.setchar(awesome_unicode);

      sl.print(f, 
          ix, hei--,
          -1.5, 1.5);

      sl.print(f, 
          ix, (hei + 3) % 6 + 1,
          -1.5, 1.5);

    }
    usleep(60000);
  }

  return 0;
}