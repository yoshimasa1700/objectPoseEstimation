#include "CRTree.h"

using namespace std;


const LeafNode* CRTree::regression(CPatch &patch) const {
  // pointer to current node
  const int* pnode = &treetable[0];
  int node = 0;
  int p1,p2;


  // Go through tree until one arrives at a leaf, i.e. pnode[0]>=0)
  while(pnode[0]==-1) {
    // binary test 0 - left, 1 - right
    // Note that x, y are changed since the patches are given as matrix and not as image 
    // p1 - p2 < t -> left is equal to (p1 - p2 >= t) == false
		
    // pointer to channel
    //uchar* ptC = ptFCh[pnode[5]];
	  
    cv::Mat ptC = patch.patch.at(pnode[9]);
	  
    //std::cout << ptC << std::endl;

    if(pnode[9] == 32){
      p1 = 0;
      p2 = 0;
      for(int j = 0;j < pnode[3]; ++j){
	for(int k = 0; k < pnode[4]; ++k)
	  p1 += (int)ptC.at<uchar>(k + pnode[2],j +  pnode[1]);
      }
	
      for(int j = 0;j < pnode[7]; ++j){
	for(int k = 0; k < pnode[8]; ++k)
	  p2 += (int)ptC.at<uchar>(k + pnode[6],j +  pnode[5]);
      }

    }else{
      // get pixel values 
      p1 = (int)ptC.at<uchar>(pnode[2], pnode[1]);//*(ptC+pnode[1]+pnode[2]*stepImg);
      p2 = (int)ptC.at<uchar>(pnode[6], pnode[5]);//*(ptC+pnode[3]+pnode[4]*stepImg);
    }
    // test
    bool test = ( p1 - p2 ) >= pnode[10];

    // next node: 2*node_id + 1 + test
    // increment node/pointer by node_id + 1 + test
    int incr = node+1+test;
    node += incr;
    pnode += incr*11;
  }

  // return leaf
  return &leaf[pnode[0]];
}

// Read tree from file
CRTree::CRTree(const char* filename) {
  cout << "Load Tree " << filename << endl;

  int dummy;

  ifstream in(filename);
  if(in.is_open()) {
    // allocate memory for tree table
    in >> max_depth;
    std::cout << "max depth: " << max_depth << std::endl;
    num_nodes = (int)pow(2.0,int(max_depth+1))-1;
    // num_nodes x 7 matrix as vector
    treetable = new int[num_nodes * 11];
    int* ptT = &treetable[0];
		
    // allocate memory for leafs
    in >> num_leaf;
    leaf = new LeafNode[num_leaf];

    std::cout << "number of leaf: " << num_leaf << std::endl;

    // number of center points per patch 
    in >> num_cp;

    // read tree nodes
    for(unsigned int n=0; n<num_nodes; ++n) {
      in >> dummy; in >> dummy;
      for(unsigned int i=0; i<11; ++i, ++ptT) {
	in >> *ptT;
      }
    }

    // read tree leafs
    LeafNode* ptLN = &leaf[0];
    for(unsigned int l=0; l<num_leaf; ++l, ++ptLN) {
      in >> dummy; // leaf node number
      int allClassNum = 0; // class number
      in >> allClassNum;
      int containClassNum = 0;
      in >> containClassNum;
      ptLN->pfg.resize(allClassNum);
      ptLN->vCenter.resize(allClassNum);

      int cNum;
      int containPoints;
      for(int i = 0; i < allClassNum; ++i){
	in >> cNum;
	in >> ptLN->pfg.at(cNum);
	in >> containPoints;

	ptLN->vCenter.at(cNum).resize(containPoints);
	
	for(int j = 0; j < containPoints; j++){
	  in >> ptLN->vCenter.at(cNum).at(j).x;

	  in >> ptLN->vCenter.at(cNum).at(j).y;
	}      
      }
      
      // for(int m = 0; m < maxNum; m++){
      // 	in >> ptLN->pfg.at(m);
      // }
      // // number of positive patches
      // in >> dummy;
      // ptLN->vCenter.resize(dummy);
      // ptLN->vClass.resize(dummy);
      // for(int i=0; i<dummy; ++i) {
      // 	// ptLN->vCenter[i].resize(num_cp);
      // 	// for(unsigned int k=0; k<num_cp; ++k) {
      // 	//   in >> ptLN->vCenter[i][k].x;
      // 	//   in >> ptLN->vCenter[i][k].y;
      // 	// }
      // 	in >> ptLN->vClass[i];
      // 	in >> ptLN->vCenter[i].x;
      // 	in >> ptLN->vCenter[i].y;
      // }

      for(int i = 0; i < ptLN->pfg.size(); ++i)
	std::cout << ptLN->pfg.at(i) << std::endl;

      for(int i = 0; i < ptLN->vCenter.size(); ++i){
	for(int j = 0; j < ptLN->vCenter.at(i).size(); ++j)
	  std::cout << ptLN->vCenter.at(i).at(j).x << " ";
	std::cout << std::endl;
      }
    }

  } else {
    cerr << "Could not read tree: " << filename << endl;
  }

  in.close();

  //std::cin >> dummy;

}

bool CRTree::saveTree(const char* filename) const {
  cout << "Save Tree " << filename << endl;

  bool done = false;

  ofstream out(filename);
  if(out.is_open()) {

    out << max_depth << " " << num_leaf << " " << num_cp << endl;

    // save tree nodes
    int* ptT = &treetable[0];
    int depth = 0;
    unsigned int step = 2;
    for(unsigned int n=0; n<num_nodes; ++n) {
      // get depth from node
      if(n==step-1) {
	++depth;
	step *= 2;
      }

      out << n << " " << depth << " ";
      for(unsigned int i=0; i<11; ++i, ++ptT) {
	out << *ptT << " ";
      }
      out << endl;
    }
    out << endl;

    

    // save tree leafs
    LeafNode* ptLN = &leaf[0];
    for(unsigned int l=0; l<num_leaf; ++l, ++ptLN) {
      out << l << " ";

      int containClassNum = 0;
      for(int i = 0; i < ptLN->pfg.size(); ++i){
	if(ptLN->pfg.at(i) != 0)
	  containClassNum++;
      }

      out << ptLN->pfg.size() << " " << containClassNum << " "; // class number
      
      // std::vector<int> classNum(defaultClass.size(), 0);
      // for(int i = 0; i < ptLN->vCenter.size(); ++i)
      // 	classNum(ptLN->vClass.at(i)) += 1;

      for(int j = 0; j < ptLN->pfg.size(); ++j){
	if(ptLN->pfg.at(j) != 0){
	  out << j << " " << ptLN->pfg.at(j) << " " << ptLN->vCenter.at(j).size() << " ";
	  for(int i = 0; i < ptLN->vCenter.at(j).size(); ++i)
	    out << ptLN->vCenter.at(j).at(i).x << " " << ptLN->vCenter.at(j).at(i).y
		<< " ";	  
	}
      }
      
      out << endl;

      // for(int j = 0; j < containClassNum; j++){
      // 	out << ptLN->pfg.at(j) << " " << classNum.at(j) << " ";
	
	
      // 	// for(unsigned int i=0; i<ptLN->vCenter.size(); ++i) {
      // 	// 	for(unsigned int k=0; k<ptLN->vCenter[i].size(); ++k) {
      // 	// 	  out << ptLN->vCenter[i][k].x << " " << ptLN->vCenter[i][k].y << " ";
      // 	// 	}
      // 	// }

      // 	for(int i = 0; i < classNum.at(); ++i){
      // 	  out << ptLN->vClass.at(i) << " " << ptLN->vCenter.at(i).x << " " << ptLN->vCenter.at(i).y << " "; 
      // 	}
      // }
      // out << endl;
    }

    out.close();

    done = true;
  }


  return done;
}

void CRTree::growTree(vector<vector<CPatch> > &TrainSet, int node , int depth, float pnratio, CConfig conf, boost::mt19937 gen, const std::vector<int> &defaultClass_){
  
  // for(int i = 0; i < TrainSet.at(0).size(); ++i){
  //   std::cout << "this is for debug growtree trainset center " << TrainSet.at(0).at(i).center << std::endl;
  // }

  // int dummy;
  // std::cin >> dummy;

  boost::uniform_int<> zeroOne( 0, 1 );
  boost::variate_generator<boost::mt19937&, 
			   boost::uniform_int<> > rand( gen, zeroOne );
  
  defaultClass = defaultClass_;
  nclass = defaultClass.size();

  containClass.clear();
  containClass.resize(nclass);
  for(int i = 0; i < TrainSet.at(0).size(); ++i)
    containClass.at(TrainSet.at(0).at(i).classNum)++;

  int remainClass = 0;
  for(int c = 0; c < nclass; ++c)
    if(containClass.at(c) != 0)
      remainClass++;
  
  if(depth < max_depth && remainClass > 1) {

    //if(depth < max_depth && TrainSet[0].size() > 0) {	
    
    // spilit patches by the binary test
    vector<vector<CPatch> > SetA;
    vector<vector<CPatch> > SetB;
    int test[10];
    
    // Set measure mode for split: 0 - classification, 1 - regression
    unsigned int measure_mode = 1;
    //if( float(TrainSet[1].size()) / float(TrainSet[0].size()+TrainSet[1].size()) >= 0.05 && 
    if(depth < max_depth-2 )
      measure_mode = rand();
    
    //cout << "MeasureMode " << measure_mode << "depth " << depth << "Pos patches " << TrainSet[0].size() << " Neg Patches " << TrainSet[1].size() << " pnratio " << pnratio  <<endl;

    cout << "Node num: " << node << endl;
    for(int i = 0; i < nclass; ++i){
      std::cout << "class" << i << ": " << containClass.at(i) << endl;
    }

    //containClass.clear();
    //containClass.resize(nclass);

    //for(int i = 0; i < TrainSet.at(0).size(); ++i){
    //containClass.at(TrainSet.at(0).at(i).classNum)++;
    //}

    //for(int i = 0; i < nclass; ++i)
    //std::cout << "class" << i << " : " << containClass.at(i) << endl;
    
    // Find optimal test
    if( optimizeTest(SetA, SetB, TrainSet, test, 100, measure_mode) ) {

      // Store binary test for current node
      int* ptT = &treetable[node*11];
      ptT[0] = -1; ++ptT; 
      for(int t=0; t<10; ++t)
	ptT[t] = test[t];
      
      double countA = 0;
      double countB = 0;
     

      // for(unsigned int l=0; l<TrainSet.size(); ++l) {
      // 	cout << "Final_Split A/B " << l << " " << SetA[l].size() << " " << SetB[l].size() << endl; 
      // 	countA += SetA[l].size(); countB += SetB[l].size();
      // }
      // for(unsigned int l=0; l<TrainSet.size(); ++l) {
      // 	cout << "Final_SplitA: " << 100 * SetA[l].size()/countA << "% "; 
      // }
      // cout << endl;
      // for(unsigned int l=0; l<TrainSet.size(); ++l) {
      // 	cout << "Final_SplitB: " << 100 *SetB[l].size() / countB << "% "; 
      // }
      //cout << endl;

      containClassA.clear();
      containClassA.resize(nclass);
      containClassB.clear();
      containClassB.resize(nclass);
      
      for(int l = 0; l < SetA.at(0).size(); ++l)
	containClassA.at(SetA.at(0).at(l).classNum)++;
      for(int l = 0; l < SetB.at(0).size(); ++l)
	containClassB.at(SetB.at(0).at(l).classNum)++;
      
      for(int l = 0; l < nclass; ++l)
      std:cout << "class" << l << " is splitted " << containClassA.at(l) << " " << containClassB.at(l) << std::endl;

      // Go left
      // If enough patches are left continue growing else stop
      if(SetA[0].size()+SetA[1].size()>min_samples) {
	growTree(SetA, 2*node+1, depth+1, pnratio, conf, gen, defaultClass_);
      } else {
	makeLeaf(SetA, pnratio, 2*node+1);
      }

      // Go right
      // If enough patches are left continue growing else stop
      if(SetB[0].size()+SetB[1].size()>min_samples) {
	growTree(SetB, 2*node+2, depth+1, pnratio, conf, gen, defaultClass_);
      } else {
	makeLeaf(SetB, pnratio, 2*node+2);
      }


    } else {

      // Could not find split (only invalid one leave split)
      makeLeaf(TrainSet, pnratio, node);
	
    }	

  } else {

    // Only negative patches are left or maximum depth is reached
    makeLeaf(TrainSet, pnratio, node);
	
  }
  
}

// Create leaf node from patches 
void CRTree::makeLeaf(std::vector<std::vector<CPatch> > &TrainSet, float pnratio, int node) {
  // Get pointer
  treetable[node*11] = num_leaf;
  LeafNode* ptL = &leaf[num_leaf];


  // for(int i = 0; i < TrainSet.at(0).size(); ++i){
  //   cv::namedWindow("test");cv::imshow("test",TrainSet.at(0).at(i).patch.at(0));
  //   cv::waitKey(0);
  //   cv::destroyWindow("test");


  // }

  //std::vector<int> reachedClass(nclass,0);
  // std::vector<int> maxflag;

  // for(int i = 0; i < TrainSet.at(0).size(); ++i)
  //   reachedClass.at(TrainSet.at(0).at(i).classNum) += 1;

  // maxflag.clear();

  // int maxnum = 0;
  // for(int c = 0; c < nclass; ++c){
  //   if(reachedClass.at(c) > maxnum){
  //     maxflag.clear();
  //     maxflag.push_back(c);
  //     maxnum = reachedClass.at(c);
  //   }else if(reachedClass.at(c) == maxnum){
  //     maxflag.push_back(c);
  //   }
  // }
  patchPerClass.clear();

  // divide reached patch to each class
  patchPerClass.resize(nclass);
  for(int c = 0; c < nclass; ++c)
    patchPerClass.at(c).clear();

  for(int i = 0; i < TrainSet.at(0).size(); ++i){
    std::cout << "this is for debug trainset center is " << TrainSet.at(0).at(i).center << std::endl;
    patchPerClass.at(TrainSet.at(0).at(i).classNum).push_back(TrainSet.at(0).at(i));
  }

  for(int i = 0; i < nclass; ++i){
    for(int j = 0; j < patchPerClass.at(i).size(); ++j){
      std::cout << "class: " << i  << " patch: " << j << " center:" << patchPerClass.at(i).at(j).center << std::endl;
    }
  }


  //int dummy;
  //std::cin >> dummy;

  // calc total default patch num
  int totalPatchNum = 0;
  for(int c = 0; c < nclass; ++c)
    totalPatchNum += defaultClass.at(c);

  ptL->pfg.resize(nclass);
  for(int k = 0; k < patchPerClass.size(); ++k){
    if(patchPerClass.at(k).size() != 0){
      //int totalnum = reachedClass.at(k);
    
      float maxOtherRatio = (float)defaultClass.at(k) 
	/ (float)(totalPatchNum - defaultClass.at(k));
  
      // Store data
      ptL->pfg.at(k) = (float)patchPerClass.at(k).size() / (maxOtherRatio * (float)(TrainSet.at(0).size() - patchPerClass.at(k).size()) + patchPerClass.at(k).size());
    }else{
      ptL->pfg.at(k) = 0;
    }
  }
				  
  // ptL->vCenter.resize( TrainSet[0].size() );
  // ptL->vClass.resize( TrainSet[0].size() );
  // for(unsigned int i = 0; i<TrainSet[0].size(); ++i) {
  //   ptL->vCenter[i] = TrainSet[0][i].center;
  //   ptL->vClass[i] = TrainSet[0][i].classNum;
  // }
				  
      
  ptL->vCenter.resize(nclass);
  //ptL->vClass.resize(TrainSet.at(0).size());
  
  int count = 0;

 
  for(int i = 0; i < nclass; ++i){
    ptL->vCenter.at(i).clear();

    for(int j = 0; j < patchPerClass.at(i).size(); ++j)
      ptL->vCenter.at(i).push_back(patchPerClass.at(i).at(j).center);
  }

  // Increase leaf counter
  ++num_leaf;
}

bool CRTree::optimizeTest(std::vector<std::vector<CPatch> > &SetA, std::vector<std::vector<CPatch> > &SetB, const std::vector<std::vector<CPatch> > &TrainSet, int* test, unsigned int iter, unsigned int measure_mode) {
	
  bool found = false;

  // temporary data for split into Set A and Set B
  std::vector<std::vector<CPatch> > tmpA(TrainSet.size());
  std::vector<std::vector<CPatch> > tmpB(TrainSet.size());

  std::cout << "Trainset size" << TrainSet.size() << std::endl;

  // temporary data for finding best test
  std::vector<std::vector<IntIndex> > valSet(TrainSet.size());
  double tmpDist;
  // maximize!!!!
  double bestDist = -DBL_MAX; 
  int tmpTest[10];

  boost::uniform_int<> dst( 0, INT_MAX );
  boost::variate_generator<boost::mt19937&,
			   boost::uniform_int<> > rand( gen, dst );

 
  std::cout << "finding best test!" << std::endl;
  
  // Find best test of ITER iterations
  for(unsigned int i =0; i<iter; ++i) {

    // reset temporary data for split
    for(unsigned int l =0; l<TrainSet.size(); ++l) {
      tmpA.at(l).clear();
      tmpB.at(l).clear(); 
    }

      
    
    // generate binary test without threshold
      
      //std::cout << TrainSet.at(0).at(0).patch.size() << std::endl;
      
      generateTest(&tmpTest[0], 
		   TrainSet.at(0).at(0).patchRoi.width, 
		   TrainSet.at(0).at(0).patchRoi.height, 
		   TrainSet.at(0).at(0).patch.size());
    
      //for(int q = 0; q < 9; ++q)
      //cout << tmpTest[q] << " ";
      //cout << endl;


    // compute value for each patch
    evaluateTest(valSet, &tmpTest[0], TrainSet);

    // for(int l = 0; l < valSet.at(0).size(); ++l){
    //   std::cout << "val = " << valSet.at(0).at(l).val << " index = " << valSet.at(0).at(l).index << std::endl;
    // }

    // int dummy;
    // std::cin >> dummy;


    //std::cout << "evaluation end" << std::endl;

    // find min/max values for threshold
    int vmin = INT_MAX;
    int vmax = INT_MIN;
    for(unsigned int l = 0; l<TrainSet.size(); ++l) {
      if(valSet[l].size()>0) {
	if(vmin>valSet[l].front().val)  vmin = valSet[l].front().val;
	if(vmax<valSet[l].back().val )  vmax = valSet[l].back().val;
      }
    }
    int d = vmax-vmin;

    if(d>0) {

      
      // Find best threshold
      for(unsigned int j=0; j<10; ++j) { 

	// Generate some random thresholds
	int tr = (rand() % (d)) + vmin; 

	

	// // Split training data into two sets A,B accroding to threshold t 
	split(tmpA, tmpB, TrainSet, valSet, tr);

	// std::cout << "this is for debug " << std::endl;
	// std::cout << "setA----------------------------------------------------" << std::endl;
	// for(int j = 0; j < tmpA.at(0).size(); ++j)
	//   std::cout << tmpA.at(0).at(j).center << std::endl;

	// std::cout << "setB****************************************************" << std::endl;
	// for(int j = 0; j < tmpB.at(0).size(); ++j)
	//   std::cout << tmpB.at(0).at(j).center << std::endl;

	// //int dummy;
	// std::cin >> dummy;

	// Do not allow empty set split (all patches end up in set A or B)
	if( tmpA[0].size()+tmpA[1].size()>0 && tmpB[0].size()+tmpB[1].size()>0 ) {

	  // Measure quality of split with measure_mode 0 - classification, 1 - regression
	  
	  tmpDist = measureSet(tmpA, tmpB, measure_mode);

	  // Take binary test with best split
	  if(tmpDist>bestDist) {

	    found = true;
	    bestDist = tmpDist;
	    for(int t=0; t<10;++t) test[t] = tmpTest[t];
	    test[9] = tr;

	    //cout << "iretayo" << endl;
	    SetA = tmpA;
	    SetB = tmpB;
	  }

	}

      } // end for j

    }

    pBar(i, iter, 50);

  } // end iter

  std::cout << std::endl;

  // return true if a valid test has been found
  // test is invalid if only splits with an empty set A or B has been created
  return found;
}

void CRTree::evaluateTest(std::vector<std::vector<IntIndex> >& valSet, const int* test, const std::vector<std::vector<CPatch> > &TrainSet) {
  
  // for(int m = 0; m < 6; ++m)
  //   std::cout << test[m] << ", ";
  // std::cout << std::endl;
  
  int p1, p2;
  
  valSet.clear();
  valSet.resize(2);

  for(unsigned int l = 0; l < TrainSet.size(); ++l) {
    valSet[l].resize(TrainSet[l].size());
    for(unsigned int i=0;i<TrainSet[l].size();++i) {
      // pointer to channel
      cv::Mat ptC = TrainSet[l][i].patch[test[8]];
      if(test[8] == 32){
	std::cout << "this is for debug hyahhaaaaaaaaaaaaaaaaaaaa" << std::endl;
	int dummy;
	std::cin >> dummy;
	p1 = 0;
	p2 = 0;
	for(int j = 0;j < test[2]; ++j){
	  for(int k = 0; k < test[3]; ++k)
	    p1 += (int)ptC.at<uchar>(k + test[1],j +  test[0]);
	}
	
	for(int j = 0;j < test[6]; ++j){
	  for(int k = 0; k < test[7]; ++k)
	    p2 += (int)ptC.at<uchar>(k + test[5],j +  test[4]);
	}
	
      }else{
	// get pixel values 
	p1 = (int)ptC.at<uchar>(test[1], test[0]);
	p2 = (int)ptC.at<uchar>(test[5], test[4]);
      }
      valSet[l][i].val = p1 - p2;
      valSet[l][i].index = i;
    }

    sort( valSet[l].begin(), valSet[l].end() , IntIndex::lessVal);
  }
}

void CRTree::split(vector<vector<CPatch> >& SetA, vector<vector<CPatch> >& SetB, const vector<vector<CPatch> >& TrainSet, const vector<vector<IntIndex> >& valSet, int t) {
  SetA.at(0).clear();
  SetB.at(0).clear();
  
  for(int i = 0; i < valSet.at(0).size(); ++i){
    if(valSet.at(0).at(i).val < t){
      SetA.at(0).push_back(TrainSet.at(0).at(valSet.at(0).at(i).index));
      SetA.at(0).back().center = TrainSet.at(0).at(valSet.at(0).at(i).index).center;
    }else{
      SetB.at(0).push_back(TrainSet.at(0).at(valSet.at(0).at(i).index));
      SetB.at(0).back().center = TrainSet.at(0).at(valSet.at(0).at(i).index).center;
    }
  }

  //std::cout << TrainSet.at(0).size() << " " << SetA.at(0).size() << " " << SetB.at(0).size() << std::endl;

  //int dummy;
  //std::cin >> dummy;

  // this is for debug
  // std::cout << "TrainSet-----------------------------------------------------------------------------------------------------------------" << std::endl;
  // for(int i = 0; i < TrainSet.at(0).size(); ++i)
  //   std::cout << TrainSet.at(0).at(i).center << std::endl;

  // std::cout << "SetA-----------------------------------------------------------------------------------------------------------------" << std::endl;
  // for(int i = 0; i < SetA.at(0).size(); ++i)
  //   std::cout << SetA.at(0).at(i).center << std::endl;

  // std::cout << "SetB-----------------------------------------------------------------------------------------------------------------" << std::endl;
  // for(int i = 0; i < SetB.at(0).size(); ++i)
  //   std::cout << SetB.at(0).at(i).center << std::endl;
  
  // int dummy;
  
  // std::cin >> dummy;

  // for(unsigned int l = 0; l<TrainSet.size(); ++l) {
  //   // search largest value such that val<t 
  //   vector<IntIndex>::const_iterator it = valSet[l].begin();
  //   while(it!=valSet[l].end() && it->val<t) {
  //     ++it;
  //   }

  //   SetA[l].resize(it-valSet[l].begin());
  //   SetB[l].resize(TrainSet[l].size()-SetA[l].size());

  //   it = valSet[l].begin();
  //   for(unsigned int i=0; i<SetA[l].size(); ++i, ++it) {
  //     SetA[l][i] = TrainSet[l][it->index];
  //   }
		
  //   it = valSet[l].begin()+SetA[l].size();
  //   for(unsigned int i=0; i<SetB[l].size(); ++i, ++it) {
  //     SetB[l][i] = TrainSet[l][it->index];
  //   }

  // }
}

double CRTree::distMean(const std::vector<CPatch>& SetA, const std::vector<CPatch>& SetB) {
  std::vector<double> meanAx(nclass,0);
  std::vector<double> meanAy(nclass,0);
  //for(std::vector<CPatch>::const_iterator it = SetA.begin(); it != SetA.end(); ++it) {


  for(int i = 0; i < SetA.size(); ++i){
    //for(unsigned int c = 0; c<nclass; ++c) {
    meanAx[SetA.at(i).classNum] += SetA.at(i).center.x;
    meanAy[SetA.at(i).classNum] += SetA.at(i).center.y;
      //}
  }

  //for(unsigned int c = 0; c<num_cp; ++c) {
  for(int c = 0; c < nclass; ++c){
    meanAx[c] /= (double)SetA.size();
    meanAy[c] /= (double)SetA.size();
  }
    //}

  vector<double> distA(nclass,0);
  //for(std::vector<CPatch>::const_iterator it = SetA.begin(); it != SetA.end(); ++it) {
  for(int i = 0; i< SetA.size(); ++i){
    //for(unsigned int c = 0; c<num_cp; ++c) {
    double tmp = SetA.at(i).center.x - meanAx[SetA.at(i).classNum];
    distA[SetA.at(i).classNum] += tmp*tmp;
    tmp = SetA.at(i).center.y - meanAy[SetA.at(i).classNum];
    distA[SetA.at(i).classNum] += tmp*tmp;
      //}
  }

  std::vector<double> meanBx(nclass,0);
  std::vector<double> meanBy(nclass,0);
  //for(std::vector<CPatch>::const_iterator it = SetB.begin(); it != SetB.end(); ++it) {
  for(int i = 0; i < SetB.size(); ++i){
    //for(unsigned int c = 0; c<num_cp; ++c) {
    meanBx[SetB.at(i).classNum] += SetB.at(i).center.x;
    meanBy[SetB.at(i).classNum] += SetB.at(i).center.y;
      //}
  }

  for(int c = 0; c < nclass; c++){

  //for(unsigned int c = 0; c<num_cp; ++c) {
    meanBx[c] /= (double)SetB.size();
    meanBy[c] /= (double)SetB.size();
    //}
  }

  std::vector<double> distB(nclass,0);
  for(std::vector<CPatch>::const_iterator it = SetB.begin(); it != SetB.end(); ++it) {
    //for(unsigned int c = 0; c<num_cp; ++c) {
    double tmp = (*it).center.x - meanBx[(*it).classNum];
    distB[(*it).classNum] += tmp*tmp;
    tmp = (*it).center.y - meanBy[(*it).classNum];
    distB[(*it).classNum] += tmp*tmp;
      //}
  }

  double minDist = DBL_MAX;

  for(unsigned int c = 0; c < nclass; ++c) {
    distA[c] += distB[c];
    if(distA[c] < minDist) minDist = distA[c];
  }

  return minDist/double( SetA.size() + SetB.size() ); 
}

double CRTree::InfGain(const std::vector<std::vector<CPatch> >& SetA, const std::vector<std::vector<CPatch> >& SetB) {
  std::vector<CPatch> set;
  set.clear();

  int maxClass = 0;
  int maxClassNum = 0;
  for(int i = 0; i < nclass; ++i)
    if(maxClassNum < containClass.at(i)){
      maxClassNum = containClass.at(i);
      maxClass = i;
    }

  for(int i = 0; i < SetA.at(0).size(); ++i)
    set.push_back(SetA.at(0).at(i));

  for(int i = 0; i < SetB.at(0).size(); ++i)
    set.push_back(SetB.at(0).at(i));
 
  double entoropyA = calcEntropy(SetA.at(0), maxClass);
  double entoropyB = calcEntropy(SetB.at(0), maxClass);
  double entoropy = calcEntropy(set, maxClass);

  double wa = (double)SetA.at(0).size() / set.size();
  double wb = (double)SetB.at(0).size() / set.size();

  return entoropy - (wa * entoropyA + wb * entoropyB);

  // //std::vector<double> n_entoropyA;
  // std::vector<int> classDataA(nclass, 0);
  // std::vector<int> classDataB(nclass, 0);
  // // get size of set A
  // double sizeA = 0;
  // for(std::vector<std::vector<CPatch> >::const_iterator it = SetA.begin(); it != SetA.end(); ++it) {
  //   sizeA += it->size();
  // }

  // // negative entropy: sum_i p_i*log(p_i)
  // double n_entropyA = 0;
  // // if(SetA.at(1).size() > 0){
  // //   for(std::vector<std::vector<CPatch> >::const_iterator it = SetA.begin(); it != SetA.end(); ++it) {
  // //     //for(int i = 0; i < nclass; ++i){
  // //     double p = double( it->size() ) / sizeA;
  // //     if(p>0) n_entropyA += p*log(p) / log(2.0); 
  // //   }
  // // }

  // std::cout << "n_entropyA = " << n_entropyA << std::endl;

  // // calculate entropy of class
  // for(int i = 0; i < SetA.at(0).size(); ++i){
  //   classDataA.at(SetA.at(0).at(i).classNum)++;
  // }
  // for(int c = 0; c < nclass; ++c){
  //   if(classDataA.at(c) != 0){
  //     double p = double(classDataA.at(c)) / SetA.at(0).size();
  //     n_entropyA += p * log(p) / log(2.0);
  //   }
  // }


  // std::cout << "n_entropyA = " << n_entropyA << std::endl;
  // // get size of set B
  // double sizeB = 0;
  // for(std::vector<std::vector<CPatch> >::const_iterator it = SetB.begin(); it != SetB.end(); ++it) {
  //   sizeB += it->size();
  // }

  // // negative entropy: sum_i p_i*log(p_i)
  // double n_entropyB = 0;
  // if(SetB.at(1).size() > 0){
  //   for(std::vector<std::vector<CPatch> >::const_iterator it = SetB.begin(); it != SetB.end(); ++it) {
  //     double p = double( it->size() ) / sizeB;
  //     if(p>0) n_entropyB += p*log(p) / log(2.0); 
  //   }
  // }

  // std::cout << "n_entropyB = " << n_entropyB << std::endl;

  // // calculate entropy of class
  // for(int i = 0; i < SetB.at(0).size(); ++i){
  //   classDataB.at(SetB.at(0).at(i).classNum)++;
  // }
  // for(int c = 0; c < nclass; ++c){
  //   if(classDataB.at(c) != 0){
  //     double p = double(classDataB.at(c)) / SetB.at(0).size();
  //     n_entropyB += p * log(p) / log(2.0);
  //   }
  // }

  // std::cout << "n_entropyB = " << n_entropyB << std::endl;

  // return (sizeA*n_entropyA+sizeB*n_entropyB)/(sizeA+sizeB); 
}

double CRTree::calcEntropy(const std::vector<CPatch> &set, int maxClass)
{
  double entropy = 0;
  int maxClassNum = 0;
  int otherClass = 0;
  
  for(int i = 0; i < set.size(); ++i)
    if(set.at(i).classNum == maxClass)
      maxClassNum++;

  otherClass = set.size() - maxClassNum;

  double p = (double)maxClassNum / (double)set.size();
  entropy += -1 * p * log(p);

  p = (double)otherClass / (double)set.size();
  entropy += -1 * p * log(p);

  return entropy;
}
/////////////////////// IO functions /////////////////////////////

void LeafNode::show(int delay, int width, int height) {
  char buffer[200];

  print();



  if(vCenter.size()>0) {
    vector<IplImage*> iShow(vCenter.size());
    for(unsigned int c = 0; c < iShow.size(); ++c) {
      iShow[c] = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U , 1 );
      cvSetZero( iShow[c] );
      for(unsigned int i = 0; i<vCenter[c].size(); ++i) {
	int y = height/2+vCenter[c][i].y;
	int x = width/2+vCenter[c][i].x;

	if(x>=0 && y>=0 && x<width && y<height)
	  cvSetReal2D( iShow[c],  y,  x, 255 );
	}
      sprintf(buffer,"Leaf%d",c);
      cvNamedWindow(buffer,1);
      cvShowImage(buffer, iShow[c]);
    }
		
    cvWaitKey(delay);
		
    for(unsigned int c = 0; c<iShow.size(); ++c) {
      sprintf(buffer,"Leaf%d",c);
      cvDestroyWindow(buffer);
      cvReleaseImage(&iShow[c]);
    }
  }
}
