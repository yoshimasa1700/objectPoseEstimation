#include "util.h"

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
  
  // load scale factor for output imae
  if (boost::optional<int> integer
      = pt.get_optional<int>("root.stride")) {
    std::cout << integer << std::endl;
    stride = *integer;
  }
  else {
    std::cout << "root.str is nothing" << std::endl;
  }

  // load offset of tree name
  if (boost::optional<int> integer
      = pt.get_optional<int>("root.offTree")) {
    std::cout << integer << std::endl;
    off_tree = *integer;
  }
  else {
    std::cout << "root.str is nothing" << std::endl;
  }

  // load test data folder
  if (boost::optional<std::string> str
      = pt.get_optional<std::string>("root.testpath")) {
    std::cout << str.get() << std::endl;
    testPath = *str;
  }
  else {
    std::cout << "root.str is nothing" << std::endl;
  }


  return 0; 
}

void loadTrainFile(CConfig conf, std::vector<CDataset> &dataSet, boost::mt19937 gen)
{

  std::string traindatafilepath = conf.trainpath + PATH_SEP +  conf.traindatafile;
  int n_folders;
  int n_files;
  std::vector<std::string> trainimagefolder;
  CDataset temp;
  std::vector<CDataset> tempDataSet;
  std::string trainDataListPath;
  int dataSetNum;

  cv::Point tempPoint;

  boost::uniform_real<> dst( 0, 1 );
  boost::variate_generator<boost::mt19937&, 
			   boost::uniform_real<> > rand( gen, dst );
  
  //read train data folder list
  //cout << "train data folder list path: " << traindatafilepath << std::endl;
  std::ifstream in(traindatafilepath.c_str());
  if(!in.is_open()){
    std::cout << "train data floder list is not found!" << std::endl;
    exit(1);
  }
  in >> n_folders;
  //std::cout << "number of training data folder: "<< n_folders << std::endl;
  trainimagefolder.resize(n_folders);
  for(int i = 0;i < n_folders; ++i)
    in >> trainimagefolder.at(i);
  in.close();
  //std::cout << "training folder list: " << std::endl;
  //for(int i = 0;i < n_folders;  ++i)
  //  std::cout << "\t"  <<trainimagefolder.at(i) << std::endl;

  //read train file name and grand truth from file
  tempDataSet.resize(0);
  for(int i = 0;i < n_folders; ++i){
    trainDataListPath 
      = conf.trainpath + PATH_SEP + trainimagefolder.at(i) 
      + PATH_SEP + "dataList.txt";
    temp.imageFilePath 
      = conf.trainpath + PATH_SEP + trainimagefolder.at(i) + PATH_SEP;
      //std::cout << trainDataListPath << std::endl;
    std::ifstream trainDataList(trainDataListPath.c_str());
    trainDataList >> n_files;
      //std::cout << "number of file: " << n_files << std::endl;
    
    
    for(int j = 0;j < n_files; ++j){
      //read file names
      trainDataList >> temp.rgbImageName;
      trainDataList >> temp.depthImageName;
      trainDataList >> temp.maskImageName;

      //read bounding box
      trainDataList >> temp.bBox.x;
      trainDataList >> temp.bBox.y;
      trainDataList >> temp.bBox.width;
      trainDataList >> temp.bBox.height;
      
      temp.centerPoint.resize(0);

      //read center point
      trainDataList >> tempPoint.x;//temp.centerPoint.x;
      trainDataList >> tempPoint.y;

      temp.centerPoint.push_back(tempPoint);
      
      //read class name
      trainDataList >> temp.className;

      //read angle grand truth
      trainDataList >> temp.angle;

      //show read data *for debug
      //temp.showDataset();
     
      tempDataSet.push_back(temp);
    }
    trainDataList.close();
  }
  dataSetNum = tempDataSet.size();
   for(int j = 0;j < dataSetNum; ++j)
      if(rand() * dataSetNum < conf.imagePerTree)
	dataSet.push_back(tempDataSet.at(j));
    std::cout << "train data number: " << dataSet.size() << std::endl;
}

void pBar(int p,int maxNum, int width){
  int i;
  std::cout << "[";// << std::flush;
  for(i = 0;i < (int)((double)(p + 1)/(double)maxNum*(double)width);++i)
    std::cout << "*";

  for(int j = 0;j < width - i;++j)
    std::cout << " ";

  std::cout << "]" << (int)((double)(p + 1)/(double)maxNum*100) << "%"  << "\r" << std::flush;
}
