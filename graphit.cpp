#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <locale>
#include <stdio.h>
#include <unistd.h>
#define PI 3.14159265 
using namespace std;
class Sparkline {
    public:
        Sparkline(std::wstring &cs) : charset( cs ){ }
        virtual ~Sparkline(){ }
 
        void print(std::vector<float> data, 
            int width, int height,
            float min, float max
        ) {
          int row, col;
          float delta = (max - min);

          // first we reduce the number of items down to the number
          // of things we are going to graph. (the width).
          
          // For our use-case we'll do linear interpolation for going past
          // or resolution point and one of a variety of methods when
          // going the other way.

          // we are fitting x units into y space
          float step = (float)data.size() / (float)width;
          float nextval, lhs;
          float iy = 0, pos = 0.0, goal = step;
          int ix = 0;

          vector<float> interpolate;
          nextval = 0.0;

          for(;;) {

            for(; (iy + 1) < goal; iy++) {
              nextval += data[ix];
              ix++;
            }
            // and this point we've gone through all the 
            // integer values to interpolate ... we may have
            // a fractional value left.
            if(goal > iy) {
              if((int)goal > (int)iy) {
                lhs = (int)goal - iy;
                nextval += lhs * data[ix];

                ix++;
                nextval += (goal - iy -lhs) * data[ix];
              } else {
                // ix is already where we want it to be.
                nextval += (goal - iy) * data[ix];
              }
            }

            // this is our interpolated value.
            interpolate.push_back(nextval / step);
            iy = goal;
            goal += step;
            nextval = 0.0;

            if(ix >= data.size()) {
              break;
            }
          }

          // now we have to find out what the graph height should be
          // this is basically value - min / (max - min)
          vector<float> toGraph;
          vector<float>::const_iterator it;
          
          for(it = interpolate.begin(); it != interpolate.end(); it++) {
            toGraph.push_back( (int)(8.0 * height * ((*it - min) / delta)));
          }

          // now we output the actual graph one row at a time.
          int tmp, current_floor, current_ceiling;
          for(row = 0; row < height; row++) {
            // if our value is less than this then it's a space.
            current_floor = (height - row - 1) * 8;
            current_ceiling = current_floor + 8;

            for(col = 0; col < width; col++) {
              tmp = toGraph[col];
              if(tmp <= current_floor) {
                wcout << " ";
              } else {
                if (tmp >= current_ceiling) {
                  tmp = 8;
                } else {
                  tmp %= 8;
                }
                wcout << charset[ tmp ];
                toGraph[col] -= tmp;
              }
            }

            wcout<<endl;
          }
 
        }
    private:
        std::wstring &charset;
};
 
int main( int argc, char **argv ){
  std::wstring charset = L" \u2581\u2582\u2583\u2584\u2585\u2586\u2587\u2588";
 
    // Mainly just set up utf-8, so wcout won't narrow our characters.
    locale::global(locale("en_US.utf8"));
 
    Sparkline sl(charset);

    /*
    float tmp[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    vector<float> f (tmp, tmp + sizeof(tmp) / sizeof(tmp[0]) ); 
    */
    vector<float> f;// (tmp, tmp + sizeof(tmp) / sizeof(tmp[0]) ); 

    int ix, iy;
    for(iy = 0; iy < 5; iy++) {
      f.push_back((float)sin (iy*PI/18));
    }


    for(;;) {
      if(iy > 300) {
        f.erase(f.begin());
      }
      iy++;
      f.push_back((float)sin (iy*PI/18));

      wcout<<"\033[0;0f";
      for(ix = 30; ix < 120; ix+=20) {
      sl.print(f, 
          ix, 8,
          -2, 2);
      }
      usleep(60000);
    }

   
    return 0;
}
