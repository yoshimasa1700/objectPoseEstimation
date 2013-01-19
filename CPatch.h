#ifndef __CPATCH__
#define __CPATCH__

#include "util.h"
#include "HoG.h"

static HoG hog;

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
    vCenter = center;
  }
  
  cv::Rect patchRoi;
  std::vector<cv::Point> vCenter;
  std::vector<cv::Mat> patch;
  //std::vector<cv::Mat *> p_image;



  void extractFeatureChannels(const cv::Mat& img, std::vector<cv::Mat>& vImg);
};

inline void CPatch::extractFeatureChannels(const cv::Mat& img, std::vector<cv::Mat>& vImg){
  vImg.resize(32);
  for(int i = 0; i < 32; ++i)
    vImg.at(i) = cv::Mat(img.rows, img.cols, CV_8UC1);

  cv::cvtColor(img, vImg.at(0), CV_BGR2GRAY);

  cv::Mat I_x(img.rows, img.cols, CV_16SC1);
  cv::Mat I_y(img.rows, img.cols, CV_16SC1);

  cv::Sobel(vImg.at(0), I_x, CV_16S, 1, 0);
  cv::Sobel(vImg.at(0), I_y, CV_16S, 0, 1);

  cv::convertScaleAbs(I_x, vImg[3], 0.25);
  cv::convertScaleAbs(I_y, vImg[4], 0.25);

  // Orientation of gradients
  for(int  y = 0; y < img.rows; y++)
    for(int  x = 0; x < img.cols; x++) {
      // Avoid division by zero
      float tx = I_x.at<float>(y, x) + (float)_copysign(0.000001f, I_x.at<float>(y, x));
      // Scaling [-pi/2 pi/2] -> [0 80*pi]
      vImg.at(1).at<uchar>(y, x) = (uchar)(( atan((float)I_y.at<float>(y, x) / tx) + 3.14159265f / 2.0f ) * 80);

      vImg.at(2).at<uchar>(y, x) = (uchar)sqrt(I_x.at<float>(y, x)*I_x.at<float>(y, x) + I_y.at<float>(y, x) * I_y.at<float>(y, x));
    }

  // Magunitude of gradients
  for(int y = 0; y < img.rows; y++)
      for(int x = 0; x < img.cols; x++ ) {
	vImg.at(2).at<uchar>(y, x) = (uchar)sqrt(I_x.at<float>(y, x)*I_x.at<float>(y, x) + I_y.at<float>(y, x) * I_y.at<float>(y, x));
      }

  
}

#endif
