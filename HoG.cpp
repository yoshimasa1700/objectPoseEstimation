/* 
// Author: Juergen Gall, BIWI, ETH Zurich
// Email: gall@vision.ee.ethz.ch
*/

#include <vector>
#include <iostream>
#include "HoG.h"

using namespace std;

HoG::HoG() {
  bins = 9;
  binsize = (3.14159265f*80.0f)/float(bins);;

  g_w = 5;
  Gauss = cvCreateMat( g_w, g_w, CV_32FC1 );
  double a = -(g_w-1)/2.0;
  double sigma2 = 2*(0.5*g_w)*(0.5*g_w);
  double count = 0;
  for(int x = 0; x<g_w; ++x) {
    for(int y = 0; y<g_w; ++y) {
      double tmp = exp(-( (a+x)*(a+x)+(a+y)*(a+y) )/sigma2);
      count += tmp;
      cvSet2D( Gauss, x, y, cvScalar(tmp) );
    }
  }
  cvConvertScale( Gauss, Gauss, 1.0/count);

  ptGauss = new float[g_w*g_w];
  int i = 0;
  for(int y = 0; y<g_w; ++y) 
    for(int x = 0; x<g_w; ++x)
      ptGauss[i++] = (float)cvmGet( Gauss, x, y );
}


void HoG::extractOBin(const cv::Mat* Iorient,const cv::Mat* Imagn, std::vector<cv::Mat*>& out, int off) {
  double* desc = new double[bins];

  
  // reset output image (border=0) and get pointers
    

  for(int k=off; k<bins+off; ++k){
    int r = out[k]->rows;
    int c = out[k]->cols;
    *(out[k]) = cv::Mat::zeros(r,c, CV_8U);
  }

  // cv::namedWindow("test");
  // cv::imshow("test",*Iorient);
  // cv::waitKey(0);
  // cv::destroyWindow("test");
  
  for(int y = 0; y < Iorient->rows - g_w; y++) {
    for(int x = 0; x < Iorient->cols - g_w; x++){
      // calc hog bin
      calcHoGBin(Iorient, Imagn, out, x, y);
    }
  }   
}

void HoG::calcHoGBin(const cv::Mat* IOri, const cv::Mat* IMag, std::vector<cv::Mat*>& out, int offX, int offY){
  int yy, xx;

  int i = 0;
  for(int y = 0; y < g_w; ++y){
    for(int x = 0; x < g_w; ++x, ++i){
      yy = y + offY;
      xx = x + offX;

      float v = (float)IOri->at<uchar>(yy, xx) / binsize;
      //std::cout << "v is " << v << std::endl;
      float w = (float)IMag->at<uchar>(yy, xx) * ptGauss[i];//Gauss.at<float>(y, x);
      //std::cout << "w is " << w << std::endl;
      int bin1 = int(v);
      int bin2;
      float delta = v - bin1 - 0.5f;
      if(delta < 0){
	bin2 = bin1 < 1 ? bins - 1 : bin1 - 1;
	delta = - delta;
      }else
	bin2 = bin1 < bins - 1 ? bin1 + 1 : 0;
      out.at(bin1 + 7)->at<uchar>(yy, xx) += (1 - delta) * w;
      out.at(bin2 + 7)->at<uchar>(yy, xx) += delta * w;
    }
  }

  //std::cout << "kokomade kitayonn" << std::endl;
 }


