#include "CPatch.h"

// void CPatch::extractPatches(std::vector<CDataset> dataSet, boost::mt19937 gen)
// {
//   boost::uniform_real<> dst( 0, 1 );
//   boost::variate_generator<boost::mt19937&, 
// 			   boost::uniform_real<> > rand( gen, dst );
//   cv::Rect temp;
//   std::vector<cv::Rect> tempPatches;

//   temp.width = width;
//   temp.height = height;
//   vPatches.resize(0);

//   for(int i = 0;i < images.size(); ++i){   
//     tempPatches.resize(0);
//     for(int j = 0; j < images.at(i).at(0).cols - width; j += stride){
//       for(int k = 0; k < images.at(i).at(0).rows - height; k += stride){
// 	if(rand() < patchRatio){
// 	  temp.x = j;
// 	  temp.y k;
// 	  tempPatches.push_back(temp);
// 	}
//       }
//     }
//     vPatches.push_back(tempPatches);
//   }
// }

// void CPatch::loadImages(std::vector<CDataset> dataSet)
// {
//   images.resize(0);

//   cv::Mat rgb,depth, mask;
//   std::vector<cv::Mat> planes;
//   std::vector<cv::Mat> allImages;
//   //std::cout << dataSet.at(0).depthImageName << std::endl;
  
//   for(int i = 0;i < dataSet.size(); ++i){
//     // load Mask image
//     mask = cv::imread(dataSet.at(0).imageFilePath 
// 		      + dataSet.at(0).maskImageName, 
// 		      CV_LOAD_IMAGE_ANYCOLOR);
    
//     // load RGB image
//     rgb = cv::imread(dataSet.at(0).imageFilePath 
// 		      + dataSet.at(0).rgbImageName, 
// 		      CV_LOAD_IMAGE_ANYCOLOR);

//     // load Depth image
//     depth = cv::imread(dataSet.at(0).imageFilePath 
// 		      + dataSet.at(0).depthImageName, 
// 		      CV_LOAD_IMAGE_ANYDEPTH);

//     for(int k = 0;k < rgb.cols; ++k)
//       for(int l = 0;l < rgb.rows; ++l){
// 	if(!(bool)mask.at<char>(l, k))
// 	  depth.at<short>(l, k) = 0;
// 	for(int j = 0;j < 3; ++j)
// 	  if(!(bool)mask.at<char>(l, k))
// 	    rgb.at<cv::Vec3b>(l, k)[j] = 0;
//       }
    
    
//     allImages.push_back(rgb);
//     allImages.push_back(depth);
//     images.push_back(allImages);
//   }
  
// }

