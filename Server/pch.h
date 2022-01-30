// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
//#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/videoio/videoio_c.h>
//#include <opencv2/core/core.hpp>
//#include <opencv2/core/cvdef.h>
#pragma comment(lib,"opencv_world451d")
//#pragma comment(lib,"opencv_img_hash451d")


//账户备用
struct SAdmin
{
	char sName[20];
	char sPass[20];
	int nPrior;
};
const int blocksize = 28800;
//定义包结构  
struct videodate//包格式  
{
	char buf[blocksize];//存放数据的变量  
	int flag;//标志          
};

enum {
	WK_SENDTEXT = 0x100,
	WK_SENDVIDEO
};




#endif //PCH_H
