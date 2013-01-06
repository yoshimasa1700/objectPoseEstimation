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
  void extractPatches(std::vector<CPatch> &patches, CImages image, boost::mt19937 gen, CConfig conf, int treeNum);
  
 private:
  CConfig		conf;
  std::vector<CRTree*>	vTrees;
  std::vector<std::vector<CDataset> > dataSet;
  // for every tree and positive or negative and every patch
  std::vector<std::vector<std::vector<CPatch> > >  vPatches; 
  std::vector<CImages>	images;
  
};

#endif
