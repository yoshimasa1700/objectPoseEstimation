#define PATH_SEP "/"

#include <stdexcept>

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include <highgui.h>

#include "CPatch.h"
#include "CRTree.h"

#include "loadConfig.h"

using namespace std;

// load config file for dataset
// void loadConfig(const char* filename) {
//   char buffer[400];
//   ifstream in(filename);
  
//   if(in.is_open()) {
    
//     // Path to trees
//     in.getline(buffer,400);
//     in.getline(buffer,400); 
//     treepath = buffer;
//     // Number of trees
//     in.getline(buffer,400); 
//     in >> ntrees;
//     in.getline(buffer,400); 
//     // Patch width
//     in.getline(buffer,400); 
//     in >> p_width;
//     in.getline(buffer,400); 
//     // Patch height
//     in.getline(buffer,400); 
//     in >> p_height;
//     in.getline(buffer,400); 
//     // Path to images
//     in.getline(buffer,400); 
//     in.getline(buffer,400); 
//     impath = buffer;
//     // File with names of images
//     in.getline(buffer,400);
//     in.getline(buffer,400);
//     imfiles = buffer;
//     // Extract features
//     in.getline(buffer,400);
//     in >> xtrFeature;
//     in.getline(buffer,400); 
//     // Scales
//     in.getline(buffer,400);
//     int size;
//     in >> size;
//     scales.resize(size);
//     for(int i=0;i<size;++i)
//       in >> scales[i];
//     in.getline(buffer,400); 
//     // Ratio
//     in.getline(buffer,400);
//     in >> size;
//     ratios.resize(size);
//     for(int i=0;i<size;++i)
//       in >> ratios[i];
//     in.getline(buffer,400); 
//     // Output path
//     in.getline(buffer,400);
//     in.getline(buffer,400);
//     outpath = buffer;
//     // Scale factor for output image (default: 128)
//     in.getline(buffer,400);
//     in >> out_scale;
//     in.getline(buffer,400);
//     // Path to training data 
//     //in.getline(buffer, 400);
//     in.getline(buffer, 400);
//     in >> trainpath;
//     // Train data file name
//     in.getline(buffer, 400);
//     in.getline(buffer, 400);
//     in >>  traindatafile;
//     in.getline(buffer, 400);
    
//     // Path to positive examples
//     in.getline(buffer,400);
//     in.getline(buffer,400);
//     trainpospath = buffer;
//     // File with postive examples
//     in.getline(buffer,400);
//     in.getline(buffer,400);
//     trainposfiles = buffer;
//     // Subset of positive images -1: all images
//     in.getline(buffer,400);
//     in >> subsamples_pos;
//     in.getline(buffer,400);
//     // Samples from pos. examples
//     in.getline(buffer,400);
//     in >> samples_pos;
//     in.getline(buffer,400);
//     // Path to positive examples
//     in.getline(buffer,400);
//     in.getline(buffer,400);
//     trainnegpath = buffer;
//     // File with postive examples
//     in.getline(buffer,400);
//     in.getline(buffer,400);
//     trainnegfiles = buffer;
//     // Subset of negative images -1: all images
//     in.getline(buffer,400);
//     in >> subsamples_neg;
//     in.getline(buffer,400);
//     // Samples from pos. examples
//     in.getline(buffer,400);
//     in >> samples_neg;
//     //in.getline(buffer,400);
//   } else {
//     cerr << "File not found " << filename << endl;
//     exit(-1);
//   }
//   in.close();

//   //output config
//   cout << endl << "------------------------------------" << endl << endl;
//   cout << "Training:         " << endl;
//   cout << "Patches:          " << p_width << " " << p_height << endl;
//   cout << "Train pos:        " << trainpospath << endl;
//   cout << "                  " << trainposfiles << endl;
//   cout << "                  " << subsamples_pos << " " << samples_pos << endl;
//   cout << "Train neg:        " << trainnegpath << endl;
//   cout << "                  " << trainnegfiles << endl;
//   cout << "                  " << subsamples_neg << " " << samples_neg << endl;
//   cout << "Trees:            " << ntrees << " " << off_tree << " " << treepath << endl;
//   cout << endl << "------------------------------------" << endl << endl;

// }


void loadTrainFile(std::vector<string>& vFilenames, std::vector<cv::Rect>& vBBox, std::vector<std::vector<cv::Point> >& vCenter )
{
  //ifstream in()
}


int main(int argc, char* argv[]){

  CConfig conf;
  
  //check argument
  if(argc < 2) {
    cout << "Usage: ./learning [config.xml] [tree_offset]" << endl;
    cout << "tree_offset: output number for trees" << endl;
    conf.loadConfig("config.xml");
  } else
    conf.loadConfig(argv[1]);

  if(argc < 3)
    conf.off_tree = 0;
  else
    conf.off_tree = atoi(argv[2]);

  //init random seed
  cv::RNG gen(cv::getTickCount());
  
  //create tree directory
  string tpath(conf.treepath);
  tpath.erase(tpath.find_last_of(PATH_SEP));
  string execstr = "mkdir ";
  execstr += tpath;
  system( execstr.c_str() );

  //create Patch class
  //CPatch Train(&gen, p_width, p_height, 2);
  
  cout << conf.trainpath << endl;
  cout << conf.traindatafile << endl;
  

  for(int i=0;i < conf.ntrees; ++i){
    //extruct patch
    
    //train a tree
    
    //save tree
    
  }
  
  return 0; 
}
