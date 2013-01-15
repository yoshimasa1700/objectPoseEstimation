#include "CRForest.h"
//#include "CRTree.h"

#include "util.h"

using namespace std;

void detect(const CRForest &forest, CConfig conf){
  
  vector<CDataset> dataSet;
  boost::mt19937 gen( conf.ntrees * static_cast<unsigned long>(time(0)) );
  CImages images;
  vector<CImages> scaledImages;
  
  vector<vector<vector<cv::Mat> > > vDetectedImg;
  
  conf.imagePerTree = 10;//読み込む画像ファイルの大まかな数

  loadTrainFile(conf, dataSet, gen);
  for(int i = 0; i < dataSet.size(); ++i)
    dataSet.at(i).showDataset();

  images.loadImages(dataSet);

  vDetectedImg.resize(images.img.size());

  for(int i = 0; i < images.img.size(); ++i){
    vDetectedImg.at(i).resize(conf.scales.size());
    for(int j = 0; j < conf.scales.size(); ++j){
      vDetectedImg.at(i).at(j).resize(conf.ratios.size());
      for(int k = 0; k < conf.ratios.size(); ++k){
	vDetectedImg.at(i).at(j).at(k).zeros(int(images.img.at(i).at(0).rows * conf.scales.at(j)),
					     int (images.img.at(i).at(0).cols * conf.scales.at(j)),
					     images.img.at(i).at(0).type());
      }
    }
    
  }



  scaledImages.clear();
  for(int i = 0; i < conf.scales.size(); ++i){
    scaledImages.push_back(convertScale(images, conf.scales.at(i)));
  }
  //cout << "kokomade kitayo" << endl;
  for(int i = 0; i < conf.scales.size(); ++i){
    for(int j = 0; j < conf.ratios.size(); ++j){
      forest.detection(dataSet, scaledImages.at(i), vDetectedImg.at(i).at(j),conf.scales.at(i), conf.ratios.at(j));
    }
  }
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
