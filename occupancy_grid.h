#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include <math.h>  

using namespace cv;
using namespace std;

struct Cell {
   int id;
   double x;
   double y;
   int value;
   int pixel_x;
   int pixel_y;
   int opencv_x;
   int opencv_y;
};

struct scan {
   float x;
   float y;
   float angle;
};


class Mapping {
   private:
      int id_counter = 1;
      const int NUM_ROWS = 400;
      const int NUM_COLS = 400;
      double position[2] = {0., 0.};
      float current_scan[41];
      float map[400][400];
      std::vector<Cell> cells;

   public:
      Mapping();
      void make_map();
      void receive_scan(double laser_info[41]);
      void update_pos(double pos[2]);
      void set_cell_by_pos(double x, double y);
      double dist(float x1, float y1, float x2, float y2);
      Cell get_closest_cell(double pos_x, double pos_y);
      void set_cell_value(int id, int value);
      void update_map();
      void show_map(Mat& image);
};    

