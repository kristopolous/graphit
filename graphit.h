#include <vector>

using namespace std;

class Graphit {
  public:
    Graphit();

    static bool useUnicode(bool value);
    int interpolate(vector<float>&interpolate, vector<float> data, int width );

    vector<wstring> plot(vector<float> toPlot, int width, int height);

    int rasterize(
        vector<float> &toGraph,
        vector<float> interpolate, int height, float min, float max);

    void print(
        vector<float> data, 
        int width, int height,
        float min, float max);

    static wstring m_charset;
};

wstring Graphit::m_charset = L"";
