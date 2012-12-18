#ifndef __CConfig__
#define __CConfig__

#include <iostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;


class CConfig
{
 public:
  CConfig();

  int loadConfig(const char* filename);
  
  // Path to trees
  string		treepath;
  // Number of trees
  int		ntrees;
  // Patch width
  int		p_width;
  // Patch height
  int		p_height;
  // Path to images
  string		impath;
  // File with names of images
  string		imfiles;
  // Extract features
  bool		xtrFeature;
  // Scales
  vector<float>	scales;
  // Ratio
  vector<float>	ratios;
  // Output path
  string		outpath;
  // scale factor for output image (default: 128)
  int		out_scale;
  // Path to training image folder 12/12/14 MasahikoToyoshi
  string trainpath;
  // training data file name
  string traindatafile;
  // Path to positive examples
  string		trainpospath;
  // File with postive examples
  string		trainposfiles;
  // Subset of positive images -1: all images
  int		subsamples_pos;
  // Sample patches from pos. examples
  unsigned int	samples_pos;
  // Path to positive examples
  string		trainnegpath;
  // File with postive examples
  string		trainnegfiles;
  // Subset of neg images -1: all images
  int		subsamples_neg;
  // Samples from pos. examples
  unsigned int	samples_neg;

  // offset for saving tree number
  int	off_tree;

  // train image per tree
  int imagePerTree;

 private:
  boost::property_tree::ptree pt;

};

#endif
