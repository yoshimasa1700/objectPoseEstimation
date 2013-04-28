#include <boost/timer.hpp>
#include "CRForest.h"

void CRForest::learning(){
  // grow each tree
  // if you want to fix this program multi thread
  // you should change below
  for(int i = 0;i < conf.ntrees; ++i){
    growATree(i);
  } // end tree loop
}

void CRForest::growATree(const int treeNum){
  std::vector<CDataset> dataSets(0);
  cv::vector<cv::vector<cv::Mat *> > images;
  cv::vector<cv::vector<cv::Mat *> > features;
  std::vector<std::vector<CPatch> > vPatches;
  cv::vector<cv::Mat *> tempFeature;
  
  char buffer[256];

  // reserve memory
  //dataSet.reserve(conf.imagePerTree + 10);
  //images.reserve((conf.imagePerTree + 10) * 3);

  std::cout << "tree number " << treeNum << std::endl;
  // initialize random seed
  //std::cout << "time is " << time(NULL) << std::endl;
  boost::mt19937    gen( treeNum * static_cast<unsigned long>(time(NULL)) );

  boost::timer t;

  loadTrainFile(conf, dataSets);//, gen);
  std::cout << "dataset loaded" << std::endl;

  //for(int k = 0; k < dataSets.size(); ++k)
  //  dataSets.at(k).showDataset();
    
  // initialize class database
  classDatabase.clear();

  // create class database
  for(int p = 0;p < dataSets.size(); ++p)
    classDatabase.add(dataSets.at(p).className);
  classDatabase.show();

  //create tree
  //vTrees.at(treeNum) = new CRTree(conf.min_sample, conf.max_depth, dataSets.at(0).centerPoint.size(),gen);
  
  CRTree *tree = new CRTree(conf.min_sample, conf.max_depth, dataSets.at(0).centerPoint.size(),gen);
  std::cout << "tree created" << std::endl;
    
  
  // load images to mamory
  loadImages(images, dataSets);

  std::cout << dataSets.at(0).centerPoint.size() << std::endl;

  std::cout << "extracting feature" << std::endl;

  features.resize(0);
   
  for(int j = 0; j < images.size(); ++j){
    //std::cout << "extruct " << j << std::endl;
    tempFeature.clear();

    // cv::namedWindow("test");
    // cv::imshow("test",*(images.at(j).at(0)));
    // cv::waitKey(0);
    // cv::destroyWindow("test");

    //extract features
    extractFeatureChannels(images.at(j).at(0), tempFeature);
    // add depth image to features
    tempFeature.push_back(images.at(j).at(1));
    features.push_back(tempFeature);
  }
  
  std::cout << "feature extructed!" << std::endl;

  for(int i = 0; i < images.size(); ++i){
    //std::cout << "releasing image number " << i << std::endl;
    delete images.at(i).at(0);
  }

  //images.clear();
  //std::cout << "images" << images.size() << std::endl;

  // extract patch from image
  //std::cout << "extruction patch from features" << std::endl;
  extractPatches(vPatches, dataSets, features, conf);
  //std::cout << "patch extracted!" << std::endl;

  std::vector<int> patchClassNum(classDatabase.vNode.size(), 0);

  for(int j = 0; j < vPatches.at(0).size(); ++j)
    patchClassNum.at(vPatches.at(0).at(j).classNum)++;

  //for(int c = 0; c < classDatabase.vNode.size(); ++c)
  //std::cout << patchClassNum.at(c) << std::endl;
    

  // grow tree
  //vTrees.at(treeNum)->growTree(vPatches, 0,0, (float)(vPatches.at(0).size()) / ((float)(vPatches.at(0).size()) + (float)(vPatches.at(1).size())), conf, gen, patchClassNum);
  tree->growTree(vPatches, 0,0, (float)(vPatches.at(0).size()) / ((float)(vPatches.at(0).size()) + (float)(vPatches.at(1).size())), conf, gen, patchClassNum);
  
  // save tree
  sprintf(buffer, "%s%03d.txt",
	  conf.treepath.c_str(), treeNum + conf.off_tree);
  std::cout << "tree file name is " << buffer << std::endl;
  //vTrees.at(treeNum)->saveTree(buffer);
  tree->saveTree(buffer);

  // save class database
  sprintf(buffer, "%s%s%03d.txt",
	  conf.treepath.c_str(),
	  conf.classDatabaseName.c_str(), treeNum + conf.off_tree);
  std::cout << "write tree data" << std::endl;
  classDatabase.write(buffer);

  double time = t.elapsed();

  std::cout << "tree " << treeNum << " calicuration time is " << time << std::endl;

  sprintf(buffer, "%s%03d_timeResult.txt",conf.treepath.c_str(), treeNum + conf.off_tree);
  std::fstream lerningResult(buffer, std::ios::out);
  if(lerningResult.fail()){
    std::cout << "can't write result" << std::endl;
  }

  lerningResult << time << std::endl;

  lerningResult.close();

  delete tree;

  // std::vector<CDataset> dataSets(0);
  // cv::vector<cv::vector<cv::Mat> > images;
  // cv::vector<cv::vector<cv::Mat> > features;
  // std::vector<std::vector<CPatch> > vPatches;
  dataSets.clear();

  //vPatches.clear();
  //std::cout << "vPatches" << vPatches.size() << std::endl;

  
  

  for(int i = 0; i < features.size(); ++i){
    if(features.at(i).size() != 0){
      for(int j = 0; j < features.at(i).size(); ++j){
  	//if(features.at(i).size != NULL)
  	//std::cout << "feature keshiteru " << i << " "<< j << std::endl;
  	delete features.at(i).at(j);
      }
    }
  }
  features.clear();
  std::cout << "features" << features.size() << std::endl;

  //delete vTrees.at(treeNum);
}

// extract patch from images
void CRForest::extractPatches(std::vector<std::vector<CPatch> > &patches,const std::vector<CDataset> dataSet,const cv::vector<cv::vector<cv::Mat*> > &image,  CConfig conf){
  

  // boost::uniform_real<> dst( 0, 1 );
  // boost::variate_generator<boost::mt19937&, 
  // 			   boost::uniform_real<> > rand( gen, dst );
  cv::Rect temp;
  CPatch tPatch;

  std::vector<CPatch> tPosPatch, posPatch, negPatch;
  std::vector<std::vector<CPatch> > patchPerClass(classDatabase.vNode.size());
  int pixNum;
  nCk nck;

  int classNum = 0;

  temp.width  = conf.p_width;
  temp.height = conf.p_height;

  patches.resize(0);
  
  tPosPatch.clear();
  posPatch.clear();
  negPatch.clear();

  std::cout << "image num is " << image.size();

  std::cout << "extracting patch from image" << std::endl;
  std::cout << image.at(0).size() << std::endl;
  for(int l = 0;l < image.size(); ++l){
    //tPosPatch.clear();
    for(int j = 0; j < image.at(l).at(0)->cols - conf.p_width; j += conf.stride){
      for(int k = 0; k < image.at(l).at(0)->rows - conf.p_height; k += conf.stride){
	//if(rand() < conf.patchRatio){
	  //for(int i = 0;i < image.img.at(l).size(); ++i){// for every channel	

	//std::cout << "x = " << j << " y = " << k << std::endl;
	  temp.x = j;
	  temp.y = k;

	  pixNum = 0;

	  //std::cout << image.img.at(l).at(1).cols << std::endl;
	  //std::cout << image.img.at(l).at(1).rows << std::endl;

	  // detect negative patch
	  for(int m = j; m < j + conf.p_width; ++m){
	    for(int n = k; n < k + conf.p_height; ++n){
	      pixNum += (int)(image.at(l).at(image.at(l).size() - 1)->at<ushort>(n, m));
	    }
	  }

	  classNum = classDatabase.search(dataSet.at(l).className);
	  
	  

	  //std::cout << classNum << std::endl;

	  if(classNum == -1){
	    std::cout << "class not found!" << std::endl;
	    exit(-1);
	  }

	  // std::cout << "this is for debug" << std::endl;
	  // std::cout << temp << std::endl;
	  // std::cout << dataSet.at(l).centerPoint << std::endl;

	  tPatch.setPatch(temp, image.at(l), dataSet.at(l), classNum);
	  //for(int q = 0; q < tPosPatch.size(); ++q){
	  // cv::namedWindow("test");
	  // cv::imshow("test",(*(tPatch.patch.at(0)))(tPatch.patchRoi));
	  // cv::waitKey(0);
	  // cv::destroyWindow("test");

	  //std::cout << pixNum << std::endl;
	    //}
	  //std::cout << pixNum << std::endl;
	  if (pixNum > 0){
	    //if(pixNum > 500 * conf.p_height * conf.p_width * 0.2)
	      tPosPatch.push_back(tPatch);
	      patchPerClass.at(classNum).push_back(tPatch);
	      // std::cout << "this is for debug patch center point is :" << std::endl;
	      // std::cout << tPatch.center << std::endl;

	      //else
	      //negPatch.push_back(tPatch);
	  } // if
	  //}
	  //}
      }//x
    }//y
    pBar(l,dataSet.size(), 50);
  }//allimages 
    //int totalPatchNum = (int)(((double)(image.at(l).at(0).cols - conf.p_width) / (double)conf.stride) * ((double)(image.at(l).at(0).rows - conf.p_height) / (double)conf.stride));

    //std::cout << "total patch num is " << totalPatchNum << std::endl;
    //std::cout << tPosPatch.size() << std::endl;

    // for(int q = 0; q < tPosPatch.size(); ++q){
    //   cv::namedWindow("test");
    //   cv::imshow("test",(*(tPosPatch.at(q).patch.at(0)))(tPosPatch.at(q).patchRoi));
    //   cv::waitKey(0);
    //   cv::destroyWindow("test");
    // }

  for(int i = 0; i < patchPerClass.size(); ++i){
    if(patchPerClass.at(i).size() > conf.patchRatio){

      std::set<int> chosenPatch = nck.generate(patchPerClass.at(i).size(), conf.patchRatio);//totalPatchNum * conf.patchRatio);
    
      //std::cout << "keisan deketa" << std::endl;

      std::set<int>::iterator ite = chosenPatch.begin();

      //cv::namedWindow("test");
      //cv::imshow("test",image.at(l).at(0));
      //cv::waitKey(0);
      //cv::destroyWindow("test");w

    
      //std::cout << "patch torimasu" << std::endl;

      //std::cout << "tPosPatch num is " << tPosPatch.size() << std::endl;

    
      while(ite != chosenPatch.end()){
	//std::cout << "this is for debug ite is " << tPosPatch.at(*ite).center << std::endl;
	posPatch.push_back(patchPerClass.at(i).at(*ite));
	ite++;
      }

    }else{
      std::cout << "can't extruct enough patch" << std::endl;
    }
  }
    //std::cout << "kokomade kimashita" << std::endl;
    //tPosPatch.clear();
    //
  patches.push_back(posPatch);
  patches.push_back(negPatch);

  std::cout << std::endl;
}



void CRForest::extractAllPatches(const CDataset &dataSet, const cv::vector<cv::Mat*> &image, std::vector<CPatch> &patches) const{

  cv::Rect temp;
  CPatch tPatch;

  temp.width = conf.p_width;
  temp.height = conf.p_height;

  patches.clear();
  //std::cout << "extraction patches!" << std::endl;
  for(int j = 0; j < image.at(0)->cols - conf.p_width; j += conf.stride){
    for(int k = 0; k < image.at(0)->rows - conf.p_height; k += conf.stride){
	temp.x = j;
	temp.y = k;
	
	//std::cout << dataSet.className << std::endl;

	int classNum = classDatabase.search(dataSet.className);

	//std::cout << "class num is " << classNum << std::endl;
	//classDatabase.show();
	if(classNum == -1){
	  std::cout << "This tree not contain this class data" << std::endl;
	  //exit(-1);
	}
	
	tPatch.setPatch(temp, image, dataSet, classNum);

	tPatch.setPosition(j,k);
	patches.push_back(tPatch);
    }
  }
}

void CRForest::loadForest(){
  char buffer[256];
  for(int i = 0; i < vTrees.size(); ++i){
    sprintf(buffer, "%s%03d.txt",conf.treepath.c_str(),i);
    vTrees[i] = new CRTree(buffer);
    sprintf(buffer, "%s%s%03d.txt", conf.treepath.c_str(), conf.classDatabaseName.c_str(), i);
    classDatabase.read(buffer);
  }
}

void CRForest::detection(const CDataset &dataSet, const cv::vector<cv::Mat*> &image, std::vector<double> &detectionResult, int &detectClass) const{




  //contain class number
  int classNum = classDatabase.vNode.size();
  cv::vector<cv::Mat> scaledImage;
  std::vector<CPatch> patches;
  cv::vector<cv::Mat*> features;
  std::vector<const LeafNode*> result;
  std::vector<int> classSum(classNum,0);
  std::vector<double> classification_result(classNum, 0);

  //this is for debug
  std::vector<int> leafPerClass(classNum, 0);
  std::vector<int> patchPerClass(classNum, 0);

  // create output image
  cv::Mat outputImage = cv::Mat::zeros(image.at(0)->rows,image.at(0)->cols,CV_8UC3);

  // set offset of patch
  int xoffset = conf.p_width / 2;
  int yoffset = conf.p_height / 2;
    
  // extract feature from test image
  features.clear();
  extractFeatureChannels(image.at(0), features);
  // add depth image to features
  features.push_back(image.at(1));
  delete image.at(0);

  // extract patches from features
  extractAllPatches(dataSet, features, patches);

  std::cout << "patch num: " << patches.size() << std::endl;

  // regression for every patch
  for(int j = 0; j < patches.size(); ++j){
    result.clear();

    int maxClass = 0;
    this->regression(result, patches.at(j));
    //std::cout << "kokomade" << std::endl;
    // vote for all trees (leafs) 
    for(std::vector<const LeafNode*>::const_iterator itL = result.begin(); itL!=result.end(); ++itL) {

      //for(int k = 0; k < classDatabase.vNode.size(); ++k)
      //classSum.at(k) = 0;

      // To speed up the voting, one can vote only for patches 
      // with a probability for foreground > 0.5
      // 
      // if((*itL)->pfg>0.5) {

      int classNum = (*itL)->pfg.size();

      int containPoints = 0;

      for(int i = 0; i < classNum; ++i)
	containPoints += (*itL)->vCenter.at(i).size();



      int maxClassNum = 0;

      for(int q = 0; q < classNum; ++q){
	if(maxClassNum < (*itL)->vCenter.at(q).size()){
	  maxClassNum = (*itL)->vCenter.at(q).size();
	  maxClass = q;
	}
      }
      
      for(int l = 0; l < classNum; ++l){
	  // voting weight for leaf 
	float w = (*itL)->pfg.at(l);// * (*itL)->vCenter.at(maxClass).size();// / (float)((float)containPoints * result.size() );

	  
	  //for(int c = 0; c < classNum; ++c){
	classification_result.at(maxClass) += (double) w; //* ((double)classSum.at(c) / (double)(*itL)->vClass.size());
      }


//      for(int u = 0;u < 1;++u)//(*itL)->vCenter.size();u++)
//        {
//          for(int v = 0; v < (*itL)->vCenter.at(u).size();v++){
//              int posy = patches.at(j).position.y + (*itL)->vCenter.at(u).at(v).y;
//              int posx = patches.at(j).position.x + (*itL)->vCenter.at(u).at(v).x;

//              //std::cout << "posy = " << posy << ", posx = " << posx << std::endl;

//              if(posy > 0 && posx > 0 &&
//                 posy <= image.at(1)->rows && posx <= image.at(1)->cols){

//                  cv::Vec<ushort,3> &p = outputImage.at<cv::Vec<ushort,3> >(posy,posx);
//                  p[1] = 200;
//                  p[2] = 200;
//                }
//            }
//        }

    } // for every leaf

    patches.at(j).detectedClass = maxClass;
    //cv::circle(outputImage,patches.at(j).position,5,cv::Scalar(0,0,200));

  } // for every patch



  std::cout << "detection result outputed" << std::endl;

  //cv::imwrite(dataSet.imageFilePath + "_result.png",outputImage);

  std::cout << dataSet.className << std::endl;
  std::cout << "result" << std::endl;
  for(int i = 0; i < classNum; ++i){
   std::cout << classDatabase.vNode.at(i).name << " : " << classification_result.at(i) << std::endl;
  }
  std::cout << std::endl;

  int maxResult = 0;
  double maxResultTemp = 0;

  for(int i = 0; i < classification_result.size(); ++i){
    if(classification_result.at(i) > maxResultTemp){
      maxResult = i;
      maxResultTemp = classification_result.at(i);
    }
  }

  detectClass = maxResult;

  for(int pN = 0; pN < patches.size(); ++pN){
      if(patches.at(pN).detectedClass == detectClass){
      cv::Vec<ushort,3> &p = outputImage.at<cv::Vec<ushort,3> >(patches.at(pN).position.y + yoffset, patches.at(pN).position.x + xoffset);
      p[0] = 200;
        }
    }


  cv::namedWindow("test");
  cv::imshow("test",outputImage);
  cv::waitKey(0);
  cv::destroyWindow("test");
  
  //for(int i = 0; i < classNum; ++i){
  if(dataSet.className == classDatabase.vNode.at(maxResult).name){
      detectionResult.at(0) += 1;// += classification_result.at(i);
      std::cout << "correct !" << std::endl;
  }else{
    std::cout << "hazure !" << std::endl;
    std::cout << classDatabase.vNode.at(maxResult).name << std::endl;
    //}
    detectionResult.at(1) += 1;
  }
  // this is for debug
  // for(int c = 0; c < classSum.size(); ++c){
  //   std::cout << "class1: ";
  //   std::cout << leafPerClass.at(c) << " ";
  //   std::cout << (float)classification_result.at(c) /  (float)patchPerClass.at(c) << std::endl;
  // }

  // std::vector<CPatch> patches;
  // cv::vector<cv::Mat> features;




  for(int i = 0; i < features.size(); ++i){
      //astd::cout << i << std::endl;
      delete features.at(i);
    }

  std::cout << "detection end!" << std::endl;
  
  //patches.clear();
  features.clear();


  std::cout << "detection end!" << std::endl;
}

// Regression 
void CRForest::regression(std::vector<const LeafNode*>& result, CPatch &patch) const{
  result.resize( vTrees.size() );
  //std::cout << "enter regression" << std::endl;
  for(int i=0; i<(int)vTrees.size(); ++i) {
    result[i] = vTrees[i]->regression(patch);
  }
}

void CRForest::loadImages(cv::vector<cv::vector<cv::Mat *> > &img, std::vector<CDataset> &dataSet){
  img.resize(0);

  cv::Mat* rgb,*depth, *mask;
  cv::vector<cv::Mat*> planes;
  cv::vector<cv::Mat*> allImages;
  //cv::vector<cv::Mat> rgbSplited;
  
  for(int i = 0;i < dataSet.size(); ++i){
    rgb = new cv::Mat();
    depth = new cv::Mat();
    mask = new cv::Mat();
  
    // load Mask image
    
    *mask = cv::imread(dataSet.at(i).imageFilePath
		       + dataSet.at(i).maskImageName,3).clone();

    // load RGB image
    *rgb = cv::imread(dataSet.at(i).imageFilePath
		      + dataSet.at(i).rgbImageName,3).clone();

    //std::cout << dataSet.at(i).rgbImageName << " " << rgb->channels() << std::endl;
    // load Depth image
    *depth = cv::imread(dataSet.at(i).imageFilePath
		       + dataSet.at(i).depthImageName,
			CV_LOAD_IMAGE_ANYDEPTH).clone();
    cv::Point tempPoint;
    tempPoint.x = (*rgb).cols / 2;
    tempPoint.y = (*rgb).rows / 2;

    dataSet.at(i).centerPoint.push_back(tempPoint);
    
    //cv::namedWindow("test");
    //cv::imshow("test",*rgb);
    //cv::waitKey(0);
    //cv::destroyWindow("test");



    //std::cout << depth << std::endl;
    


    for(int k = 0;k < rgb->cols; ++k)
      for(int l = 0;l < rgb->rows; ++l){
    	//std::cout << depth.at<ushort>(l, k) << " " << std::endl;
    	//if(!(bool)mask->at<char>(l, k))
    	  //depth->at<ushort>(l, k) = 0;
    	// for(int j = 0;j < 3; ++j)
    	//   if(!(bool)mask.at<char>(l, k))
    	//     rgb.at<cv::Vec3b>(l, k)[j] = 0;
      }
    //rgbSplited.resize(rgb.channels());
    
    //cv::split(rgb, rgbSplited);
    
    allImages.clear();
    allImages.push_back(rgb);
    allImages.push_back(depth);
    img.push_back(allImages);

    delete mask;
  }

    
}

