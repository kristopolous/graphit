#include <iostream>
#include "graphit.h"

wstring Graphit::m_charset = L"";

Graphit::Graphit() {
  if(m_charset.length() == 0) {
    Graphit::use_unicode(true);
  }
}

bool Graphit::use_unicode(bool value) {
  if(value) {
    m_charset = L" \u2581\u2582\u2583\u2584\u2585\u2586\u2587\u2588";
  } else {
    m_charset = L" _.-~'`";
  }
  return value;
}

int Graphit::interpolate(vector<float>&interpolated, vector<float> data, int width ) {
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
    interpolated.push_back(nextval / step);
    iy = goal;
    goal += step;
  }

  return 0;
}

int Graphit::rasterize(
    vector<float> &rasterized,
    vector<float> interpolated, 
    int height, float min, float max) {

  int m_charlen = Graphit::m_charset.size() - 1;
  float delta = (max - min);

  // now we have to find out what the graph height should be
  // this is basically value - min / (max - min)
  vector<float>::const_iterator it;
  
  for(it = interpolated.begin(); it != interpolated.end(); it++) {
    rasterized.push_back( (int)(m_charlen * height * ((*it - min) / delta)));
  }

  return 0;
}

int Graphit::plot(
    vector<wstring> &buffer,
    vector<float> rasterized, 
    int width, int height) {

  int m_charlen = m_charset.size() - 1;

  int 
    row_ix,
    col_ix,
    tmp, 
    current_floor, 
    current_ceiling;

  wstring col;
  
  for(row_ix = 0; row_ix < height; row_ix++) {
    col.clear();

    // if our value is less than this then it's a space.
    current_floor = (height - row_ix - 1) * m_charlen;
    current_ceiling = current_floor + m_charlen;

    for(col_ix = 0; col_ix < width; col_ix++) {
      tmp = rasterized[col_ix];
      if(tmp <= current_floor) {
        tmp = 0;
      } else if (tmp >= current_ceiling) {
        tmp = m_charlen;
      } else {
        tmp %= m_charlen;
      }
      col += m_charset[ tmp ];
      rasterized[col_ix] -= tmp;
    }
    buffer.push_back(col);
  }

  return 0;
}

int Graphit::process(
  vector<wstring> &output,
  vector<float> data, 
  int width, int height,
  float min, float max
) {
  int ret = 0;

  if(min > max || width <= 0 || height <= 0) {
    return -1;
  }

  vector<float> interpolation;
  vector<float> toPlot;

  ret = this->interpolate( interpolation, data, width);
  if(ret) {
    return ret;
  }

  ret = this->rasterize( toPlot, interpolation, height, min, max);
  if(ret) {
    return ret;
  }

  ret = this->plot( output, toPlot, width, height);
  if(ret) {
    return ret;
  }

  return ret;
}

int Graphit::print_curses(
    vector<float> data, 
    WINDOW*win,
    float min, float max
) {
  int ret = 0;

  // let's get the width and height from an ncurses macro
  if(!win || min > max) {
    return -1;
  }

  int width, height;
  getmaxyx(win, height, width);

  vector<wstring> output;
  ret = this->process(
      output, data,
      width, height,
      min, max
  );

  if(ret) {
    return ret;
  }
  
  int row = 0;
  vector<wstring>::const_iterator it;
  for(it = output.begin(); it != output.end(); it++) {
    mvwaddnwstr(
      win,
      row++, 0,
      (const wchar_t *)((*it).c_str()), 
      (*it).size()
    );
  }

  wrefresh(win); 
  return ret;
}

int Graphit::print_direct(
    vector<float> data, 
    int width, int height,
    float min, float max
) {
  int ret = 0;

  vector<wstring> output;
  ret = this->process(
      output, data,
      width, height,
      min, max
  );

  if(ret) {
    return ret;
  }

  vector<wstring>::const_iterator it;
  for(it = output.begin(); it != output.end(); it++) {
    wcout << (*it) << endl;
  }

  return ret;
}
