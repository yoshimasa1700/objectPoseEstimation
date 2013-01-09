#ifndef __CRTREE__
#define __CRTREE__

#include "CPatch.h"
#include "util.h"
#include <Ctime>

// Auxilary structure
struct IntIndex {
	int val;
	unsigned int index;
	bool operator<(const IntIndex& a) const { return val<a.val; }
};

class LeafNode
{
 public:
  LeafNode(){};

  void show(int delay, int width, int height);
  void print() 
  {
    std::cout << "Leaf " << vCenter.size() << " " << pfg << std::endl;
  }
  float pfg;
  std::vector<std::vector<cv::Point> > vCenter;
    
};

class CRTree 
{
 public:
  //constructor
 CRTree(int		min_s,	//min sample
	int		max_d,	//max depth of tree
	int		cp,	//number of center point
	boost::mt19937	randomGen	// random number seed
	)	 
   : min_samples(min_s), max_depth(max_d), num_leaf(0), num_cp(cp), gen(randomGen)
  {
    num_nodes = (int)pow(2.0, int(max_depth + 1)) - 1;
      
    // number of nodes x 7 matrix as vector
    treetable = new int[num_nodes * 7];
    // init treetable
    for(int i = 0; i< num_nodes * 7; ++i)
      treetable[i] = 0;
    
    leaf= new LeafNode[(int)pow(2.0, int(max_depth))];
  }

  CRTree(const char* filename);

  //destructor
  ~CRTree()
    {
      delete [] treetable;
      delete [] leaf;
    }
  
  // Set/Get functions
  unsigned int GetDepth() const {return max_depth;}
  unsigned int GetNumCenter() const {return num_cp;}

  // Regression
  const LeafNode* regression(uchar** ptFCh, int stepImg) const;

  // Training
  void growTree(std::vector<std::vector<CPatch> > &TrData, int node, int depth, float pnratio, CConfig conf, boost::mt19937 gen);

  bool optimizeTest(std::vector<std::vector<CPatch> > &SetA,
			    std::vector<std::vector<CPatch> > &SetB, 
			    std::vector<std::vector<CPatch> > &TrainSet, 
			    int* test, 
			    unsigned int iter, 
			    unsigned int measure_mode);
  void generateTest(int* test, unsigned int max_w, unsigned int max_h, unsigned int max_c);

  void makeLeaf(std::vector<std::vector<CPatch> > &TrainSet, float pnratio, int node);

  void evaluateTest(std::vector<std::vector<IntIndex> >& valSet, const int* test, std::vector<std::vector<CPatch> > &TrainSet);
  void split(std::vector<std::vector<CPatch> >& SetA, std::vector<std::vector<CPatch> >& SetB, const std::vector<std::vector<CPatch> >& TrainSet, const std::vector<std::vector<IntIndex> >& valSet, int t);
  double distMean(const std::vector<CPatch>& SetA, const std::vector<CPatch>& SetB);
  double InfGain(const std::vector<std::vector<CPatch> >& SetA, const std::vector<std::vector<CPatch> >& SetB);

  double measureSet(const std::vector<std::vector<CPatch> >& SetA, const std::vector<std::vector<CPatch> >& SetB, unsigned int mode) {
    if (mode==0) return InfGain(SetA, SetB); else return -distMean(SetA[1],SetB[1]);
  };

  
  
  // IO functions
  bool saveTree(const char* filename) const;
  void showLeaves(int width, int height) const {
    for(unsigned int l=0; l<num_leaf; ++l)
      leaf[l].show(5000, width, height);
  }
  
 private:
  // Data structure
  // tree table
  // 2^(max_depth+1)-1 x 7 matrix as vector
  // column: leafindex x1 y1 x2 y2 channel thres
  // if node is not a leaf, leaf=-1
  int* treetable;
  
  // stop growing when number of patches is less than min_samples
  unsigned int	min_samples;
  // depth of the tree: 0-max_depth
  unsigned int	max_depth;
  // number of nodes: 2^(max_depth+1)-1
  unsigned int	num_nodes;
  // number of leafs
  unsigned int	num_leaf;
  // number of center points per patch
  unsigned int	num_cp;
  //leafs as vector
  LeafNode*	leaf;

  // depth of this tree
  unsigned int depth;

  boost::mt19937 gen;
};

inline void CRTree::generateTest(int* test, unsigned int max_w, unsigned int max_h, unsigned int max_c) {
  boost::mt19937    gen2(static_cast<unsigned long>(time(NULL)) );
  
  boost::uniform_int<> dst( 0, INT_MAX );
  boost::variate_generator<boost::mt19937&,
			   boost::uniform_int<> > rand( gen2, dst );
  
  test[0] = rand() % max_w;
  test[1] = rand() % max_h;
  test[2] = rand() % max_w;
  test[3] = rand() % max_h;
  test[4] = rand() % max_c;

}


#endif
