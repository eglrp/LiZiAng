# 声明

1. 本程序神经网络部分参照Git：Darknet项目

2. 本程序模型参数与超参数由清华大学自动化系曹仲寻优得到

3. 本程序相关执行脚本由清华大学自动化系李子昂和曹仲分别编写，转载请注明

   ​

# Jetson TK1准备事项：

1. 使用Jetpack套件对Jetson TK1进行刷机，更新Ubuntu系统相关库
2. 从OpenCV下载OpenCV最新版本，在Jetson TK1上对OpenCV进行cmake、make安装
3. 将darknet文件夹放在home/ubuntu/路径下，并授权读写权限

   ​



# darknet文件夹中文件说明：

1. ./lza.sh为开机自启动脚本,需要加入/etc/rc.local中，功能是运行darknet中的Tony.sh

2. ./Tony.sh为主脚本，作用是执行./opencv进行拍照,执行./get_file_list.py和./runtestv2.sh进行识别,再传输到服务器上

3. ./opencv为拍照程序，由1中的opencv.cpp生成，每个板子需要改变其中的命名，以区分不同板子拍摄的图

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

