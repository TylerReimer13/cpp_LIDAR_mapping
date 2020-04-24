#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include <math.h> 
#include "occupancy_grid.h"

using namespace cv;
using namespace std;  

Mapping::Mapping() {};

void Mapping::make_map() {
   int pix_y_counter = -200;
   for (int i=0; i<NUM_ROWS; i++) {
      int pix_x_counter = -200;
      for (int j=0; j<NUM_COLS; j++) {
         map[i][j] = .5;
         Cell cell;
         cell.id = id_counter;
         cell.x = (double)pix_x_counter/40;
         cell.y = (double)-pix_y_counter/40;
         cell.pixel_x = pix_x_counter;
         cell.pixel_y = -pix_y_counter;
         cell.opencv_x = j;
         cell.opencv_y = i;
         cell.value=128;
         cells.push_back(cell);
         id_counter++;
         pix_x_counter++;
      }
   pix_y_counter++;
   }
};

void Mapping::receive_scan(double laser_info[41]) {
   for (int c=0; c<41; c++) {
      current_scan[c] = laser_info[c];
   }
};

void Mapping::update_pos(double pos[2]) {
   position[0] = pos[0];
   position[1] = pos[1];
};

void Mapping::set_cell_by_pos(double x, double y) {
   for (int n=0; n<cells.size(); n++) {
      if (cells[n].x == x && cells[n].y == y) {
         cells[n].value = 0;
      }
   }
};

double Mapping::dist(float x1, float y1, float x2, float y2) {
   double delt_y = y2-y1;
   double delt_x = x2-x1;

   double x_dist_sqrd = pow(delt_x, 2);
   double y_dist_sqrd = pow(delt_y, 2);
   double dist = sqrt(x_dist_sqrd+y_dist_sqrd);
   return dist;
};

Cell Mapping::get_closest_cell(double pos_x, double pos_y) {
   float closest_so_far = 10000000.;
   Cell candidate_cell;
   for (int k=0; k<cells.size(); k++) {
      double distance = dist(cells[k].x, cells[k].y, pos_x, pos_y);
      if (distance < closest_so_far) {
         closest_so_far = distance;
         candidate_cell = cells[k];
      }
   }
   return candidate_cell;
};

void Mapping::update_map() {
   Cell this_cell;
   double range;
   double global_x;
   double global_y;
   double angle_deg;
   double theta;
   double heading_deg=0.; // CLOCKWISE ROTATION

   for (int f=0; f<41; f++) {
      if (current_scan[f] < 2.5) {
         range = current_scan[f];
         angle_deg = -20+f;
         global_x = position[0] + range*sin((heading_deg+angle_deg+90.)*.0175);
         global_y = position[1] + range*cos((heading_deg+angle_deg+90.)*.0175);
         this_cell = get_closest_cell(global_x, global_y);
         set_cell_by_pos(this_cell.x, this_cell.y);
      }
   }
};

void Mapping::show_map(Mat& image) {
   for (int b=0; b<cells.size(); b++) {
      image.at<uchar>(cells[b].opencv_y, cells[b].opencv_x) = cells[b].value;
   }
   image.at<uchar>(200, 200) = 255.;
   imshow( "Display window", image);
   waitKey(0);  
};

int main() {
   Mapping mapper;
   mapper.make_map();
   Mat image(400, 400, CV_8UC1, Scalar(100));
   namedWindow( "Display window", (2000,2000) );

   double test_scan[41] = {2., 2., 2., 2., 2., 300., 300., 300., 300., 300., 300., 300., 300., 300., 300., 300., 300., 300., 300., 300., 2.4, 300., 300., 300., 300., 300., 300., 300., 300., 300., 300.,
   300., 300., 300., 300., 300., 2., 2., 2., 2., 2.};
   mapper.receive_scan(test_scan);  
   mapper.update_map();
    
   mapper.show_map(image); 

   return 0;
};
