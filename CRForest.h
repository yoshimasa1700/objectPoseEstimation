#ifndef __CRFOREST__

#define __CRFOREST__

#include "CRTree.h"
#include "CPatch.h"
#include "util.h"
#include "HoG.h"

static HoG hog;

class CRForest {
 public:
  CRForest(CConfig config){
    conf = config;
    vTrees.resize(conf.ntrees);
  }
  ~CRForest() {
    int numberOfTrees = vTrees.size();
    for(int i = 0;i < numberOfTrees;++i){
      if(vTrees.at(i) != NULL)
	delete vTrees.at(i);
    }
  }
  
  void learning();

  void growATree(const int treeNum);
  
  void detection(const CDataset &dataSet, 
		 const cv::vector<cv::Mat*> &image, std::vector<double> &detectionResult, int &detectClass) const;
  
  void extractPatches(std::vector<std::vector<CPatch> > &patches, 
		      const std::vector<CDataset> dataSet, 
		      const cv::vector<cv::vector<cv::Mat*> > &image, 
		      /*boost::mt19937 gen, */CConfig conf);

  void loadForest();
  void extractAllPatches(const CDataset &dataSet, 
			 const cv::vector<cv::Mat*> &image, 
			 std::vector<CPatch> &patches) const;
  // Regression 
  void regression(std::vector<const LeafNode*>& result, 
		  CPatch &patch) const;
  void loadImages(cv::vector<cv::vector<cv::Mat*> > &img, 
		  std::vector<CDataset> &dataSet);

  void extractFeatureChannels(const cv::Mat* img, 
			      cv::vector<cv::Mat*>& vImg) const;
  void minFilter(cv::Mat* src, cv::Mat* des, int fWind) const;
  void maxFilter(cv::Mat* src, cv::Mat* des, int fWind) const;

  CClassDatabase classDatabase;

 private:
  CConfig		conf;
  std::vector<CRTree*>	vTrees;
  
};

inline void CRForest::extractFeatureChannels(const cv::Mat* img, cv::vector<cv::Mat*>& vImg) const{
  vImg.clear();
  vImg.resize(32);
  for(int i = 0; i < 32; ++i)
    vImg.at(i) = new cv::Mat(img->rows, img->cols, CV_8UC1);


  //std::cout << img->channels() << std::endl;
  
  cv::cvtColor(*img, *(vImg.at(0)), CV_BGR2GRAY);


  cv::Mat I_x(img->rows, img->cols, CV_16SC1);
  cv::Mat I_y(img->rows, img->cols, CV_16SC1);

   
  cv::Sobel(*(vImg.at(0)), I_x, CV_16S, 1, 0);
  cv::Sobel(*(vImg.at(0)), I_y, CV_16S, 0, 1);

  cv::convertScaleAbs(I_x, *(vImg[3]), 0.25);
  cv::convertScaleAbs(I_y, *(vImg[4]), 0.25);
  
  //std::cout << "vimg[3]" << std::endl;
  
   /* cv::namedWindow("test"); */
   /* cv::imshow("test",*(vImg[3])); */
   /* cv::waitKey(0); */
   /* cv::destroyWindow("test"); */

  // Orientation of gradients
  for(int  y = 0; y < img->rows; y++)
    for(int  x = 0; x < img->cols; x++) {
      // Avoid division by zero
      float tx = (float)I_x.at<short>(y, x) + (float)copysign(0.000001f, I_x.at<short>(y, x));
      // Scaling [-pi/2 pi/2] -> [0 80*pi]
      vImg.at(1)->at<uchar>(y, x) = (uchar)(( atan((float)I_y.at<short>(y, x) / tx) + 3.14159265f / 2.0f ) * 80);
      //std::cout << "scaling" << std::endl;
      vImg.at(2)->at<uchar>(y, x) = (uchar)sqrt((float)I_x.at<short>(y, x)* (float)I_x.at<short>(y, x) + (float)I_y.at<short>(y, x) * (float)I_y.at<short>(y, x));
    }
  
  //std::cout << "kokomade" << std::endl;
  
  // Magunitude of gradients
  for(int y = 0; y < img->rows; y++)
      for(int x = 0; x < img->cols; x++ ) {
	vImg.at(2)->at<uchar>(y, x) = (uchar)sqrt(I_x.at<short>(y, x)*I_x.at<short>(y, x) + I_y.at<short>(y, x) * I_y.at<short>(y, x));
      }

  hog.extractOBin(vImg[1], vImg[2], vImg, 7);

  // calc I_xx I_yy
  cv::Sobel(*(vImg.at(0)), I_x, CV_16S, 2, 0);
  cv::Sobel(*(vImg.at(0)), I_y, CV_16S, 0, 2);

  cv::convertScaleAbs(I_x, *(vImg[5]), 0.25);
  cv::convertScaleAbs(I_y, *(vImg[6]), 0.25);

  cv::Mat img_Lab;
  cv::cvtColor(*img, img_Lab, CV_RGB2Lab);
  cv::vector<cv::Mat> tempVImg(3);

  cv::split(img_Lab, tempVImg);

  for(int i = 0; i < 3; ++i)
    tempVImg.at(i).copyTo(*(vImg.at(i)));

  // min filter
  for(int c = 0; c < 16; ++c)
    minFilter(vImg[c], vImg[c + 16], 5);

  for(int c = 0; c < 16; ++c)
    maxFilter(vImg[c], vImg[c], 5);

  /* cv::namedWindow("test"); */
  /* cv::imshow("test",*img); */
  /* cv::waitKey(0); */
  /* cv::destroyWindow("test"); */
 
  /* std::cout << "extructing feature 2 " << std::endl; */
  
}

inline void CRForest::minFilter(cv::Mat* src, cv::Mat* des, int fWind) const{
  int d = (fWind - 1) / 2;
  cv::Rect roi;
  cv::Mat desTemp(src->rows, src->cols, CV_8U), vTemp;

    for(int y = 0; y < src->rows - fWind; ++y){ //for image height
      if(y < fWind)
	roi = cv::Rect(0, 0, src->cols, fWind - y);
      else
	roi = cv::Rect(0, y, src->cols, fWind);

      cv::reduce((*src)(roi), vTemp, 0, CV_REDUCE_MIN);
      
      roi = cv::Rect(0, y + d, src->cols, 1);
      cv::Mat roiDesTemp(desTemp, roi);
      vTemp.copyTo(roiDesTemp);
    }// For image height

  for(int x = 0; x < src->cols - fWind; ++x){ // for image width
    if(x < d)
      roi = cv::Rect(0, 0, fWind - x, src->rows);
    else
      roi = cv::Rect(x, 0, fWind, src->rows);

    cv::reduce(desTemp(roi), vTemp, 1, CV_REDUCE_MIN);
      
    roi = cv::Rect(x + d, 0, 1, src->rows);
    cv::Mat roiDesTemp((*des), roi);
    vTemp.copyTo((*des)(roi));// = vTemp.clone();//copyTo((*des)(roi));
  } // for image width
}

inline void CRForest::maxFilter(cv::Mat* src, cv::Mat* des, int fWind) const{
  int d = (fWind - 1) / 2;
  cv::Rect roi;
  cv::Mat desTemp(src->rows, src->cols, CV_8U), vTemp;


  /* cv::namedWindow("test"); */
  /* cv::imshow("test",*des); */
  /* cv::waitKey(0); */
  /* cv::destroyWindow("test"); */

    for(int y = 0; y < src->rows - fWind; ++y){ //for image height
      if(y < fWind)
	roi = cv::Rect(0, 0, src->cols, fWind - y);
      else
	roi = cv::Rect(0, y, src->cols, fWind);

      cv::reduce((*src)(roi), vTemp, 0, CV_REDUCE_MAX);
      
      roi = cv::Rect(0, y + d, src->cols, 1);
      cv::Mat roiDesTemp(desTemp, roi);
      vTemp.copyTo(desTemp(roi));
    }// For image height

  for(int x = 0; x < src->cols - fWind; ++x){ // for image width
    if(x < d)
      roi = cv::Rect(0, 0, fWind - x, src->rows);
    else
      roi = cv::Rect(x, 0, fWind, src->rows);

    cv::reduce(desTemp(roi), vTemp, 1, CV_REDUCE_MAX);
      
    roi = cv::Rect(x + d, 0, 1, src->rows);
    cv::Mat roiDesTemp(*des, roi);
    vTemp.copyTo((*des)(roi));// = vTemp.clone();//copyTo((*des)(roi));
  } // for image width

  
}

#endif
