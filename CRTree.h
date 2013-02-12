#ifndef __CRTREE__
#define __CRTREE__

#include "CPatch.h"
#include "util.h"
#include <ctime>

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
    std::cout << "Leaf " << vCenter.size() << " ";
    for(int i = 0; i < pfg.size(); i++)std::cout << pfg.at(i) << " ";
    std::cout << std::endl;
  }
  std::vector<float> pfg;
  std::vector<cv::Point> vCenter;
  std::vector<int> vClass;
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
    treetable = new int[num_nodes * 11];
    // init treetable
    for(int i = 0; i< num_nodes * 11; ++i)
      treetable[i] = 0;
    
    leaf= new LeafNode[(int)pow(2.0, int(max_depth))];
  }
  
  CRTree(const char* filename);

  //destructor
  ~CRTree()
    {
      //std::cout << "yobareta!" << std::endl;
      delete [] treetable;
      delete [] leaf;
    }
  
  // Set/Get functions
  unsigned int GetDepth() const {return max_depth;}
  unsigned int GetNumCenter() const {return num_cp;}

  // Regression
  const LeafNode* regression(CPatch &patch) const;

  // Training
  void growTree(std::vector<std::vector<CPatch> > &TrData, int node, int depth, float pnratio, CConfig conf, boost::mt19937 gen,const std::vector<int> &defaultClass_);

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
  double calcEntropy(const std::vector<CPatch> &set, int maxClass);
  double measureSet(const std::vector<std::vector<CPatch> >& SetA, const std::vector<std::vector<CPatch> >& SetB, unsigned int mode) {
    return InfGain(SetA, SetB);
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
  int nclass;

  std::vector<int> defaultClass, containClass, containClassA, containClassB;
};

inline void CRTree::generateTest(int* test, unsigned int max_w, unsigned int max_h, unsigned int max_c) {
  boost::mt19937    gen2(static_cast<unsigned long>(time(NULL)) );
  
  boost::uniform_int<> dst( 0, INT_MAX );
  boost::variate_generator<boost::mt19937&,
    boost::uniform_int<> > rand( gen, dst );

  boost::uniform_real<> dst2( 0, 1 );
  boost::variate_generator<boost::mt19937&,
    boost::uniform_real<> > rand2( gen, dst2 );

  //if(0.5 < rand2()){
    test[0] = rand() % max_w;
    test[1] = rand() % max_h;
    test[4] = rand() % max_w;
    test[5] = rand() % max_h;
    test[8] = rand() % (max_c - 1);

    test[2] = 0;
    test[3] = 0;
    test[6] = 0;
    test[7] = 0;
  /* }else{ */
  /*   test[8] = max_c - 1; */

  /*   test[0] = rand() % (max_w / 2 - 1); */
  /*   test[1] = rand() % (max_h / 2 - 1); */
  /*   test[2] = rand() % (max_w / 2 - 1); */
  /*   test[3] = rand() % (max_h / 2 - 1); */

  /*   test[4] = test[0] + test[2] + rand() % (max_w - test[0] - test[2] - 1); */
  /*   test[5] = test[1] + test[3] + rand() % (max_h - test[1] - test[3] - 1); */
  /*   test[6] = 1 + rand() % (max_w - test[4] - 1); */
  /*   test[7] = 1 + rand() % (max_h - test[5] - 1); */
  /* } */

}


#endif
