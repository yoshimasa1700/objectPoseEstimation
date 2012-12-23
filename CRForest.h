#ifndef __CRFOREST__
#define __CRFOREST__

#include <vector>
#include "loadConfig.h"
#include "CPatch.h"
#include "CRTree.h"
#include "CDataset.h"

#include <boost/random.hpp>
#include "loadTrainFile.h"

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
  
 private:
  CConfig conf;
  std::vector<CRTree*> vTrees;
  std::vector<std::vector<CDataset> > dataSet;
  std::vector<CPatch> vPatches;
  
};

void CRForest::learning(){
  // init dataset and patch vector
  dataSet.resize(conf.ntrees);
  vPatches.resize(conf.ntrees, CPatch(conf.p_width, conf.p_height, 1000));

  // grow each tree
  // if you want to fix this program multi thread
  // you should change below
  


  for(int i=0;i < conf.ntrees; ++i){
    cout << "tree number " << i << endl;
    // initialize random seed
    boost::mt19937    gen( i * static_cast<unsigned long>(time(0)) );
    
    //load train image list and grand truth
    loadTrainFile(conf, dataSet.at(i), gen);

    // extract patch from images
    vPatches.at(i).extractPatches(dataSet.at(i));

    //create a tree
    vTrees.at(i) = new CRTree(conf.min_sample, 
    			      conf.max_depth,
    			      dataSet.at(i).at(0).centerPoint.size());

    //grow a tree
    //vTrees.at(treeNum).grow(conf, dataSet.at(i));
  }
}

#endif
