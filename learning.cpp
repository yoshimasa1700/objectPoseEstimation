#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

#include <boost/random.hpp>

#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv/cxcore.hpp>

#include "CPatch.h"
#include "CRTree.h"
#include "CDataset.h"

#include "loadConfig.h"
#include "loadTrainFile.h"

using namespace std;

int main(int argc, char* argv[]){

  CConfig		conf;	 // setting
  std::vector<CDataset> dataSet; // training data name list and grand truth

  //read argument
  //check argument
  if(argc < 2) {
    cout << "Usage: ./learning [config.xml]"<< endl;
    conf.loadConfig("config.xml");
  } else
    conf.loadConfig(argv[1]);

  if(argc < 3)
    conf.off_tree = 0;
  else
    conf.off_tree = atoi(argv[2]);
  
  //create tree directory
  string tpath(conf.treepath);
  tpath.erase(tpath.find_last_of(PATH_SEP));
  string execstr = "mkdir ";
  execstr += tpath;
  system( execstr.c_str() );

  // learning
  // most important!!!
  // if you fix this programm multi thread
  // you should fix this forloop!
  for(int i=0;i < conf.ntrees; ++i){
    cout << "tree number " << i << endl;
    // initialize random seed
    boost::minstd_rand    gen( i * (unsigned int)time(NULL) );
    
    //load train image list and grand truth
    loadTrainFile(conf, dataSet, gen);
    
    //train a tree
    
    //save tree
    
  }
  
  return 0; 
}
