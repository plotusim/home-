# 总结

## ROS 

[官网](wiki.ros.org)

### 通信

***

ROS一共四种通信架构。Topic，Service，Parameter Service，Actionlib.

这是详细的 [介绍文档](https://blog.csdn.net/sru_alo/article/details/100100761) .

#### Topic

[使用自定义消息（C++）](https://blog.csdn.net/weixin_44741023/article/details/90340590)

[消息的发布和订阅](https://blog.csdn.net/u010510350/article/details/72455970)

[msg的使用(其他包调用)](https://blog.csdn.net/u013453604/article/details/72903398) 

[ ROS 自定义消息类型、使用方法以及常见错误解决方案](https://blog.csdn.net/qq_16775293/article/details/80449203)

#### Service

[ ROS编写自己的服务](https://blog.csdn.net/weixin_41869763/article/details/114361876)

***

### 环境和细节

***

[如何编写launch文件](https://blog.csdn.net/wawayu_0/article/details/79414688)

[cmake中的link区别](https://blog.csdn.net/woyebuzhidao888/article/details/44236999)

[ros项目中使用mysql8.0相关配置](https://blog.csdn.net/malingyu/article/details/89214420)

[add_dependencies，add_executable，target_link_libraries](https://blog.csdn.net/weixin_42099082/article/details/99935253)

[openpose在ros环境下的配置](https://blog.csdn.net/qq_23670601/article/details/104346782)

[darknet_ros(YOLO V3)在ros下环境配置](https://blog.csdn.net/qq_42145185/article/details/105730256)

[catkin_make指令说明](https://blog.csdn.net/console_log/article/details/79374052)

[回调函数传参（多个参数）](https://blog.csdn.net/weixin_44139428/article/details/102813167)

[如何让ros支持C++11标准（用于seetaface）](https://blog.csdn.net/qq_34213260/article/details/85019295)

***

## Ubuntu

### 环境配置和安装

***

[CUDA_10.0及cuDNN的安装](https://blog.csdn.net/lihe4151021/article/details/90237681)     [另外一个参考方法](https://blog.csdn.net/wanzhen4330/article/details/81699769)

注意保存路径不要包含中文，不然关闭图形化界面后找不到对应文件。

[Ubuntu vscode配置c/c++环境](https://blog.csdn.net/weixin_44974875/article/details/112521712)

[ubuntu安装opencv的正确方法](https://blog.csdn.net/public669/article/details/99044895)

[ubuntu16.04 openpose编译安装](https://blog.csdn.net/CYummy/article/details/88541396)

[ubuntu 16.04系统下 安装mysql服务，及安装图形化管理界面](https://blog.csdn.net/qq_37638061/article/details/80246799)

[Ubuntu下安装MySQL 以及C++连接MySQL](https://blog.csdn.net/sinat_32596537/article/details/87283931)

[ubuntu安装MySQL Connector/C++ from Source](https://blog.csdn.net/u010235142/article/details/52027256)

[Ubuntu下boost库的编译安装步骤及卸载方法详解](https://blog.csdn.net/yaoxiaokui/article/details/49183909)

不过安装ros的时候boost库已经自动安装。

***

### 其他问题及解决方法

[Ubuntu16.04 将其他磁盘挂载到 /home， 解决/home空间不足](https://blog.csdn.net/weixin_30539835/article/details/96662502)

[Warning: Permanently added 'github.com,192.30.253.113' (RSA) to the list of known hosts.（已解决）](https://blog.csdn.net/qq_41999617/article/details/83691324)

以上还有可能是网不太行

[Linux关闭图形界面之后一直出现iwlwifi 0000:00:14.3: Unhandled alg: 0x707 解决办法](https://blog.csdn.net/yanhanhui1/article/details/104101028)

***

## MySQL

[Mysql Workbench使用教程](https://blog.csdn.net/zs1342084776/article/details/88701261)

[MySQL笔记：包括SQL语法，MySQL管理，C/C++接口编程 ](https://github.com/guodongxiaren/MySQL-docs)

存储人脸特征向量的解决方法：将float特征向量转变成字符串，用空格间隔

```c++
    std::shared_ptr<float> feature1(new float[fr->GetExtractFeatureSize()]); //声明一个共享指针
    fr->Extract(simg, points, feature1.get()); //获取特征向量
    float* feature_t = feature1.get(); //指针指向特征向量（连续存储）
    ...
    for(int i=0;i<size;i++){
        feature = feature + to_string(*feature_t)+" ";
        ++feature_t;
    }//转化为字符串
```

读取方法（用boost按照空格分解）

```c++
    ResultSet *res;  //结果集类
    res = stmt->executeQuery("SELECT name,features FROM data"); //返回名字以及相应的特征集

    string name[50]; //暂定50个人脸数据
    string fes[50]; //暂时存储特征值的str
    int data_size =0;

    while(res->next()){ //读取数据
        name[data_size]=res->getString("name");
        fes[data_size]= res->getString("features");
        data_size++;
    }

    float* features[50]; //人脸特征值数组指针
    for(int j=0;j<data_size;j++){
        vector<string> vec;
        boost::split(vec,fes[j],boost::is_any_of(" "),boost::token_compress_on);//将str存入vector中
        float temp[size];
        for(int i=0;i<size;i++){
            temp[i] = atof(vec[i].c_str());
        }
        float* f = temp; //转化为数组指针
        features[j] = f;
    }

```

## Seetaface

[SeetaFace 入门教程](https://www.cnblogs.com/zhiji6/p/12605648.html)

使用时得熟悉部分C++11特性，并注意在cmake文件声明使用C++11

在ros使用seetaface，mysql时cmake文件配置

```cmake
## seetaface文件所在位置
include_directories(
  /home/tl/sf6/include
)
## 同上
link_directories(/home/tl/sf6/lib64)
link_directories(/usr/lib/x86_64-linux-gnu)
link_directories(/usr/lib)

add_executable(detector src/detector.cpp)

target_link_libraries(detector 
${catkin_LIBRARIES}
${OpenCV_LIBRARIES}
## 以下是使用的模块
libSeetaFaceDetector600.so 
libmysqlclient.so
libmysqlcppconn.so
libSeetaAgePredictor600.so
libSeetaFaceLandmarker600.so
libSeetaFaceRecognizer610.so
libSeetaGenderPredictor600.so
)
```

在c++中的头文件声明例子

```c++
#include "seeta/FaceDetector.h"
#include "seeta/FaceLandmarker.h"
#include "seeta/FaceRecognizer.h"
```

## Darknet_yolo

```ros
roslaunch darknet_ros darknet_ros.launch //使用yolo
```

```ros
roslaunch usb_cam usb_cam-test.launch //使用摄像头
```

修改yolo订阅的topic：修改darknet_ros/config/ros.yaml中的信息

yolo发布的消息定义在darknet_ros_msgs下的msg文件

使用时包含的头文件如下

```C++
#include <darknet_ros_msgs/BoundingBox.h>
#include <darknet_ros_msgs/BoundingBoxes.h>
#include <darknet_ros_msgs/ObjectCount.h>
```

部分camke文件如下

```ros
find_package(catkin REQUIRED COMPONENTS
  darknet_ros_msgs
)
```
