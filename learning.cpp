#include "CRForest.h"
#include "util.h"

using namespace std;

int main(int argc, char* argv[]){

  CConfig		conf;	 // setting

  //read and check argument
  if(argc < 2) {
    cout << "Usage: ./learning [config.xml] [off_tree] [learningMode]"<< endl;
    conf.loadConfig("config.xml");
  } else
    conf.loadConfig(argv[1]);

  if(argc < 3)
    conf.off_tree = 0;
  else
    conf.off_tree = atoi(argv[2]);

  if(argc < 4)
    conf.learningMode = 0;
  else
    conf.learningMode = atoi(argv[3]);
  
  //create tree directory
  string tpath(conf.treepath);
  tpath.erase(tpath.find_last_of(PATH_SEP));
  string execstr = "mkdir ";
  execstr += tpath;
  system( execstr.c_str() );

  // create random forest class
  CRForest forest(conf);

  // learning
  forest.learning();

  return 0; 
}
