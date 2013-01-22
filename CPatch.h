#ifndef __CPATCH__
#define __CPATCH__

#include "util.h"

class CPatch 
{
 public:
 CPatch(cv::Rect roi, cv::Mat* image)
   : patchRoi(roi)
    {
      //patch = image(cv::Rect(roi));
    }
  
  CPatch(){}

  void setPatch(int x, int y, int w, int h, cv::Mat image){
    //patch = image(cv::Rect(x, y, w, h));
  }

  void setPatch(cv::Rect roi,const std::vector<cv::Mat> &image, std::vector<cv::Point> center){
    patch.clear();
    //xstd::cout << image->size() << std::endl;
    
    patchRoi = roi;
    //image(roi);
    //vCenter = center;
    for(int i = 0; i < image.size(); ++i)
      patch.push_back((image.at(i))(cv::Rect(roi)));
    //std::cout << patch.size() << std::endl;
    //p_image.push_back(&(image->at(i)));
    cv::Point tempPoint;
    for(int i = 0; i < center.size(); ++i)
      vCenter.push_back(cv::Point(center.at(i).x - roi.x - (roi.width/2),
				  center.at(i).y - roi.y - (roi.height/2)));
  }
  cv::Rect patchRoi;
  std::vector<cv::Point> vCenter;
  std::vector<cv::Mat> patch;
  //std::vector<cv::Mat *> p_image;

  //HoG hog;
  
  
  
};

#endif
