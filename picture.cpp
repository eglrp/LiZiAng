//在出现窗口的条件下拍照，延时有问题
#include "iostream"    
#include <cv.h>  
#include <cxcore.h>    
#include <highgui.h>   
#include <opencv2/core/core_c.h>  
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <stdio.h>  
#include <errno.h>  
#include <sys/stat.h>  
#include <fcntl.h>   /* 文件控制定义*/  
#include <termios.h> /* PPSIX 终端控制定义*/  
#include <stdlib.h>   
#include <sys/types.h>   

using namespace std;
#define GPS_LEN 1024 
typedef unsigned int UINT;  
typedef int BYTE; 
typedef struct __gprmc__  
{  
   UINT time;/* gps定位时间 */  
   char pos_state;/*gps状态位*/  
   float latitude;/*纬度 */  
   float longitude;/* 经度 */  
   float speed; /* 速度 */  
   float direction;/*航向 */  
   UINT date;  /*日期  */  
   float declination; /* 磁偏角 */  
   char dd;  
   char mode;/* GPS模式位 */  
  
}GPRMC;//gps类
char t_str[256] = "";

int set_serial(int fd,int nSpeed,int nBits,char nEvent,int nStop)//读取数据  
{  
    struct termios newttys1,oldttys1;  
  
     /*保存原有串口配置*/  
     if(tcgetattr(fd,&oldttys1)!=0)  
     {  
          perror("Setupserial 1");  
          return -1;  
     }  
     memset(&newttys1,0,sizeof(newttys1));/* 先将新串口配置清0 */  
     newttys1.c_cflag|=(CLOCAL|CREAD ); /* CREAD 开启串行数据接收，CLOCAL并打开本地连接模式 */  
  
     newttys1.c_cflag &=~CSIZE;/* 设置数据位 */  
     /* 数据位选择 */  
     switch(nBits)  
     {  
         case 7:  
             newttys1.c_cflag |=CS7;  
             break;  
         case 8:  
             newttys1.c_cflag |=CS8;  
             break;  
     }  
     /* 设置奇偶校验位 */  
     switch( nEvent )  
     {  
         case '0':  /* 奇校验 */  
             newttys1.c_cflag |= PARENB;/* 开启奇偶校验 */  
             newttys1.c_iflag |= (INPCK | ISTRIP);/*INPCK打开输入奇偶校验；ISTRIP去除字符的第八个比特  */  
             newttys1.c_cflag |= PARODD;/*启用奇校验(默认为偶校验)*/  
             break;  
         case 'E':/*偶校验*/  
             newttys1.c_cflag |= PARENB; /*开启奇偶校验  */  
             newttys1.c_iflag |= ( INPCK | ISTRIP);/*打开输入奇偶校验并去除字符第八个比特*/  
             newttys1.c_cflag &= ~PARODD;/*启用偶校验*/  
             break;  
         case 'N': /*无奇偶校验*/  
             newttys1.c_cflag &= ~PARENB;  
             break;  
     }  
     /* 设置波特率 */  
    switch( nSpeed )  
    {  
        case 2400:  
            cfsetispeed(&newttys1, B2400);  
            cfsetospeed(&newttys1, B2400);  
            break;  
        case 4800:  
            cfsetispeed(&newttys1, B4800);  
            cfsetospeed(&newttys1, B4800);  
            break;  
        case 9600:  
            cfsetispeed(&newttys1, B9600);  
            cfsetospeed(&newttys1, B9600);  
            break;  
        case 115200:  
            cfsetispeed(&newttys1, B115200);  
            cfsetospeed(&newttys1, B115200);  
            break;  
        default:  
            cfsetispeed(&newttys1, B9600);  
            cfsetospeed(&newttys1, B9600);  
            break;  
    }  
     /*设置停止位*/  
    if( nStop == 1)/* 设置停止位；若停止位为1，则清除CSTOPB，若停止位为2，则激活CSTOPB */  
    {  
        newttys1.c_cflag &= ~CSTOPB;/*默认为一位停止位； */  
    }  
    else if( nStop == 2)  
    {  
        newttys1.c_cflag |= CSTOPB;/* CSTOPB表示送两位停止位 */  
    }  
  
    /* 设置最少字符和等待时间，对于接收字符和等待时间没有特别的要求时*/  
    newttys1.c_cc[VTIME] = 0;/* 非规范模式读取时的超时时间；*/  
    newttys1.c_cc[VMIN]  = 0; /* 非规范模式读取时的最小字符数*/  
    tcflush(fd ,TCIFLUSH);/* tcflush清空终端未完成的输入/输出请求及数据；TCIFLUSH表示清空正收到的数据，且不读取出来 */  
  
     /*激活配置使其生效*/  
    if((tcsetattr( fd, TCSANOW,&newttys1))!=0)  
    {  
        perror("com set error");  
        exit(1);  
    }  
  
    return 0;  
}
int gps_analyse (char *buff,GPRMC *gps_data) //分析数据 
{  
    char *ptr=NULL;  
     if(gps_data==NULL)  
      {  
         return -1;  
      }  
      if(strlen(buff)<10)  
      {  
         return -1;  
      }  
/* 如果buff字符串中包含字符"$GPRMC"则将$GPRMC的地址赋值给ptr */  
      if(NULL==(ptr=strstr(buff,"$GPRMC")))  
      {  
         return -1;  
      }  
/* sscanf函数为从字符串输入，意思是将ptr内存单元的值作为输入分别输入到后面的结构体成员 */  
      sscanf(ptr,"$GPRMC,%d.00,%c,%f,N,%f,E,%f,,%d,,,%c*",&(gps_data->time),&(gps_data->pos_state),&(gps_data->latitude),&(gps_data->longitude),&(gps_data->speed),&(gps_data->date),&(gps_data->mode));  
      return 0;  
}  
  
int print_gps (GPRMC *gps_data)  
{  
    printf("============================================================\n");  
    printf("==   GPS state bit : %c  [A:有效状态 V:无效状态]              \n",gps_data->pos_state);  
    printf("==   GPS mode  bit : %c  [A:自主定位 D:差分定位]               \n", gps_data->mode);  
    printf("==   Date : 20%02d-%02d-%02d                                  \n",gps_data->date%100,(gps_data->date%10000)/100,gps_data->date/10000);  
    printf("==   Time : %02d:%02d:%02d                                   \n",(gps_data->time/10000+8)%24,(gps_data->time%10000)/100,gps_data->time%100);  
    printf("==   纬度 : 北纬:%d度%d分%d秒                              \n", ((int)gps_data->latitude) / 100, (int)(gps_data->latitude - ((int)gps_data->latitude / 100 * 100)), (int)(((gps_data->latitude - ((int)gps_data->latitude / 100 * 100)) - ((int)gps_data->latitude - ((int)gps_data->latitude / 100 * 100))) * 60.0));  
    printf("==   经度 : 东经:%d度%d分%d秒                              \n", ((int)gps_data->longitude) / 100, (int)(gps_data->longitude - ((int)gps_data->longitude / 100 * 100)), (int)(((gps_data->longitude - ((int)gps_data->longitude / 100 * 100)) - ((int)gps_data->longitude - ((int)gps_data->longitude / 100 * 100))) * 60.0));  
    printf("============================================================\n\n");  
    
    
    char filename[256] = "/home/ubuntu/Pictures/predictions/1-"; 
    sprintf(t_str,"20%02d-%02d-%02d-%02d:%02d:%02d",gps_data->date%100,(gps_data->date%10000)/100,gps_data->date/10000,(gps_data->time/10000+8)%24,(gps_data->time%10000)/100,gps_data->time%100);
    strcat(filename,t_str);
    strcat(filename,".txt");
    FILE *fp = fopen(filename,"w");

    char szBuf[256] = {0};  
    time_t timer = time(NULL);  
    strftime(szBuf, sizeof(szBuf), "%Y-%m-%d-%H:%M:%S", localtime(&timer));
    fprintf(fp,"%s\n",szBuf);
    
    fprintf(fp,"============================================================\n");  
    fprintf(fp,"==   GPS state bit : %c  [A:有效状态 V:无效状态]              \n",gps_data->pos_state);  
    fprintf(fp,"==   GPS mode  bit : %c  [A:自主定位 D:差分定位]               \n", gps_data->mode);  
    fprintf(fp,"==   Date : 20%02d-%02d-%02d                                  \n",gps_data->date%100,(gps_data->date%10000)/100,gps_data->date/10000);  
    fprintf(fp,"==   Time : %02d:%02d:%02d                                   \n",(gps_data->time/10000+8)%24,(gps_data->time%10000)/100,gps_data->time%100);  
    fprintf(fp,"==   纬度 : 北纬:%d度%d分%d秒                              \n", ((int)gps_data->latitude) / 100, (int)(gps_data->latitude - ((int)gps_data->latitude / 100 * 100)), (int)(((gps_data->latitude - ((int)gps_data->latitude / 100 * 100)) - ((int)gps_data->latitude - ((int)gps_data->latitude / 100 * 100))) * 60.0));  
    fprintf(fp,"==   经度 : 东经:%d度%d分%d秒                              \n", ((int)gps_data->longitude) / 100, (int)(gps_data->longitude - ((int)gps_data->longitude / 100 * 100)), (int)(((gps_data->longitude - ((int)gps_data->longitude / 100 * 100)) - ((int)gps_data->longitude - ((int)gps_data->longitude / 100 * 100))) * 60.0));  
    fprintf(fp,"============================================================\n\n");
    return 0;  
}



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


    int fd=0;  
    int n=0;  
    GPRMC gprmc;  
    char buff[GPS_LEN];  






    // 此处可能需要修改！改为板子上真实的串口名称！‘/dev/ttyACM0’是Jetson TK1测试时GPS模块的串口名称，不保证通用性
    char *dev_name="/dev/ttyACM0";  
    





    if((fd=open(dev_name,O_RDWR|O_NOCTTY|O_NDELAY))<0)  
    {  
            perror("Can't Open the ttyUSB0 Serial Port");  
            return -1;  
    }  
    set_serial( fd,9600,8,'N',1);  
  
    //while(1)  
    {  
       sleep(1);  
       if((n=read(fd,buff,sizeof(buff)))<0)  
        {  
           perror("read error");  
           return -1;  
        }  
        memset(&gprmc, 0 , sizeof(gprmc));  
        gps_analyse(buff,&gprmc);  
        print_gps(&gprmc);  
  
    }  
    close(fd);
  
    
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
	          /*char szBuf[256] = {0};  
        	  time_t timer = time(NULL);  
        	  strftime(szBuf, sizeof(szBuf), "%Y-%m-%d-%H:%M:%S", localtime(&timer));*/
		  strcat(filename,t_str);
		  solve(filename,i);
		  cout<<cvSaveImage(filename, newimg, 0)<<endl;
		 i++;
	  }    
  }    
  cvReleaseCapture(&pCapture);    
  //cvDestroyWindow("video");
}
