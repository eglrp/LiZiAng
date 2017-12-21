# GPS模块

1. GPS文件夹中是GPS模块调试程序
2. 修改/GPS/main.c中第38行，dev_name的值修改为GPS模块串口的绝对路径文件，如/dev/tty.usbmodem1421, 该文件可以通过在/dev目录下插拔USB串口线来对比找到
3. 在/GPS 文件夹中使用 make 命令，得到gps_test程序，然后./gps_test即可运行, 显示如/GPS/processedGPS.jpg：

![Image text](https://raw.githubusercontent.com/yaoygTony/LiZiAng/master/GPS/processedGPS.jpg)

4. 为了方便地获得GPS模块的原始信息，在/dev文件夹下运行： screen 串口名称 ，如， screen tty.usbmodem1421 可以直接查看，显示如/GPS/rawGPS.jpg：

![Image text](https://raw.githubusercontent.com/yaoygTony/LiZiAng/master/GPS/rawGPS.jpg)


   ​

# 声明

1. 本程序神经网络部分参照Git：Darknet项目
2. 本程序模型参数与超参数由清华大学自动化系曹仲寻优得到
3. 本程序相关执行脚本由清华大学自动化系李子昂和曹仲分别编写，转载请注明

   ​

# Jetson TK1准备事项：

1. 使用Jetpack套件对Jetson TK1进行刷机，更新Ubuntu系统相关库
2. 从OpenCV下载OpenCV最新版本，在Jetson TK1上对OpenCV进行cmake、make安装
3. 将曹仲编写好的darknet文件夹放在home/ubuntu/路径下，并授权读写权限
4. 将李子昂编写好的lza.sh,Tony.sh和opencv从该git上clone，并放在darknet文件夹中

   ​

# darknet文件夹中文件说明：

1. ./lza.sh为开机自启动脚本,需要加入/etc/rc.local中，功能是运行darknet中的Tony.sh
2. ./Tony.sh为主脚本，作用是执行./opencv进行拍照,执行./get_file_list.py和./runtestv2.sh进行识别,再传输到服务器上
3. ./opencv为拍照程序，由src4opencv中的opencv.cpp生成，每个板子需要改变其中的命名，以区分不同板子拍摄的图。当前./opencv中所有路径、命名参数在程序中写死，还未给出用户使用接口
4. ./get_file_list.py读取指定目录下拍好的所有照片， ./runtestv2.sh调用模型对照片进行识别

   ​

# 基于Jetson TK1的程序使用方式：

1. 在Jetson TK1启动前保证网络环境，保证可以连接到服务器上
2. 将摄像头使用USB方式连接到Jetson TK1上
3. 开机

   ​

# 程序结果：

1. 开机后便开始执行程序
2. 图片每1秒拍摄两张，累计拍10张后会将10张一起送进神经网络，神经网络处理速度约为5秒一张，10张照片处理结果会被一齐发送到服务器
3. 服务器收到10张结果后，Jetson TK1会将板子本地上的10张照片原图和10张处理结果删掉，腾出空间
4. 回到到程序结果2，循环执行程序 
