#include "loadConfig.h"

CConfig::CConfig()
{
}


int CConfig::loadConfig(const char* filename)
{
  read_xml(filename, pt);
  
  // load tree path
  if (boost::optional<std::string> str
      = pt.get_optional<std::string>("root.treepath")) {
    std::cout << str.get() << std::endl;
    treepath = *str;
  }
  else {
    std::cout << "root.str is nothing" << std::endl;
  }

  // load number of tree
  if (boost::optional<int> integer
      = pt.get_optional<int>("root.ntree")) {
    std::cout << integer << std::endl;
    ntrees = *integer;
  }
  else {
    std::cout << "root.str is nothing" << std::endl;
  }

  // load patch width
  if (boost::optional<int> integer
      = pt.get_optional<int>("root.pwidth")) {
    std::cout << integer << std::endl;
    p_width = *integer;
  }
  else {
    std::cout << "root.str is nothing" << std::endl;
  }

  // load patch height
  if (boost::optional<int> integer
      = pt.get_optional<int>("root.pheight")) {
    std::cout << integer << std::endl;
    p_height = *integer;
  }
  else {
    std::cout << "root.str is nothing" << std::endl;
  }

  // load test image path
  if (boost::optional<std::string> str
      = pt.get_optional<std::string>("root.imgpath")) {
    std::cout << str.get() << std::endl;
    impath = *str;
  }
  else {
    std::cout << "root.str is nothing" << std::endl;
  }

  // load image name list
  if (boost::optional<std::string> str
      = pt.get_optional<std::string>("root.imgnamelist")) {
    std::cout << str.get() << std::endl;
    imfiles = *str;
  }
  else {
    std::cout << "root.str is nothing" << std::endl;
  }

  // load extruct feature flag
  if (boost::optional<bool> boolean
      = pt.get_optional<bool>("root.efeatures")) {
    std::cout << boolean << std::endl;
    xtrFeature = *boolean;
  }
  else {
    std::cout << "root.str is nothing" << std::endl;
  }

  // load image scales
  scales.resize(0);
  BOOST_FOREACH (const boost::property_tree::ptree::value_type& child,
		 pt.get_child("root.scales")) {
    const float value = boost::lexical_cast<float>(child.second.data());
    scales.push_back(value);
    
    std::cout << value << std::endl;
  }
  for (int i;i < scales.size(); ++i)
    std::cout << i << ": " << scales.at(i) << std::endl;

  // load image ratios
  ratios.resize(0);
  BOOST_FOREACH (const boost::property_tree::ptree::value_type& child, pt.get_child("root.ratio")) {
    const float value = boost::lexical_cast<float>(child.second.data());
    ratios.push_back(value);
    
    std::cout << value << std::endl;
  }
  for (int i;i < ratios.size(); ++i)
    std::cout << i << ": " << ratios.at(i) << std::endl;

  // load output path
  if (boost::optional<std::string> str
      = pt.get_optional<std::string>("root.outpath")) {
    std::cout << str.get() << std::endl;
    outpath = *str;
  }
  else {
    std::cout << "root.str is nothing" << std::endl;
  }

  // load scale factor for output imae
  if (boost::optional<int> integer
      = pt.get_optional<int>("root.sfactor")) {
    std::cout << integer << std::endl;
    out_scale = *integer;
  }
  else {
    std::cout << "root.str is nothing" << std::endl;
  }


  // load training image name list
  if (boost::optional<std::string> str
      = pt.get_optional<std::string>("root.traindataname")) {
    std::cout << str.get() << std::endl;
    traindatafile = *str;
  }
  else {
    std::cout << "root.str is nothing" << std::endl;
  }
  
  // load training image folder
  if (boost::optional<std::string> str
      = pt.get_optional<std::string>("root.trainimgpath")) {
    std::cout << str.get() << std::endl;
    trainpath = *str;
  }
  else {
    std::cout << "root.str is nothing" << std::endl;
  }

  // load scale factor for output imae
  if (boost::optional<int> integer
      = pt.get_optional<int>("root.trainimagepertree")) {
    std::cout << integer << std::endl;
    imagePerTree = *integer;
  }
  else {
    std::cout << "root.str is nothing" << std::endl;
  }
  
  // load scale factor for output imae
  if (boost::optional<int> integer
      = pt.get_optional<int>("root.minsample")) {
    std::cout << integer << std::endl;
    min_sample = *integer;
  }
  else {
    std::cout << "root.str is nothing" << std::endl;
  }

  // load scale factor for output imae
  if (boost::optional<int> integer
      = pt.get_optional<int>("root.maxdepth")) {
    std::cout << integer << std::endl;
    max_depth = *integer;
  }
  else {
    std::cout << "root.str is nothing" << std::endl;
  }
  
  // load scale factor for output imae
  if (boost::optional<int> integer
      = pt.get_optional<int>("root.featurechannel")) {
    std::cout << integer << std::endl;
    featureChannel = *integer;
  }
  else {
    std::cout << "root.str is nothing" << std::endl;
  }

  // load scale factor for output imae
  if (boost::optional<double> variableDouble
      = pt.get_optional<double>("root.patchratio")) {
    std::cout << variableDouble << std::endl;
    patchRatio = *variableDouble;
  }
  else {
    std::cout << "root.str is nothing" << std::endl;
  }

  return 0;
  
}
