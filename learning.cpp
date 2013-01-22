#include "CRForest.h"
//#include "CRTree.h"

#include "util.h"

using namespace std;

int main(int argc, char* argv[]){

  //cout << "aiueo" << endl;
  CConfig		conf;	 // setting
  //std::vector<CDataset> dataSet; // training data name list and grand truth

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
  
  //create tree directory
  string tpath(conf.treepath);
  tpath.erase(tpath.find_last_of(PATH_SEP));
  string execstr = "mkdir ";
  execstr += tpath;
  system( execstr.c_str() );


  cout << "kokomade" << endl;
  // create random forest class
  CRForest forest(conf);

  // learning
  forest.learning();

  return 0; 
}
