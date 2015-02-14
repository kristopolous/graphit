#include <vector>
#include <ncursesw/ncurses.h>

#ifndef __d8b39cb3_015d_4087_a042_d2c7d98f5eba_GRAPHIT
#define __d8b39cb3_015d_4087_a042_d2c7d98f5eba_GRAPHIT

using namespace std;

class Graphit {
  public:
    Graphit();

    static bool useUnicode(bool value);

    int print_curses(
        vector<float> data, 
        WINDOW*win,
        float min, float max
    );

    int print(
        vector<float> data, 
        int width, int height,
        float min, float max
    );

  private:

    int process(
      vector<wstring> &output,
      vector<float> data, 
      int width, int height,
      float min, float max
    );

    int interpolate(
        vector<float> &interpolate, 
        vector<float> data, 
        int width);

    int plot(
        vector<wstring> &buffer,
        vector<float> toPlot,
        int width, int height);

    int rasterize(
        vector<float> &toGraph,
        vector<float> interpolate, int height, float min, float max);

    static wstring m_charset;
};

#endif
