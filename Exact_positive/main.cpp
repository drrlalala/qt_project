/*
 * Launch.cpp
 *
 *  Created on: Dec 3, 2014
 *      Author: lpan
 */

#include "Launch.h"
#include "ReadFromFile.h"
#include "Util.h"
#include "RectObject.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <bitset>

#include <sys/stat.h>

/**
 * 随机生成负样本
 */

using namespace std;
using namespace cv;

ofstream out_stream;
Util u;

vector<char*> lbpath;
vector<char*> picdir;
//vector<string> root;
string root;


Launch::Launch() {
    // TODO Auto-generated constructor stub

}

Launch::~Launch() {
    // TODO Auto-generated destructor stub
}



void Launch::start(char*lbdir, char*picdir, string outdir){
    //读入所有标注行
    vector<string> lblines;
    ReadFromFile rff = ReadFromFile();
    rff.read(lbdir, lblines);
    if(lblines.empty()){
        return;
    }
//	cout << "lines: " << lblines.size() << endl;
//	cout << "outdir: " << outdir << endl;
    //循环标注每一行
    for(vector<string>::size_type i = 0; i < lblines.size(); i++){
        //读入一张图片
        Mat img;
        string s = u.trim(lblines[i]);
        vector<string> items1;
        vector<string> items3;
        u.split(s, "@", items1);
        s = items1[0];
        u.split(s,"\\",items3);//表示/字符
        s = items1[items1.size() - 1];
        vector<string> items2;
        u.split(s, " ", items2);
        stringstream ss;
        ss << picdir << "/" << u.trim(items2[0]);
        cout << "reading: " << ss.str() << endl;

        img = imread(ss.str());
        //获取图片中被标注过的矩形框
        vector<RectObject> lrects;
        getPointRange(items2, lrects);

        for(int i=0;i<lrects.size();i++){

            cout<<lrects[i].x<<" "<<lrects[i].y<<"  "<<lrects[i].width<<"  "<<lrects[i].height<<endl;

            CvRect roi=cvRect(lrects[i].x,lrects[i].y,lrects[i].height,lrects[i].width);

            Mat small_pic;
            img(roi).copyTo(small_pic);
            resize(small_pic,small_pic,Size(40,40));

            stringstream so;
            vector<string> picname;
            u.split(u.trim(items2[0]), ".", picname);

            if(picname.size() == 2){
                so << outdir << "/" << items3[items3.size()-1] << "_"<< picname[0] << "." << picname[1];
                cout << "writing: " << so.str().c_str() << endl;
                imwrite(so.str(), small_pic);
            }
        }
   }
}

/**
 * 获取一张图片中所有矩形所对应的比特集
 * 矩形所框范围都为1，其他位置为0
 */
void Launch::getBitsets(vector<RectObject> &lrects, bitset<BITLENGTH> &bits){
    for(vector<RectObject>::size_type i = 0; i < lrects.size(); i++){
        getOneBitset(lrects[i], bits);
    }
}


/**
 * 获取一个矩形框的比特集
 */
void Launch::getOneBitset(RectObject& rect, bitset<BITLENGTH>& bits){
    int width = rect.width;
    int height = rect.height;
    int x = rect.x;
    int y = rect.y;
    for(int i = x; i < x + height; i++){
        for(int j = y; j < y + width; j++){
            bits.set(j*PIC_WIDTH + i);
        }
    }
}

void Launch::getPointRange(vector<string> &items2,  vector<RectObject> &pr){
    string s;
    for(vector<string>::size_type i = 1; i < items2.size(); i++){
        vector<string> rectPara;
        u.split(items2[i], ",", rectPara);
        if(rectPara.size() == 4){
            RectObject p;
            p.x = atoi( rectPara[0].c_str() );
            p.y = atoi( rectPara[1].c_str() );
            p.height = atoi( rectPara[2].c_str());
            p.width = atoi( rectPara[3].c_str());
            pr.push_back(p);
        }
    }
}

/******************************************************************/
void init(){
    int cnt=0;

    for(cnt=0;cnt<30;cnt++){
        lbpath.push_back("/home/dongrr/data/positive_human_label/dxz/1363764871labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/dxz/1363765470labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/dxz/1364352112labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/dxz/1364612843labels.txt");

        lbpath.push_back("/home/dongrr/data/positive_human_label/hk/1362384151labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/hk/1362909815labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/hk/1363323851labels.txt");

        lbpath.push_back("/home/dongrr/data/positive_human_label/myh/1362383003labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/myh/1362383398labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/myh/1362384843labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/myh/1364611412labels.txt");

        lbpath.push_back("/home/dongrr/data/positive_human_label/ssl/1363323549labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/ssl/1364613142labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/ssl/1364787339labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/ssl/1364789164labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/ssl/1364789466labels.txt");

        lbpath.push_back("/home/dongrr/data/positive_human_label/xdy/1363324153labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/xdy/1364525067labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/xdy/1364525973labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/xdy/1364611110labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/xdy/1364787037labels.txt");

        lbpath.push_back("/home/dongrr/data/positive_human_label/zhaozz/1362385183labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/zhaozz/1363322945labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/zhaozz/1363323247labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/zhaozz/1365488022labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/zhaozz/1365489585labels.txt");

        lbpath.push_back("/home/dongrr/data/positive_human_label/zx/1364612318labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/zx/1364789768labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/zx/1364790372labels.txt");
        lbpath.push_back("/home/dongrr/data/positive_human_label/zx/1365487719labels.txt");
    }

    for(cnt=0;cnt<30;cnt++){
        picdir.push_back("/home/dongrr/data/positive_human_label/dxz/1363764871");
        picdir.push_back("/home/dongrr/data/positive_human_label/dxz/1363765470");
        picdir.push_back("/home/dongrr/data/positive_human_label/dxz/1364352112");
        picdir.push_back("/home/dongrr/data/positive_human_label/dxz/1364612843");

        picdir.push_back("/home/dongrr/data/positive_human_label/hk/1362384151");
        picdir.push_back("/home/dongrr/data/positive_human_label/hk/1362909815");
        picdir.push_back("/home/dongrr/data/positive_human_label/hk/1363323851");

        picdir.push_back("/home/dongrr/data/positive_human_label/myh/1362383003");
        picdir.push_back("/home/dongrr/data/positive_human_label/myh/1362383398");
        picdir.push_back("/home/dongrr/data/positive_human_label/myh/1362384843");
        picdir.push_back("/home/dongrr/data/positive_human_label/myh/1364611412");

        picdir.push_back("/home/dongrr/data/positive_human_label/ssl/1363323549");
        picdir.push_back("/home/dongrr/data/positive_human_label/ssl/1364613142");
        picdir.push_back("/home/dongrr/data/positive_human_label/ssl/1364787339");
        picdir.push_back("/home/dongrr/data/positive_human_label/ssl/1364789164");
        picdir.push_back("/home/dongrr/data/positive_human_label/ssl/1364789466");

        picdir.push_back("/home/dongrr/data/positive_human_label/xdy/1363324153");
        picdir.push_back("/home/dongrr/data/positive_human_label/xdy/1364525067");
        picdir.push_back("/home/dongrr/data/positive_human_label/xdy/1364525973");
        picdir.push_back("/home/dongrr/data/positive_human_label/xdy/1364611110");
        picdir.push_back("/home/dongrr/data/positive_human_label/xdy/1364787037");

        picdir.push_back("/home/dongrr/data/positive_human_label/zhaozz/1362385183");
        picdir.push_back("/home/dongrr/data/positive_human_label/zhaozz/1363322945");
        picdir.push_back("/home/dongrr/data/positive_human_label/zhaozz/1363323247");
        picdir.push_back("/home/dongrr/data/positive_human_label/zhaozz/1365488022");
        picdir.push_back("/home/dongrr/data/positive_human_label/zhaozz/1365489585");

        picdir.push_back("/home/dongrr/data/positive_human_label/zx/1364612318");
        picdir.push_back("/home/dongrr/data/positive_human_label/zx/1364789768");
        picdir.push_back("/home/dongrr/data/positive_human_label/zx/1364790372");
        picdir.push_back("/home/dongrr/data/positive_human_label/zx/1365487719");

    }

//    mkdir("/home/dongrr/data/day_positive/dxz",S_IRWXU);//mkdir bu neng jian li duo ji mu lu,yi ci yi ji
//    mkdir("/home/dongrr/data/day_positive/dxz/1363764871",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/dxz/1363765470",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/dxz/1364352112",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/dxz/1364612843",S_IRWXU);

//    mkdir("/home/dongrr/data/day_positive/hk",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/hk/1362384151",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/hk/1362909815",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/hk/1363323851",S_IRWXU);

//    mkdir("/home/dongrr/data/day_positive/myh",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/myh/1362383003",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/myh/1362383398",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/myh/1362384843",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/myh/1364611412",S_IRWXU);

//    mkdir("/home/dongrr/data/day_positive/ssl",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/ssl/1363323549",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/ssl/1364613142",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/ssl/1364787339",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/ssl/1364789164",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/ssl/1364789466",S_IRWXU);

//    mkdir("/home/dongrr/data/day_positive/xdy",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/xdy/1363324153",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/xdy/1364525067",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/xdy/1364525973",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/xdy/1364611110",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/xdy/1364787037",S_IRWXU);

//    mkdir("/home/dongrr/data/day_positive/zhaozz",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/zhaozz/1362385183",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/zhaozz/1363322945",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/zhaozz/1363323247",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/zhaozz/1365488022",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/zhaozz/1365489585",S_IRWXU);

//    mkdir("/home/dongrr/data/day_positive/zx",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/zx/1364612318",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/zx/1364789768",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/zx/1364790372",S_IRWXU);
//    mkdir("/home/dongrr/data/day_positive/zx/1365487719",S_IRWXU);

    mkdir("/home/dongrr/data/day_positive",S_IRWXU);
    root="/home/dongrr/data/day_positive";

//    for(cnt=0;cnt<30;cnt++){
//        root.push_back("/home/dongrr/data/day/dxz/1363764871");
//        root.push_back("/home/dongrr/data/day/dxz/1363765470");
//        root.push_back("/home/dongrr/data/day/dxz/1364352112");
//        root.push_back("/home/dongrr/data/day/dxz/1364612843");

//        root.push_back("/home/dongrr/data/day/hk/1362384151");
//        root.push_back("/home/dongrr/data/day/hk/1362909815");
//        root.push_back("/home/dongrr/data/day/hk/1363323851");

//        root.push_back("/home/dongrr/data/day/myh/1362383003");
//        root.push_back("/home/dongrr/data/day/myh/1362383398");
//        root.push_back("/home/dongrr/data/day/myh/1362384843");
//        root.push_back("/home/dongrr/data/day/myh/1364611412");

//        root.push_back("/home/dongrr/da大街22号泛利大厦3层

//                       邮编（Postcode）：100027ta/day/ssl/1363323549");
//        root.push_back("/home/dongrr/data/day/ssl/1364613142");
//        root.push_back("/home/dongrr/data/day/ssl/1364787339");
//        root.push_back("/home/dongrr/data/day/ssl/1364789164");
//        root.push_back("/home/dongrr/data/day/ssl/1364789466");

//        root.push_back("/home/dongrr/data/day/xdy/1363324153");
//        root.push_back("/home/dongrr/data/day/xdy/1364525067");
//        root.push_back("/home/dongrr/data/day/xdy/1364525973");
//        root.push_back("/home/dongrr/data/day/xdy/1364611110");
//        root.push_back("/home/dongrr/data/day/xdy/1364787037");

//        root.push_back("/home/dongrr/data/day/zhaozz/1362385183");
//        root.push_back("/home/dongrr/data/day/zhaozz/1363322945");
//        root.push_back("/home/dongrr/data/day/zhaozz/1363323247");
//        root.push_back("/home/dongrr/data/day/zhaozz/1365488022");
//        root.push_back("/home/dongrr/data/day/zhaozz/1365489585");

//        root.push_back("/home/dongrr/data/day/zx/1364612318");
//        root.push_back("/home/dongrr/data/day/zx/1364789768");
//        root.push_back("/home/dongrr/data/day/zx/1364790372");
//        root.push_back("/home/dongrr/data/day/zx/1365487719");

//    }

}
/******************************************************/

int main(){

    // label file
//    char*lbpath = "/home/dongrr/data/正样本人工标注/dxz/1363764871labels.txt";
//    char*picdir = "/home/dongrr/data/trainset_night/unlabeled_pic/dht/1366033472/";
//    string root = "/home/dongrr/data/night/negative";


    init();


    Launch l = Launch();
    int i=0;

    for(i=25;i<30;i++){
    l.start(lbpath[i], picdir[i], root);
    }

}

