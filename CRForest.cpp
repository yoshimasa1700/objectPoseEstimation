#include "CRForest.h"

void CRForest::learning(){
  // init dataset and patch vector
  dataSet.resize(conf.ntrees);
  images.resize(conf.ntrees);
  vPatches.resize(conf.ntrees);

  // grow each tree
  // if you want to fix this program multi thread
  // you should change below
  for(int i=0;i < conf.ntrees; ++i){

    // reserve memory
    dataSet.reserve(conf.imagePerTree + 10);
    images.reserve((conf.imagePerTree + 10) * 3);

    std::cout << "tree number " << i << std::endl;
    // initialize random seed
    boost::mt19937    gen( i * static_cast<unsigned long>(time(0)) );
    
    //load train image list and grand truth
    loadTrainFile(conf, dataSet.at(i), gen);
    
    // load images to mamory
    images.at(i).loadImages(dataSet.at(i));

    vPatches.reserve((int)((conf.imagePerTree + 10) 
		     * 3 
			   * (images.at(i).img.at(0).at(0).cols - conf.p_width)
			   * (images.at(i).img.at(0).at(0).rows - conf.p_height)
		     / conf.stride)
		     );

    // extract patch from image
    extractPatches(vPatches.at(i), images.at(i), gen, conf, i);

    // release memory
    images.at(i).img.resize(0);
    std::vector<std::vector<cv::Mat> >().swap(images.at(i).img);
    vPatches.at(i).resize(0);
    std::vector<CPatch>().swap(vPatches.at(i));
  }

  dataSet.resize(0);
  images.resize(0);
  vPatches.resize(0);
}

void CRForest::extractPatches(std::vector<std::vector<CPatch> > &patches, CImages image,boost::mt19937 gen, CConfig conf, int treeNum){
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
  for(int l = 0;l < image.img.size(); ++l){
    for(int j = 0; j < image.img.at(l).at(0).cols - conf.p_width; j += conf.stride){
      for(int k = 0; k < image.img.at(l).at(0).rows - conf.p_height; k += conf.stride){
	if(rand() < conf.patchRatio){
	  //for(int i = 0;i < image.img.at(l).size(); ++i){// for every channel	  
	    temp.x = j;
	    temp.y = k;

	    pixNum = 0;

	    for(int m; m < conf.p_width; ++m){
	      for(int n; n < conf.p_height; ++n){
		pixNum += image.img.at(l).at(1).<char>(j, k);
	      }
	    }	    
	    
	    tPatch.setPatch(temp,&image.img.at(l), dataSet.at(treeNum).at(l).centerPoint);

	    if(pixNum > 0)
	      posPatch.push_back(tPatch);
	    else
	      negPatch.push_back(tPatch);
	    //}
	}
      }
    }
  }
  patches.push_back(posPatch);
  patches.push_back(negPatch);
}
