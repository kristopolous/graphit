#include <vector>
#include <ncursesw/ncurses.h>

#ifndef __d8b39cb3_015d_4087_a042_d2c7d98f5eba_GRAPHIT
#define __d8b39cb3_015d_4087_a042_d2c7d98f5eba_GRAPHIT

using namespace std;

class Graphit {
  public:
    Graphit();

    // Toggles on or off unicode characters
    static bool use_unicode(bool value);

    // Takes a vector of floats, data and renders 
    // inside of a curses window, win, plotting between
    // Y values min and max.
    int print_curses(
        vector<float> data, 
        WINDOW *win,
        float min, float max);

    // Identical to print_curses but prints the 
    // results directly to the screen via wcout 
    // calls.
    //
    // Returns 0 on success.
    int print(
        vector<float> data, 
        int width, int height,
        float min, float max);

  private:

    // Process will take a vector of floats,
    // data and then do an 
    //
    //  1 interpolation
    //  2 rasterization
    //  3 plot
    //
    // of that data with respect to the width
    // height, min, and max specified. The
    // results will go into the output.
    //
    // Generally speaking this is all that
    // is needed to interact with directly.
    //
    // Returns 0 on success.
    int process(
        vector<wstring> &output,
        vector<float> data, 
        int width, int height,
        float min, float max);

    // Takes the vector data and does
    // a linear interpolation of it to
    // a width, width, into the vector
    // interpolated.  Returns 0 on success.
    int interpolate(
        vector<float> &interpolated, 
        vector<float> data, 
        int width);

    // Takes an vector interpolated,
    // a plotting height, height, and
    // a minimum and maximum to plot,
    // min and max, and scales the 
    // interpolation to values
    // which can be plotted.
    // Returns 0 on success.
    int rasterize(
        vector<float> &rasterized,
        vector<float> interpolated, 
        int height, 
        float min, float max);

    // Takes a vector, rasterized,
    // and a given width and height,
    // and fills up a vector of wstrings
    // with the character set according
    // to those values
    // Returns 0 on success.
    int plot(
        vector<wstring> &buffer,
        vector<float> rasterized,
        int width, int height);


    // The plotting charset, defined
    // statically through useUnicode
    static wstring m_charset;
};

#endif
