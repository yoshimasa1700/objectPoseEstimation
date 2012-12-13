#define PATH_SEP "/"

#include <stdexcept>

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include <highgui.h>

//#include "CRFEstimator.h"
#include "CPatch.h"
#include "CRTree.h"

using namespace std;

// Path to trees
string treepath;
// Number of trees
int ntrees;
// Patch width
int p_width;
// Patch height
int p_height;
// Path to images
string impath;
// File with names of images
string imfiles;
// Extract features
bool xtrFeature;
// Scales
vector<float> scales;
// Ratio
vector<float> ratios;
// Output path
string outpath;
// scale factor for output image (default: 128)
int out_scale;
// Path to positive examples
string trainpospath;
// File with postive examples
string trainposfiles;
// Subset of positive images -1: all images
int subsamples_pos;
// Sample patches from pos. examples
unsigned int samples_pos; 
// Path to positive examples
string trainnegpath;
// File with postive examples
string trainnegfiles;
// Subset of neg images -1: all images
int subsamples_neg;
// Samples from pos. examples
unsigned int samples_neg;

// offset for saving tree number
int off_tree;

// load config file for dataset
void loadConfig(const char* filename) {
  char buffer[400];
  ifstream in(filename);
  
  if(in.is_open()) {
    
    // Path to trees
    in.getline(buffer,400);
    in.getline(buffer,400); 
    treepath = buffer;
    // Number of trees
    in.getline(buffer,400); 
    in >> ntrees;
    in.getline(buffer,400); 
    // Patch width
    in.getline(buffer,400); 
    in >> p_width;
    in.getline(buffer,400); 
    // Patch height
    in.getline(buffer,400); 
    in >> p_height;
    in.getline(buffer,400); 
    // Path to images
    in.getline(buffer,400); 
    in.getline(buffer,400); 
    impath = buffer;
    // File with names of images
    in.getline(buffer,400);
    in.getline(buffer,400);
    imfiles = buffer;
    // Extract features
    in.getline(buffer,400);
    in >> xtrFeature;
    in.getline(buffer,400); 
    // Scales
    in.getline(buffer,400);
    int size;
    in >> size;
    scales.resize(size);
    for(int i=0;i<size;++i)
      in >> scales[i];
    in.getline(buffer,400); 
    // Ratio
    in.getline(buffer,400);
    in >> size;
    ratios.resize(size);
    for(int i=0;i<size;++i)
      in >> ratios[i];
    in.getline(buffer,400); 
    // Output path
    in.getline(buffer,400);
    in.getline(buffer,400);
    outpath = buffer;
    // Scale factor for output image (default: 128)
    in.getline(buffer,400);
    in >> out_scale;
    in.getline(buffer,400);
    // Path to positive examples
    in.getline(buffer,400);
    in.getline(buffer,400);
    trainpospath = buffer;
    // File with postive examples
    in.getline(buffer,400);
    in.getline(buffer,400);
    trainposfiles = buffer;
    // Subset of positive images -1: all images
    in.getline(buffer,400);
    in >> subsamples_pos;
    in.getline(buffer,400);
    // Samples from pos. examples
    in.getline(buffer,400);
    in >> samples_pos;
    in.getline(buffer,400);
    // Path to positive examples
    in.getline(buffer,400);
    in.getline(buffer,400);
    trainnegpath = buffer;
    // File with postive examples
    in.getline(buffer,400);
    in.getline(buffer,400);
    trainnegfiles = buffer;
    // Subset of negative images -1: all images
    in.getline(buffer,400);
    in >> subsamples_neg;
    in.getline(buffer,400);
    // Samples from pos. examples
    in.getline(buffer,400);
    in >> samples_neg;
    //in.getline(buffer,400);

  } else {
    cerr << "File not found " << filename << endl;
    exit(-1);
  }
  in.close();

  //output config
  cout << endl << "------------------------------------" << endl << endl;
  cout << "Training:         " << endl;
  cout << "Patches:          " << p_width << " " << p_height << endl;
  cout << "Train pos:        " << trainpospath << endl;
  cout << "                  " << trainposfiles << endl;
  cout << "                  " << subsamples_pos << " " << samples_pos << endl;
  cout << "Train neg:        " << trainnegpath << endl;
  cout << "                  " << trainnegfiles << endl;
  cout << "                  " << subsamples_neg << " " << samples_neg << endl;
  cout << "Trees:            " << ntrees << " " << off_tree << " " << treepath << endl;
  cout << endl << "------------------------------------" << endl << endl;

}

int main(int argc, char* argv[]){
  //check argument
  if(argc < 2) {
    cout << "Usage: CRForest-Detector [config.txt] [tree_offset]" << endl;
    cout << "tree_offset: output number for trees" << endl;
    loadConfig("config.txt");
  } else
    loadConfig(argv[1]);

  if(argc < 3)
    off_tree = 0;
  else
    off_tree = atoi(argv[2]);

  //init random seed

  //create tree directory

  //create Patch class

  for(int i=0;i < ntrees; ++i){
    //extruct patch

    //train a tree

    //save tree

  }
  
  return 0;

}


