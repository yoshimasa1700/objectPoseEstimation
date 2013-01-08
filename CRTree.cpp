#include "CRTree.h"

using namespace std;


// Read tree from file
CRTree::CRTree(const char* filename) {
  cout << "Load Tree " << filename << endl;

  int dummy;

  ifstream in(filename);
  if(in.is_open()) {
    // allocate memory for tree table
    in >> max_depth;
    num_nodes = (int)pow(2.0,int(max_depth+1))-1;
    // num_nodes x 7 matrix as vector
    treetable = new int[num_nodes * 7];
    int* ptT = &treetable[0];
		
    // allocate memory for leafs
    in >> num_leaf;
    leaf = new LeafNode[num_leaf];

    // number of center points per patch 
    in >> num_cp;

    // read tree nodes
    for(unsigned int n=0; n<num_nodes; ++n) {
      in >> dummy; in >> dummy;
      for(unsigned int i=0; i<7; ++i, ++ptT) {
	in >> *ptT;
      }
    }

    // read tree leafs
    LeafNode* ptLN = &leaf[0];
    for(unsigned int l=0; l<num_leaf; ++l, ++ptLN) {
      in >> dummy;
      in >> ptLN->pfg;
			
      // number of positive patches
      in >> dummy;
      ptLN->vCenter.resize(dummy);
      for(int i=0; i<dummy; ++i) {
	ptLN->vCenter[i].resize(num_cp);
	for(unsigned int k=0; k<num_cp; ++k) {
	  in >> ptLN->vCenter[i][k].x;
	  in >> ptLN->vCenter[i][k].y;
	}
      }
    }

  } else {
    cerr << "Could not read tree: " << filename << endl;
  }

  in.close();

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
      for(unsigned int i=0; i<7; ++i, ++ptT) {
	out << *ptT << " ";
      }
      out << endl;
    }
    out << endl;

    // save tree leafs
    LeafNode* ptLN = &leaf[0];
    for(unsigned int l=0; l<num_leaf; ++l, ++ptLN) {
      out << l << " " << ptLN->pfg << " " << ptLN->vCenter.size() << " ";
			
      for(unsigned int i=0; i<ptLN->vCenter.size(); ++i) {
	for(unsigned int k=0; k<ptLN->vCenter[i].size(); ++k) {
	  out << ptLN->vCenter[i][k].x << " " << ptLN->vCenter[i][k].y << " ";
	}
      }
      out << endl;
    }

    out.close();

    done = true;
  }


  return done;
}

void CRTree::growTree(vector<vector<CPatch> > &TrainSet, int node , int depth, float pnratio, CConfig conf, boost::mt19937 gen){
  
  boost::uniform_int<> zeroOne( 0, 1 );
  boost::variate_generator<boost::mt19937&, 
			   boost::uniform_int<> > rand( gen, zeroOne );

  if(depth < max_depth && TrainSet[0].size() > 0) {	
    
    // spilit patches by the binary test
    vector<vector<CPatch> > SetA;
    vector<vector<CPatch> > SetB;
    int test[6];
    
    // Set measure mode for split: 0 - classification, 1 - regression
    unsigned int measure_mode = 1;
    if( float(TrainSet[0].size()) / float(TrainSet[0].size()+TrainSet[1].size()) >= 0.05 && depth < max_depth-2 )
      measure_mode = rand();
    
    cout << "MeasureMode " << depth << " " << measure_mode << " " << TrainSet[0].size() << " " << endl;
    
    // Find optimal test
    if( optimizeTest(SetA, SetB, TrainSet, test, 100, measure_mode) ) {

      // Store binary test for current node
      int* ptT = &treetable[node*7];
      ptT[0] = -1; ++ptT; 
      for(int t=0; t<6; ++t)
	ptT[t] = test[t];

     
      
      double countA = 0;
      double countB = 0;
     

      for(unsigned int l=0; l<TrainSet.size(); ++l) {
	cout << "Final_Split A/B " << l << " " << SetA[l].size() << " " << SetB[l].size() << endl; 
	countA += SetA[l].size(); countB += SetB[l].size();
      }
      for(unsigned int l=0; l<TrainSet.size(); ++l) {
	cout << "Final_SplitA: " << SetA[l].size()/countA << "% "; 
      }
      cout << endl;
      for(unsigned int l=0; l<TrainSet.size(); ++l) {
	cout << "Final_SplitB: " << SetB[l].size() / countB << "% "; 
      }
      cout << endl;

      // Go left
      // If enough patches are left continue growing else stop
      if(SetA[0].size()+SetA[1].size()>min_samples) {
	growTree(SetA, 2*node+1, depth+1, pnratio, conf, gen);
      } else {
	makeLeaf(SetA, pnratio, 2*node+1);
      }

      // Go right
      // If enough patches are left continue growing else stop
      if(SetB[0].size()+SetB[1].size()>min_samples) {
	growTree(SetB, 2*node+2, depth+1, pnratio, conf, gen);
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
  treetable[node*7] = num_leaf;
  LeafNode* ptL = &leaf[num_leaf];

  // Store data
  ptL->pfg = TrainSet[1].size() / float(pnratio*TrainSet[0].size()+TrainSet[1].size());
  ptL->vCenter.resize( TrainSet[1].size() );
  for(unsigned int i = 0; i<TrainSet[1].size(); ++i) {
    ptL->vCenter[i] = TrainSet[1][i].vCenter;
  }

  // Increase leaf counter
  ++num_leaf;
}

bool CRTree::optimizeTest(std::vector<std::vector<CPatch> > &SetA, std::vector<std::vector<CPatch> > &SetB, std::vector<std::vector<CPatch> > &TrainSet, int* test, unsigned int iter, unsigned int measure_mode) {
	
  bool found = false;

  // temporary data for split into Set A and Set B
  std::vector<std::vector<CPatch> > tmpA(TrainSet.size());
  std::vector<std::vector<CPatch> > tmpB(TrainSet.size());

  // temporary data for finding best test
  std::vector<std::vector<IntIndex> > valSet(TrainSet.size());
  double tmpDist;
  // maximize!!!!
  double bestDist = -DBL_MAX; 
  int tmpTest[6];

  boost::uniform_int<> dst( 0, INT_MAX );
  boost::variate_generator<boost::mt19937&,
			   boost::uniform_int<> > rand( gen, dst );

 
  std::cout << "finding best test" << std::endl;
  
  // Find best test of ITER iterations
  for(unsigned int i =0; i<iter; ++i) {

    // reset temporary data for split
    for(unsigned int l =0; l<TrainSet.size(); ++l) {
      tmpA.at(l).clear();
      tmpB.at(l).clear(); 
    }

      

    // generate binary test without threshold
      
      //std::cout << TrainSet.at(0).at(0).patch.size() << std::endl;
      
      generateTest(&tmpTest[0], TrainSet.at(0).at(0).patchRoi.width, TrainSet.at(0).at(0).patchRoi.height, TrainSet.at(0).at(0).patch.size());
    



    // compute value for each patch
    evaluateTest(valSet, &tmpTest[0], TrainSet);

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

	

	// Split training data into two sets A,B accroding to threshold t 
	split(tmpA, tmpB, TrainSet, valSet, tr);

	// Do not allow empty set split (all patches end up in set A or B)
	if( tmpA[0].size()+tmpA[1].size()>0 && tmpB[0].size()+tmpB[1].size()>0 ) {

	  // Measure quality of split with measure_mode 0 - classification, 1 - regression
	  
	  tmpDist = measureSet(tmpA, tmpB, measure_mode);

	  // Take binary test with best split
	  if(tmpDist>bestDist) {

	    found = true;
	    bestDist = tmpDist;
	    for(int t=0; t<5;++t) test[t] = tmpTest[t];
	    test[5] = tr;

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

void CRTree::evaluateTest(std::vector<std::vector<IntIndex> >& valSet, const int* test, std::vector<std::vector<CPatch> > &TrainSet) {
  
  // for(int m = 0; m < 6; ++m)
  //   std::cout << test[m] << ", ";
  // std::cout << std::endl;
  
  for(unsigned int l = 0; l < TrainSet.size(); ++l) {
    valSet[l].resize(TrainSet[l].size());
    for(unsigned int i=0;i<TrainSet[l].size();++i) {

      // pointer to channel
      cv::Mat ptC = TrainSet[l][i].patch[test[4]];
      ///xcv::Mat toyoshi = *(TrainSet[l][i].p_image[test[4]]);
      //std::cout << "kokomade kitayo" <<TrainSet[l][i].p_image[test[4]] << std::endl;x


      // get pixel values 
      

      int p1 = (int)ptC.at<uchar>(test[1], test[0]);
      int p2 = (int)ptC.at<uchar>(test[3], test[2]);
		
      valSet[l][i].val = p1 - p2;
      valSet[l][i].index = i;			
    }
    sort( valSet[l].begin(), valSet[l].end() );
  }
}

void CRTree::split(vector<vector<CPatch> >& SetA, vector<vector<CPatch> >& SetB, const vector<vector<CPatch> >& TrainSet, const vector<vector<IntIndex> >& valSet, int t) {
  for(unsigned int l = 0; l<TrainSet.size(); ++l) {
    // search largest value such that val<t 
    vector<IntIndex>::const_iterator it = valSet[l].begin();
    while(it!=valSet[l].end() && it->val<t) {
      ++it;
    }

    SetA[l].resize(it-valSet[l].begin());
    SetB[l].resize(TrainSet[l].size()-SetA[l].size());

    it = valSet[l].begin();
    for(unsigned int i=0; i<SetA[l].size(); ++i, ++it) {
      SetA[l][i] = TrainSet[l][it->index];
    }
		
    it = valSet[l].begin()+SetA[l].size();
    for(unsigned int i=0; i<SetB[l].size(); ++i, ++it) {
      SetB[l][i] = TrainSet[l][it->index];
    }

  }
}

double CRTree::distMean(const std::vector<CPatch>& SetA, const std::vector<CPatch>& SetB) {
  std::vector<double> meanAx(num_cp,0);
  std::vector<double> meanAy(num_cp,0);
  for(std::vector<CPatch>::const_iterator it = SetA.begin(); it != SetA.end(); ++it) {
    for(unsigned int c = 0; c<num_cp; ++c) {
      meanAx[c] += (*it).vCenter[c].x;
      meanAy[c] += (*it).vCenter[c].y;
    }
  }

  for(unsigned int c = 0; c<num_cp; ++c) {
    meanAx[c] /= (double)SetA.size();
    meanAy[c] /= (double)SetA.size();
  }

  vector<double> distA(num_cp,0);
  for(std::vector<CPatch>::const_iterator it = SetA.begin(); it != SetA.end(); ++it) {
    for(unsigned int c = 0; c<num_cp; ++c) {
      double tmp = (*it).vCenter[c].x - meanAx[c];
      distA[c] += tmp*tmp;
      tmp = (*it).vCenter[c].y - meanAy[c];
      distA[c] += tmp*tmp;
    }
  }

  std::vector<double> meanBx(num_cp,0);
  std::vector<double> meanBy(num_cp,0);
  for(std::vector<CPatch>::const_iterator it = SetB.begin(); it != SetB.end(); ++it) {
    for(unsigned int c = 0; c<num_cp; ++c) {
      meanBx[c] += (*it).vCenter[c].x;
      meanBy[c] += (*it).vCenter[c].y;
    }
  }

  for(unsigned int c = 0; c<num_cp; ++c) {
    meanBx[c] /= (double)SetB.size();
    meanBy[c] /= (double)SetB.size();
  }

  std::vector<double> distB(num_cp,0);
  for(std::vector<CPatch>::const_iterator it = SetB.begin(); it != SetB.end(); ++it) {
    for(unsigned int c = 0; c<num_cp; ++c) {
      double tmp = (*it).vCenter[c].x - meanBx[c];
      distB[c] += tmp*tmp;
      tmp = (*it).vCenter[c].y - meanBy[c];
      distB[c] += tmp*tmp;
    }
  }

  double minDist = DBL_MAX;

  for(unsigned int c = 0; c<num_cp; ++c) {
    distA[c] += distB[c];
    if(distA[c] < minDist) minDist = distA[c];
  }

  return minDist/double( SetA.size() + SetB.size() ); 
}

double CRTree::InfGain(const std::vector<std::vector<CPatch> >& SetA, const std::vector<std::vector<CPatch> >& SetB) {

  // get size of set A
  double sizeA = 0;
  for(std::vector<std::vector<CPatch> >::const_iterator it = SetA.begin(); it != SetA.end(); ++it) {
    sizeA += it->size();
  }

  // negative entropy: sum_i p_i*log(p_i)
  double n_entropyA = 0;
  for(std::vector<std::vector<CPatch> >::const_iterator it = SetA.begin(); it != SetA.end(); ++it) {
    double p = double( it->size() ) / sizeA;
    if(p>0) n_entropyA += p*log(p); 
  }

  // get size of set B
  double sizeB = 0;
  for(std::vector<std::vector<CPatch> >::const_iterator it = SetB.begin(); it != SetB.end(); ++it) {
    sizeB += it->size();
  }

  // negative entropy: sum_i p_i*log(p_i)
  double n_entropyB = 0;
  for(std::vector<std::vector<CPatch> >::const_iterator it = SetB.begin(); it != SetB.end(); ++it) {
    double p = double( it->size() ) / sizeB;
    if(p>0) n_entropyB += p*log(p); 
  }

  return (sizeA*n_entropyA+sizeB*n_entropyB)/(sizeA+sizeB); 
}

/////////////////////// IO functions /////////////////////////////

void LeafNode::show(int delay, int width, int height) {
  char buffer[200];

  print();

  if(vCenter.size()>0) {
    vector<IplImage*> iShow(vCenter[0].size());
    for(unsigned int c = 0; c<iShow.size(); ++c) {
      iShow[c] = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U , 1 );
      cvSetZero( iShow[c] );
      for(unsigned int i = 0; i<vCenter.size(); ++i) {
	int y = height/2+vCenter[i][c].y;
	int x = width/2+vCenter[i][c].x;

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
