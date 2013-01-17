#include "CRForest.h"

void CRForest::learning(){
  // init dataset and patch vector
  dataSet.resize(conf.ntrees);
  images.resize(conf.ntrees);
  vPatches.resize(conf.ntrees);

  char buffer[256];

  //boost::mt19937    gen( conf.ntrees * static_cast<unsigned long>(time(0)) );

  // grow each tree
  // if you want to fix this program multi thread
  // you should change below
  for(int i=0;i < conf.ntrees; ++i){

    // reserve memory
    dataSet.reserve(conf.imagePerTree + 10);
    images.reserve((conf.imagePerTree + 10) * 3);

    std::cout << "tree number " << i << std::endl;
    // initialize random seed
    //std::cout << "time is " << time(NULL) << std::endl;
    boost::mt19937    gen( i * static_cast<unsigned long>(time(NULL)) );
    
    //load train image list and grand truth
    loadTrainFile(conf, dataSet.at(i), gen);

    for(int p = 0;p < dataSet.at(i).size(); ++p)
      dataSet.at(i).at(p).showDataset();

    //create tree
    vTrees.at(i) = new CRTree(conf.min_sample, conf.max_depth, dataSet.at(i).at(0).centerPoint.size(),gen);
    
    // load images to mamory
    images.at(i).loadImages(dataSet.at(i));

    // reserve memory
    vPatches.at(i).reserve((int)((conf.imagePerTree + 10) * 3 * 
				 (images.at(i).img.at(0).at(0).cols - conf.p_width) * 
				 (images.at(i).img.at(0).at(0).rows - conf.p_height) / conf.stride));


    std::cout << "allocate memory!" << std::endl;

    // extract patch from image
    extractPatches(vPatches.at(i), images.at(i), gen, conf, i);
    std::cout << "patch extracted!" << std::endl;

    // grow tree
    vTrees.at(i)->growTree(vPatches.at(i), 0,0, (float)(vPatches.at(i).at(0).size()) / ((float)(vPatches.at(i).at(0).size()) + (float)(vPatches.at(i).at(1).size())), conf, gen);

    // save tree
    sprintf(buffer, "%s%03d.txt",conf.treepath.c_str(), i + conf.off_tree);
    vTrees.at(i)->saveTree(buffer);

    // release memory
    images.at(i).img.resize(0);
    std::vector<std::vector<cv::Mat> >().swap(images.at(i).img);
    vPatches.at(i).resize(0);
    std::vector<std::vector<CPatch> >().swap(vPatches.at(i));
  }

  dataSet.resize(0);
  images.resize(0);
  vPatches.resize(0);
}

// extract patch from images
void CRForest::extractPatches(std::vector<std::vector<CPatch> > &patches, CImages &image,boost::mt19937 gen, CConfig conf, int treeNum){
  boost::uniform_real<> dst( 0, 1 );
  boost::variate_generator<boost::mt19937&, 
			   boost::uniform_real<> > rand( gen, dst );
  cv::Rect temp;
  CPatch tPatch;

  std::vector<CPatch> posPatch, negPatch;
  
  int pixNum;

  temp.width = conf.p_width;
  temp.height = conf.p_height;

  patches.resize(0);

  posPatch.clear();
  negPatch.clear();

  std::cout << "extracting patch from image" << std::endl;
  std::cout << image.img.at(0).size() << std::endl;
  for(int l = 0;l < image.img.size(); ++l){
    for(int j = 0; j < image.img.at(l).at(0).cols - conf.p_width; j += conf.stride){
      for(int k = 0; k < image.img.at(l).at(0).rows - conf.p_height; k += conf.stride){
	if(rand() < conf.patchRatio){
	  //for(int i = 0;i < image.img.at(l).size(); ++i){// for every channel	  
	    temp.x = j;
	    temp.y = k;

	    pixNum = 0;

	    //std::cout << image.img.at(l).at(1).cols << std::endl;
	    //std::cout << image.img.at(l).at(1).rows << std::endl;
	    for(int m = j; m < j + conf.p_width; ++m){
	      for(int n = k; n < k + conf.p_height; ++n){
		pixNum += (int)(image.img.at(l).at(1).at<uchar>(m, n));
	      }
	    }
	    //std::cout << "pixNum is " << pixNum << std::endl;
	    
	    //std::cout << image.img.at(l).size() << std::endl;

	    tPatch.setPatch(temp, image.img.at(l), dataSet.at(treeNum).at(l).centerPoint);

	    if(pixNum > 0)
	      posPatch.push_back(tPatch);
	    else
	      negPatch.push_back(tPatch);
	    //}
	}
      }
      

    }
    pBar(l,dataSet.at(treeNum).size(), 50);
  }
  patches.push_back(posPatch);
  patches.push_back(negPatch);
  std::cout << std::endl;
}

void CRForest::extractAllPatches(const CDataset &dataSet, const std::vector<cv::Mat> &image, std::vector<CPatch> &patches) const{

  cv::Rect temp;
  CPatch tPatch;

  temp.width = conf.p_width;
  temp.height = conf.p_height;

  patches.clear();
  //std::cout << "extraction patches!" << std::endl;
  for(int j = 0; j < image.at(0).cols - conf.p_width; j += conf.stride){
    for(int k = 0; k < image.at(0).rows - conf.p_height; k += conf.stride){
	temp.x = j;
	temp.y = k;
	
	tPatch.setPatch(temp, image, dataSet.centerPoint);
	patches.push_back(tPatch);
    }
  }
}

void CRForest::loadForest(){
  char buffer[256];
  for(int i = 0; i < vTrees.size(); ++i){
    sprintf(buffer, "%s%03d.txt",conf.treepath.c_str(),i);
    vTrees[i] = new CRTree(buffer);
  }
}

void CRForest::detection(const CDataset &dataSet, const std::vector<cv::Mat> &image, std::vector<cv::Mat> &vDetectImg) const{
  

  std::vector<CPatch> patches;
  std::vector<cv::Mat> scaledImage;
  
  std::vector<const LeafNode*> result;

  int xoffset = conf.p_width / 2;
  int yoffset = conf.p_height / 2;
  
  for(int i = 0; i < conf.scales.size(); ++i){
    scaledImage = convertScale(image, conf.scales.at(i));
    extractAllPatches(dataSet, scaledImage, patches);

    result.clear();

    for(int j = 0; j < patches.size(); ++j){
      this->regression(result, patches.at(j));

      // vote for all trees (leafs) 
      for(std::vector<const LeafNode*>::const_iterator itL = result.begin(); itL!=result.end(); ++itL) {

	// To speed up the voting, one can vote only for patches 
	// with a probability for foreground > 0.5
	// 
	// if((*itL)->pfg>0.5) {

	// voting weight for leaf 
	float w = (*itL)->pfg / float( (*itL)->vCenter.size() * result.size() );

	// vote for all points stored in the leaf
	for(std::vector<std::vector<cv::Point> >::const_iterator it = (*itL)->vCenter.begin(); it!=(*itL)->vCenter.end(); ++it) {

	  for(int c = 0; c < vDetectImg.size(); ++c) {
	    int x = int(xoffset - (*it)[0].x * conf.ratios[c] + 0.5);
	    int y = yoffset-(*it)[0].y;
	    if(y>=0 && y<vDetectImg.at(c).rows && x>=0 && x<vDetectImg[c].cols) {
	      
	      vDetectImg.at(c).at<uchar>(x,y) = w;
	      //*(ptDet[c]+x+y*stepDet) += w;
	    }
	  }
	}

	// } // end if

      }

    } // for every patch

    // smooth result image
    //for(int c=0; c<(int)vDetectImg.size(); ++c)
    //  cvSmooth( vDetectImg[c], vDetectImg[c], CV_GAUSSIAN, 3, 0, 0, 0);

  } // for every scale
}

// Regression 
void CRForest::regression(std::vector<const LeafNode*>& result, CPatch &patch) const{
  result.resize( vTrees.size() );
  for(int i=0; i<(int)vTrees.size(); ++i) {
    result[i] = vTrees[i]->regression(patch);
  }
}
