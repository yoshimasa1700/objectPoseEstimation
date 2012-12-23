#ifndef __CPATCH__
#define __CPATCH__

#include <opencv2/opencv.hpp>
#include "CDataset.h"
#include <vector>

class CPatch 
{
 public:
 CPatch(int w, int h, int num_l)
   : width(w), height(h){}
  
  //extract patch
  void extractPatches(std::vector<CDataset> dataSet);
  
 private:
  int width;
  int height;
  //std::vector<cv::Mat> patches;
  std::vector<cv::Rect> vPatches;
  std::vector<cv::Point> vCenter;
  std::vector<std::vector<cv::Mat> > images;
};

#endif
