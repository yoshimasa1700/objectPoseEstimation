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
    //std::cout << "kokomade kita" << std::endl;

    int numberOfTrees = vTrees.size();

    for(int i = 0;i < numberOfTrees;++i){
      //std::cout << "releasing tree " << i << std::endl;
      delete vTrees.at(i);
    }
  }
  
  void learning();
  void detection(std::vector<CDataset> dataSet, CImages inputImages, std::vector<cv::Mat> &vDetectImg, float scale, float ratio) const;
  void extractPatches(std::vector<std::vector<CPatch> > &patches, CImages &image, boost::mt19937 gen, CConfig conf, int treeNum);
  void loadForest();
  void extractAllPatches(CDataset dataSet, std::vector<cv::Mat> &image, std::vector<CPatch> &patches) const;
  // Regression 
  void regression(std::vector<const LeafNode*>& result, uchar** ptFCh, int stepImg) const;
  
 private:
  CConfig		conf;
  std::vector<CRTree*>	vTrees;
  std::vector<std::vector<CDataset> > dataSet;
  // for every tree and positive or negative and every patch
  std::vector<std::vector<std::vector<CPatch> > >  vPatches; 
  std::vector<CImages>	images;
  
};

#endif
