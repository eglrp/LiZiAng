//在出现窗口的条件下拍照，延时有问题
#include "iostream"    
#include <cv.h>  
#include <cxcore.h>    
#include <highgui.h>   
#include <opencv2/core/core_c.h>  
#include <string.h>
#include <unistd.h>
#include <time.h>
using namespace std;   
void solve(char * str1, int i)
{
	i = i % 10;
	char c = i + 48;
	int l = strlen(str1);
	str1[l] = '-';
	str1[l+1] = c;
	str1[l+2] = '\0';	
	char str2[] = ".jpg";
	strcat(str1,str2);
} 
int main( int argc, char** argv )    
{ 
	IplImage* img, *newimg;//图像结构体
  //声明IplImage指针    
  IplImage* pFrame = NULL;    
    
 //获取摄像头    
  CvCapture* pCapture = cvCreateCameraCapture(-1);    
  if(pCapture ==NULL)  
  {  
        printf("Get Capture false!\n");  
  }  
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_WIDTH, 1024);//设置图像属性 宽和高  
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_HEIGHT,768);  
  //创建窗口    
  //cvNamedWindow("video", 1);    
  int i = 0;  
  int count = 0;  
  //显示视屏    
  while(count< 10)    
  {    
      pFrame=cvQueryFrame( pCapture );    
      if(!pFrame)  
      {  
          printf("The frame is null!\n");  
          break;   
      }  
      //cvShowImage("video",pFrame);   
      char c=cvWaitKey(10);
      usleep(500000);//500ms
      count++;    
      if(c==27)break;//esc退出
      else
      {
		  img = cvQueryFrame(pCapture);
		  
		  newimg=cvCreateImage(cvSize(1024,768),IPL_DEPTH_8U,3);  
		  cvResize(img,newimg,CV_INTER_LINEAR);
		  
		  char filename[200] = "/home/ubuntu/Pictures/prepared/1-";//tk1-first name the photo
	          char szBuf[256] = {0};  
        	  time_t timer = time(NULL);  
        	  strftime(szBuf, sizeof(szBuf), "%Y-%m-%d-%H:%M:%S", localtime(&timer));
		  strcat(filename,szBuf);
		  solve(filename,i);
		  cout<<cvSaveImage(filename, newimg, 0)<<endl;
		 i++;
	  }    
  }    
  cvReleaseCapture(&pCapture);    
  //cvDestroyWindow("video");  
}
