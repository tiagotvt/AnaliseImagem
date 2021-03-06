#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

//"g++ $(pkg-config --cflags opencv) grama.cpp $(pkg-config --libs opencv) -o grama"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
  Scalar GRASS_MIN = Scalar(22, 30, 70);
  Scalar GRASS_MAX = Scalar(60,255,200);
  Mat edge,dst,threshold, final;
  Mat HSV;
  int dilation_size =4;
  int sqr = 200;
  

  Mat src = imread( "teste.jpg",1);
  int sqr_row = src.rows/sqr;
  int sqr_col = src.cols/sqr;


  blur( src, src, Size(2,2) );
  cvtColor(src,HSV,CV_BGR2HSV);
  inRange(HSV,GRASS_MIN,GRASS_MAX,threshold);
  Mat element = getStructuringElement( MORPH_RECT,Size( 2*dilation_size + 1, 2*dilation_size+1 ), Point( dilation_size, dilation_size ) );
  morphologyEx( threshold, dst, MORPH_CLOSE, element );
  

  int minBound=dst.rows;
  int maxBound=0;
  int tmp=0;

  for(int i = 0; i < dst.cols; i++){  
     for(int j = 0; j < dst.rows; j++){
        if(dst.at<uchar>(j, i)==0){
        tmp=j;
        break;
        }
     }
    if(tmp<minBound) minBound=tmp;
    if(tmp>maxBound) maxBound=tmp;
  }

    // Copia todos os pixels da imagem original que não são pretos no threshold
    src.copyTo(final, threshold);

  //line(dst, Point(0,minBound), Point(dst.cols,minBound),  Scalar(0), 1,8, 0); //minmum boudary
  //line(dst, Point(0,maxBound), Point(dst.cols,maxBound),  Scalar(255), 1,8, 0); // maximum boundary
  //line(src, Point(0,minBound), Point(dst.cols,minBound),  Scalar(0,0,255), 1,8, 0); //minmum boudary
  //line(src, Point(0,maxBound), Point(dst.cols,maxBound),  Scalar(0,0,255), 1,8, 0); // maximum boundary

    // Fazer o Average da cor dos pixels por area


  Vec3b cont;
  Mat final_sqr = Mat::zeros(4000,6000, CV_8UC3);
  int x_cont, y_cont, sqr_num=0,sqr_num_x=0,sqr_num_y=0, avg_temp=0;
  int blue_temp = 0;
  int green_temp = 0;
  int red_temp = 0;

  printf("TESTEEE\n %d %d %d",sqr_col*sqr_row, sqr_row, sqr_col);
  while(sqr_num < sqr_row*sqr_col)
  {
    printf("ALOOO");
     for(y_cont = (sqr*sqr_num_y); y_cont < (sqr*(sqr_num_y+1)); y_cont++) 
     {
      for(x_cont = (sqr*sqr_num_x); x_cont < (sqr*(sqr_num_x+1)); x_cont++) 
      {
          if ( final.at<Vec3b>(y_cont, x_cont) != Vec3b(0,0,0) ) 
          {
            blue_temp += final.at<Vec3b>(y_cont, x_cont)[0];
            green_temp += final.at<Vec3b>(y_cont, x_cont)[1];
            red_temp += final.at<Vec3b>(y_cont, x_cont)[2];
            avg_temp ++;
          }
      }
    }
    printf("%d  ",sqr_num);
    printf("%d  ",sqr_num_y);
    printf("%d  \n",sqr_num_x);
    printf("TESTEEE2\n");
    if(avg_temp==0) avg_temp=1;
    rectangle(final_sqr, Point(0+(sqr*sqr_num_x),(0+sqr*sqr_num_y)),Point((sqr*(sqr_num_x+1))-1,(sqr*(sqr_num_y+1))-1),
              Scalar(blue_temp/avg_temp,green_temp/avg_temp,red_temp/avg_temp),-1,8);
    //imwrite( "sqr.jpg", final_sqr );
    blue_temp=0;
    green_temp=0;
    red_temp=0;
    avg_temp=0;

    sqr_num++;
    sqr_num_x = sqr_num%sqr_col;
    if(sqr_num_x == 0)
    {
      sqr_num_y++;
    }
    printf("%d  ",sqr_num);
    printf("%d  ",sqr_num_y);
    printf("%d  \n",sqr_num_x);
    printf("TESTEEE3\n");
  }

  printf("TESTEEE4\n");
  imwrite( "sqr.jpg", final_sqr );
  imwrite( "threshold.jpg", threshold );
  imwrite( "dst.jpg", dst );
  imwrite( "HSV.jpg", src );
  imwrite( "final.jpg", final );
  //imshow( "source", src );
  //imshow( "threshold", threshold );
  //imshow( "dst", dst );
  waitKey(0);
  return 0;
}