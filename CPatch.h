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
  ~CPatch(){
    std::cout << "patch destructor" << std::endl;
    //for(int i = 0; i < patch.size(); ++i)
    //patch.at(i).release();
  }

  void setPatch(int x, int y, int w, int h, cv::Mat image){
    //patch = image(cv::Rect(x, y, w, h));
  }

  void setPatch(cv::Rect roi,const cv::vector<cv::Mat*> &image, std::vector<cv::Point> centerPoint, int cn){
    patch.clear();
    //vCenter.clear();
    //xstd::cout << image->size() << std::endl;
    
    patchRoi = roi;
    classNum = cn;
    //image(roi);
    //vCenter = center;
    for(int i = 0; i < image.size(); ++i)
      patch.push_back((*image.at(i))(cv::Rect(roi)));
    //std::cout << patch.size() << std::endl;
    //p_image.push_back(&(image->at(i)));
    cv::Point tempPoint;
    center.x =  centerPoint.at(0).x - roi.x - (roi.width/2);
    center.y =  centerPoint.at(0).y - roi.y - (roi.height/2);
  }
  cv::Rect patchRoi;
  cv::Point center;
  cv::vector<cv::Mat> patch;
  int classNum;
  //std::vector<cv::Mat *> p_image;

  //HoG hog;
};

#endif
