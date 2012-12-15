#ifndef __CPATCH__
#define __CPATCH__

#include <opencv2/opencv.hpp>

class CPatch 
{
 public:
 CPatch(CvRNG* pRNG, int w, int h, int num_l)
   : cvRNG(pRNG), width(w), height(h){}
  
  //extract patch and hold patch
  void extractPatches(cv::Mat *img,
		      unsigned int n,
		      int label,
		      cv::Rect* box = 0,
		      std::vector<cv::Point>* vCenter = 0);
  
 private:
  CvRNG *cvRNG;
  int width;
  int height;
  std::vector<cv::Mat> patches;
};

#endif
