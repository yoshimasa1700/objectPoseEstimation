#include "CPatch.h"

void CPatch::extractPatches(std::vector<CDataset> dataSet)
{
  images.resize(0);

  cv::Mat rgb,depth, mask;
  std::vector<cv::Mat> planes;
  std::vector<cv::Mat> allImages;
  //std::cout << dataSet.at(0).depthImageName << std::endl;
  
  for(int i = 0;i < dataSet.size(); ++i){
    // load Mask image
    mask = cv::imread(dataSet.at(0).imageFilePath 
		      + dataSet.at(0).maskImageName, 
		      CV_LOAD_IMAGE_ANYCOLOR);
    
    // load RGB image
    rgb = cv::imread(dataSet.at(0).imageFilePath 
		      + dataSet.at(0).rgbImageName, 
		      CV_LOAD_IMAGE_ANYCOLOR);

    // load Depth image
    depth = cv::imread(dataSet.at(0).imageFilePath 
		      + dataSet.at(0).depthImageName, 
		      CV_LOAD_IMAGE_ANYDEPTH);

    for(int k = 0;k < rgb.cols; ++k)
      for(int l = 0;l < rgb.rows; ++l){
	if(!(bool)mask.at<char>(l, k))
	  depth.at<short>(l, k) = 0;
	for(int j = 0;j < 3; ++j)
	  if(!(bool)mask.at<char>(l, k))
	    rgb.at<cv::Vec3b>(l, k)[j] = 0;
      }
    
    
    allImages.push_back(rgb);
    allImages.push_back(depth);
    images.push_back(allImages);
  }
  
}
