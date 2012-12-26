#ifndef __CRFOREST__
#define __CRFOREST__

#include "CRTree.h"
#include "CPatch.h"
#include "util.h"

class CRForest {
 public:
  CRForest(CConfig config){
    conf = config;
    vTrees.resize(conf.ntrees);
  }
  ~CRForest() {
    for(int i = 0;i < conf.ntrees;i++)
      delete vTrees.at(i);
  }
  
  void learning();
  //void extractPatches(boost::mt19937 gen);
  
 private:
  CConfig conf;
  std::vector<CRTree*> vTrees;
  std::vector<std::vector<CDataset> > dataSet;
  std::vector<CPatch> patches;
  std::vector<CImages> images;
  
};

inline void CRForest::learning(){
  // init dataset and patch vector
  dataSet.resize(conf.ntrees);
  patches.resize(conf.ntrees, CPatch(conf.p_width, 
				     conf.p_height, 
				     conf.stride, 
				     conf.patchRatio,
				     1000));

  // grow each tree
  // if you want to fix this program multi thread
  // you should change below
  


  for(int i=0;i < conf.ntrees; ++i){
    std::cout << "tree number " << i << std::endl;
    // initialize random seed
    boost::mt19937    gen( i * static_cast<unsigned long>(time(0)) );
    
    //load train image list and grand truth
    //loadTrainFile(conf, dataSet.at(i), gen);

    // load images to mamory
    //images.at(i).loadImages(dataSet.at(i));
  }
}
//void CRForest::extractPatches(boost::mt19937 gen){
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
  // 	  temp.y = k;
  // 	  tempPatches.push_back(temp);
  // 	}
  //       }
  //     }
  //     vPatches.push_back(tempPatches);
  //   }
  // }
//}



#endif
