#include "CDataset.h"

CDataset::CDataset(){
}

void CDataset::showDataset(){
  std::cout << "RGB image name:" << rgbImageName << std::endl;
  std::cout << "Depth image name:" << depthImageName << std::endl;
  std::cout << "mask image name:" << maskImageName << std::endl;
  
  std::cout << "bouding box: " << bBox << std::endl;
  std::cout << "center point: " << centerPoint << std::endl;

  std::cout << "class: " << className << std::endl;

  std::cout << "angle grand truth:" << angle << std::endl; 
}
