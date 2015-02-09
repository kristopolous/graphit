#include <iostream>
#include <vector>
#include <cmath>
#include <locale>
#include <unistd.h>
#define PI 3.14159265 

using namespace std;

class Graphit {
    public:
        Graphit(wstring cs) : charset( cs ){ }
        virtual ~Graphit(){ }
 
        void setchar(wstring cs) {
          charset = cs;
        }

        void print(
            vector<float> data, 
            int width, int height,
            float min, float max
        ) {
          int m_charlen = charset.size() - 1;

          int row, col;
          float delta = (max - min);
          float step = (float)data.size() / width;

          // first we reduce the number of items down to the number
          // of things we are going to graph. (the width).
          
          // For our use-case we'll do linear interpolation for going past
          // or resolution point and one of a variety of methods when
          // going the other way.

          // we are fitting x units into y space
          float nextval, lhs;
          float iy = 0.0, pos = 0.0, goal = step;
          int ix;

          vector<float> interpolate;

          for(ix = 0; ix < data.size();) {
            nextval = 0.0;

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
                nextval += (goal - iy - lhs) * data[ix];
              } else {
                // ix is already where we want it to be.
                nextval += (goal - iy) * data[ix];
              }
            }

            // this is our interpolated value.
            interpolate.push_back(nextval / step);
            iy = goal;
            goal += step;
          }

          // now we have to find out what the graph height should be
          // this is basically value - min / (max - min)
          vector<float> toGraph;
          vector<float>::const_iterator it;
          
          for(it = interpolate.begin(); it != interpolate.end(); it++) {
            toGraph.push_back( (int)(m_charlen * height * ((*it - min) / delta)));
          }

          // now we output the actual graph one row at a time.
          int tmp, current_floor, current_ceiling;
          for(row = 0; row < height; row++) {
            // if our value is less than this then it's a space.
            current_floor = (height - row - 1) * m_charlen;
            current_ceiling = current_floor + m_charlen;

            for(col = 0; col < width; col++) {
              tmp = toGraph[col];
              if(tmp <= current_floor) {
                tmp = 0;
              } else if (tmp >= current_ceiling) {
                tmp = m_charlen;
              } else {
                tmp %= m_charlen;
              }
              wcout << charset[ tmp ];
              toGraph[col] -= tmp;
            }

            wcout << endl;
          }
        }

    private:
        wstring charset;
};
 
int main( int argc, char **argv ){
  wstring awesome_unicode = L" \u2581\u2582\u2583\u2584\u2585\u2586\u2587\u2588";

  // :-(
  wstring sucker_ascii = L" _.-~'`";
 
  // Mainly just set up utf-8, so wcout won't narrow our characters.
  locale::global(locale("en_US.utf8"));

  Graphit sl(awesome_unicode);

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

    wcout<<"\033[0;0f";
    hei = 7;

    for(ix = 20; ix <= 80; ix+=30) {
      sl.setchar(awesome_unicode);

      sl.print(f, 
          ix, hei--,
          -1.5, 1.5);

      sl.print(f, 
          ix, (hei + 3) % 6 + 1,
          -1.5, 1.5);

      sl.setchar(sucker_ascii);

      sl.print(f, 
          ix, hei,
          -1.5, 1.5);

      sl.print(f, 
          ix, (hei + 3) % 6 + 1,
          -1.5, 1.5);
    }
    usleep(60000);
  }

  return 0;
}
