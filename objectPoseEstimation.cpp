#include "CRForest.h"
#include <opencv2/opencv.hpp>
//#include "CRTree.h"

#include "util.h"

using namespace std;

void loadImage(const CDataset &dataSet, std::vector<cv::Mat> &image){
  cv::Mat rgb,depth, mask;
  
  //std::vector<cv::Mat> rgbSplited;

  mask = cv::imread(dataSet.imageFilePath
		    + dataSet.maskImageName,
		    CV_LOAD_IMAGE_ANYCOLOR);
  
  // load RGB image
  rgb = cv::imread(dataSet.imageFilePath
		   + dataSet.rgbImageName,
		   CV_LOAD_IMAGE_ANYCOLOR);

  // load Depth image
  depth = cv::imread(dataSet.imageFilePath
		     + dataSet.depthImageName,
		     CV_LOAD_IMAGE_ANYDEPTH);

  // masking image
  for(int k = 0;k < rgb.cols; ++k)
    for(int l = 0;l < rgb.rows; ++l){
      if(!(bool)mask.at<char>(l, k))
	depth.at<short>(l, k) = 0;
      for(int j = 0;j < 3; ++j)
	if(!(bool)mask.at<char>(l, k))
	  rgb.at<cv::Vec3b>(l, k)[j] = 0;
    }

  //rgbSplited.resize(rgb.channels());
  
  //cv::split(rgb, rgbSplited);

  //for(int i = 0; i < rgbSplited.size(); ++i)
  image.push_back(rgb);//rgbSplited.at(i));
  image.push_back(depth);
}

void loadTestFile(CConfig conf, std::vector<CDataset> &dataSet){
  std::string testfilepath = conf.testPath + PATH_SEP +  conf.testData;
  int n_folders;
  int n_files;
  std::vector<std::string> testimagefolder;
  CDataset temp;
  std::vector<CDataset> tempDataSet;
  std::string testDataListPath;
  int dataSetNum;

  cv::Point tempPoint;

  std::ifstream in(testfilepath.c_str());
  if(!in.is_open()){
    std::cout << "test data floder list is not found!" << std::endl;
    exit(1);
  }
  in >> n_folders;

  testimagefolder.resize(n_folders);
  for(int i = 0;i < n_folders; ++i)
    in >> testimagefolder.at(i);
  in.close();

  //read train file name and grand truth from file
  tempDataSet.resize(0);
  for(int i = 0;i < n_folders; ++i){
    testDataListPath 
      = conf.testPath + PATH_SEP + testimagefolder.at(i) 
      + PATH_SEP + "testDataList.txt";
    temp.imageFilePath 
      = conf.testPath + PATH_SEP + testimagefolder.at(i) + PATH_SEP;
      //std::cout << trainDataListPath << std::endl;
    std::ifstream testDataList(testDataListPath.c_str());
    if(testDataList.is_open()){
      testDataList >> n_files;
      //std::cout << "number of file: " << n_files << std::endl;
      for(int j = 0;j < n_files; ++j){
	//read file names
	testDataList >> temp.rgbImageName;
	testDataList >> temp.depthImageName;
	testDataList >> temp.maskImageName;

	//read bounding box
	testDataList >> temp.bBox.x;
	testDataList >> temp.bBox.y;
	testDataList >> temp.bBox.width;
	testDataList >> temp.bBox.height;
      
	temp.centerPoint.resize(0);

	//read center point
	testDataList >> tempPoint.x;//temp.centerPoint.x;
	testDataList >> tempPoint.y;

	temp.centerPoint.push_back(tempPoint);
      
	//read class name
	testDataList >> temp.className;

	//read angle grand truth
	testDataList >> temp.angle;

	//show read data *for debug
	//temp.showDataset();
     
	tempDataSet.push_back(temp);
      }
      testDataList.close();
    }
  }
  dataSetNum = tempDataSet.size();
   for(int j = 0;j < dataSetNum; ++j)
     dataSet.push_back(tempDataSet.at(j));
    std::cout << "test data number: " << dataSet.size() << std::endl;
}

void detect(const CRForest &forest, CConfig conf){
  std::vector<CDataset> dataSet;
  vector<cv::Mat> image;
  //vector<CImages> scaledImages;
  //vector<vector<cv::Mat> >  vDetectedImg(conf.scales.size());
  char buffer[256];

  std::vector<double> detectionResult;
  
  //conf.imagePerTree = 10;//読み込む画像ファイルの大まかな数


  int classNum = forest.classDatabase.vNode.size();

  detectionResult.resize(classNum);
  
  for(int i = 0; i < classNum; ++i)
    detectionResult.at(i) = 0.0;
  
  loadTestFile(conf, dataSet);
  for(int i = 0; i < dataSet.size(); ++i)
   dataSet.at(i).showDataset();

  //loadImage(dataSet.at(0), image);
  for(int m = 0; m < dataSet.size(); ++m){
    image.clear();               
    loadImage(dataSet.at(m), image);
    
    //for(int i = 0; i < conf.scales.size(); ++i){
    forest.detection(dataSet.at(m), image, detectionResult);
      //}
  }
  
  //std::cout << dataSet.className << std::endl;
  //std::cout << "detection result" << std::endl;
  // for(int i = 0; i < classNum; ++i){
  //  std::cout << forest.classDatabase.vNode.at(i).name << " : " << detectionResult.at(i) << std::endl;
  // }
  // std::cout << std::endl;

  std::cout << "detection result:" << (detectionResult.at(0) / dataSet.size()) * 100 << "%" << std::endl;
}

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

  // create random forest class
  CRForest forest(conf);

  forest.loadForest();
  
  //create tree directory
  string opath(conf.outpath);
  opath.erase(opath.find_last_of(PATH_SEP));
  string execstr = "mkdir ";
  execstr += opath;
  system( execstr.c_str() );

  // learning
  //forest.learning();
  detect(forest, conf);
  
  return 0; 
}
