#ifndef __CPATCH__
#define __CPATCH__

#include "util.h"

class CPatch 
{
 public:
 CPatch(int w, int h, int s, double ratio, int num_l)
   : width(w), height(h), stride(s), patchRatio(ratio){}
  
  //extract patch
  //void extractPatches(std::vector<CDataset> dataSet, boost::mt19937 gen);
  //void loadImages(std::vector<CDataset> dataSet);
  
  void setWitdh(int w){width = w;};
  void setHeight(int h){height = h;};
  void setStride(int s){stride = s;};
  
 private:
  int width;
  int height;
  int stride;
  double patchRatio;
  //std::vector<cv::Mat> patches;
  std::vector<std::vector<cv::Rect> > vPatches;
  std::vector<cv::Point> vCenter;
  std::vector<std::vector<cv::Mat> > images;
};

#endif
