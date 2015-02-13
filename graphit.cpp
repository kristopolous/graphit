#include <iostream>
#include <cmath>
#include <locale>
#include <unistd.h>
#include "graphit.h"
#define PI 3.14159265 

using namespace std;

Graphit::Graphit() {
  if(m_charset.length() == 0) {
    Graphit::useUnicode(true);
  }
}

bool Graphit::useUnicode(bool value) {
  if(value) {
    m_charset = L" \u2581\u2582\u2583\u2584\u2585\u2586\u2587\u2588";
  } else {
    m_charset = L" _.-~'`";
  }
}
int Graphit::interpolate(vector<float>&interpolate, vector<float> data, int width ) {
  float step = (float)data.size() / width;

  // first we reduce the number of items down to the number
  // of things we are going to graph. (the width).
  
  // For our use-case we'll do linear interpolation for going past
  // or resolution point and one of a variety of methods when
  // going the other way.

  // we are fitting x units into y space
  float nextval, lhs;
  float iy = 0.0, pos = 0.0, goal = step;

  for(int ix = 0; ix < data.size();) {
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
  return 0;
}

int Graphit::rasterize(
    vector<float> &toGraph,
    vector<float> interpolate, int height, float min, float max) {

  int m_charlen = Graphit::m_charset.size() - 1;
  float delta = (max - min);

  // now we have to find out what the graph height should be
  // this is basically value - min / (max - min)
  vector<float>::const_iterator it;
  
  for(it = interpolate.begin(); it != interpolate.end(); it++) {
    toGraph.push_back( (int)(m_charlen * height * ((*it - min) / delta)));
  }

  return 0;
}

vector<wstring> Graphit::plot(vector<float> toPlot, int width, int height) {
  int m_charlen = m_charset.size() - 1;

  // now we output the actual graph one row at a time.
  int 
    row_ix,
    col_ix,
    tmp, 
    current_floor, 
    current_ceiling;

  wstring col;
  vector<wstring> buffer;
  
  for(row_ix = 0; row_ix < height; row_ix++) {
    col.empty();

    // if our value is less than this then it's a space.
    current_floor = (height - row_ix - 1) * m_charlen;
    current_ceiling = current_floor + m_charlen;

    for(col_ix = 0; col_ix < width; col_ix++) {
      tmp = toPlot[col_ix];
      if(tmp <= current_floor) {
        tmp = 0;
      } else if (tmp >= current_ceiling) {
        tmp = m_charlen;
      } else {
        tmp %= m_charlen;
      }
      col += m_charset[ tmp ];
      toPlot[col_ix] -= tmp;
    }
    buffer.push_back(col);
  }
  return buffer;
}

void Graphit::print(
    vector<float> data, 
    int width, int height,
    float min, float max
) {
  int m_charlen = m_charset.size() - 1;

  int row, col;

  vector<float> interpolation;
  this->interpolate(
      interpolation, data, width);

  vector<float> toGraph;
  this->rasterize(
    toGraph, interpolation, 
    height, min, max);

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
      wcout << m_charset[ tmp ];
      toGraph[col] -= tmp;
    }

    wcout << endl;
  }
}
 
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
