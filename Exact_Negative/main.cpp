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
vector<string> root;


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

        ////debug
//		for(vector<RectObject>::size_type j = 0; j < lrects.size(); j++){
//			rectangle( img,  Point(lrects[j].x, lrects[j].y), Point(lrects[j].x + lrects[j].height, lrects[j].y + lrects[j].width), Scalar( 255, 255, 0 ), 2, 8 );
//		}

//		//获取一张图片的bitset
        bitset<BITLENGTH> lbits;
        getBitsets(lrects, lbits);
        int count = 0;
        int cal = 0;
        while(count < 1000){
//			cal++;
//			if(cal == 1000){
//				break;
//			}
            //生成一个图片尺寸的随机数，大小在30*30 到 img.rows*img.rows
//			int scale = (rand()%(img.rows - 30))+ 30;
            int scale = rand()%150 + 40;
            //生成一个0到(rows - scale)的随机数
            int randx = (rand() % (img.cols - scale));
            int randy = (rand() % (img.rows - scale));
            RectObject ro = RectObject();
            ro.width = scale;
            //height / width = 1 / 1.25
            //ro.height = scale/1.25;
            ro.height = scale;
//			ro.height = ro.width = scale;
            ro.x = randx; ro.y = randy;
            bitset<BITLENGTH> rbits;
            getOneBitset(ro, rbits);



            //1. 获取在标注范围内(有一定的重合面积)的负样本， 负样本完全在标注范围之内的，但是与整个标注范围不能重叠太多。
            //计算随机生成的矩形与所有标注矩形的最大重合面积
            double max_overlap = 0;
            for(vector<RectObject>::size_type j = 0; j < lrects.size(); j++){
                bitset<BITLENGTH> lbits;
                getOneBitset(lrects[j], lbits);
                bitset<BITLENGTH> tbits = lbits & rbits;
                double overlap_rate = ((double)tbits.count()/lbits.count());
                if(max_overlap < overlap_rate){
                    max_overlap = overlap_rate;
                }
            }

//			if(max_overlap <= 0.5 && max_overlap > 0.1){
            if(max_overlap <= 0.5){
                Point top = Point(randx, randy);
                Point bottom = Point(randx + ro.width, randy + ro.height);
//				rectangle( img,  top, bottom, Scalar( 0, 255, 0 ), 2, 8 );

                Rect roi(top, bottom);
                Mat small(img, roi);
                resize(small, small, Size(40, 40));
                stringstream so;
                vector<string> picname;
                u.split(u.trim(items2[0]), ".", picname);
//				cout << outdir << endl;
                if(picname.size() == 2){
                    so << outdir << "/" << items3[items3.size()-1] << "_"<< picname[0] << "_" << count << "." << picname[1];
                    cout << "writing: " << so.str().c_str() << endl;
                    imwrite(so.str(), small);
                    count++;
                }
//				count++;
            }




        //2. 生成与标注范围重合面积不超过一半的图片， 在标注范围之外，同时与标注范围内重合面积在一半以内的图片。

//			bitset<BITLENGTH> tbits = lbits & rbits;
////			double overlap_rate = ((double)tbits.count()/lbits.count());
////			cout << "overlap_rate: " << overlap_rate << endl;
////			if(tbits.count() == 0){
//			if(overlap_rate > 0 && overlap_rate < 0.5){
//				Point top = Point(randx, randy);
//				Point bottom = Point(randx + scale, randy + scale);
//				rectangle( img,  top, bottom, Scalar( 0, 255, 0 ), 2, 8 );
//
//				Rect roi(top, bottom);
//				Mat small(img, roi);
//				resize(small, small, Size(30, 30));
//				stringstream so;
//				vector<string> picname;
//				u.split(u.trim(items2[0]), ".", picname);
////				cout << outdir << endl;
//				if(picname.size() == 2){
//					so << outdir << "/" << "1364612318_"<< picname[0] << "_" << count << "." << picname[1];
//					cout << "writing: " << so.str().c_str() << endl;
//					imwrite(so.str(), small);
//					count++;
//				}
//				count++;
//			}






        }
//		namedWindow("img", 1);
//		imshow( "img", img );
//		waitKey (0);
//		break;
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

//    mkdir("/home/dongrr/data/day/dxz",S_IRWXU);//mkdir bu neng jian li duo ji mu lu,yi ci yi ji
//    mkdir("/home/dongrr/data/day/dxz/1363764871",S_IRWXU);
//    mkdir("/home/dongrr/data/day/dxz/1363765470",S_IRWXU);
//    mkdir("/home/dongrr/data/day/dxz/1364352112",S_IRWXU);
//    mkdir("/home/dongrr/data/day/dxz/1364612843",S_IRWXU);

//    mkdir("/home/dongrr/data/day/hk",S_IRWXU);
//    mkdir("/home/dongrr/data/day/hk/1362384151",S_IRWXU);
//    mkdir("/home/dongrr/data/day/hk/1362909815",S_IRWXU);
//    mkdir("/home/dongrr/data/day/hk/1363323851",S_IRWXU);

//    mkdir("/home/dongrr/data/day/myh",S_IRWXU);
//    mkdir("/home/dongrr/data/day/myh/1362383003",S_IRWXU);
//    mkdir("/home/dongrr/data/day/myh/1362383398",S_IRWXU);
//    mkdir("/home/dongrr/data/day/myh/1362384843",S_IRWXU);
//    mkdir("/home/dongrr/data/day/myh/1364611412",S_IRWXU);

//    mkdir("/home/dongrr/data/day/ssl",S_IRWXU);
//    mkdir("/home/dongrr/data/day/ssl/1363323549",S_IRWXU);
//    mkdir("/home/dongrr/data/day/ssl/1364613142",S_IRWXU);
//    mkdir("/home/dongrr/data/day/ssl/1364787339",S_IRWXU);
//    mkdir("/home/dongrr/data/day/ssl/1364789164",S_IRWXU);
//    mkdir("/home/dongrr/data/day/ssl/1364789466",S_IRWXU);

//    mkdir("/home/dongrr/data/day/xdy",S_IRWXU);
//    mkdir("/home/dongrr/data/day/xdy/1363324153",S_IRWXU);
//    mkdir("/home/dongrr/data/day/xdy/1364525067",S_IRWXU);
//    mkdir("/home/dongrr/data/day/xdy/1364525973",S_IRWXU);
//    mkdir("/home/dongrr/data/day/xdy/1364611110",S_IRWXU);
//    mkdir("/home/dongrr/data/day/xdy/1364787037",S_IRWXU);

//    mkdir("/home/dongrr/data/day/zhaozz",S_IRWXU);
//    mkdir("/home/dongrr/data/day/zhaozz/1362385183",S_IRWXU);
//    mkdir("/home/dongrr/data/day/zhaozz/1363322945",S_IRWXU);
//    mkdir("/home/dongrr/data/day/zhaozz/1363323247",S_IRWXU);
//    mkdir("/home/dongrr/data/day/zhaozz/1365488022",S_IRWXU);
//    mkdir("/home/dongrr/data/day/zhaozz/1365489585",S_IRWXU);

//    mkdir("/home/dongrr/data/day/zx",S_IRWXU);
//    mkdir("/home/dongrr/data/day/zx/1364612318",S_IRWXU);
//    mkdir("/home/dongrr/data/day/zx/1364789768",S_IRWXU);
//    mkdir("/home/dongrr/data/day/zx/1364790372",S_IRWXU);
//    mkdir("/home/dongrr/data/day/zx/1365487719",S_IRWXU);

    for(cnt=0;cnt<30;cnt++){
        root.push_back("/home/dongrr/data/day/dxz/1363764871");
        root.push_back("/home/dongrr/data/day/dxz/1363765470");
        root.push_back("/home/dongrr/data/day/dxz/1364352112");
        root.push_back("/home/dongrr/data/day/dxz/1364612843");

        root.push_back("/home/dongrr/data/day/hk/1362384151");
        root.push_back("/home/dongrr/data/day/hk/1362909815");
        root.push_back("/home/dongrr/data/day/hk/1363323851");

        root.push_back("/home/dongrr/data/day/myh/1362383003");
        root.push_back("/home/dongrr/data/day/myh/1362383398");
        root.push_back("/home/dongrr/data/day/myh/1362384843");
        root.push_back("/home/dongrr/data/day/myh/1364611412");

        root.push_back("/home/dongrr/data/day/ssl/1363323549");
        root.push_back("/home/dongrr/data/day/ssl/1364613142");
        root.push_back("/home/dongrr/data/day/ssl/1364787339");
        root.push_back("/home/dongrr/data/day/ssl/1364789164");
        root.push_back("/home/dongrr/data/day/ssl/1364789466");

        root.push_back("/home/dongrr/data/day/xdy/1363324153");
        root.push_back("/home/dongrr/data/day/xdy/1364525067");
        root.push_back("/home/dongrr/data/day/xdy/1364525973");
        root.push_back("/home/dongrr/data/day/xdy/1364611110");
        root.push_back("/home/dongrr/data/day/xdy/1364787037");

        root.push_back("/home/dongrr/data/day/zhaozz/1362385183");
        root.push_back("/home/dongrr/data/day/zhaozz/1363322945");
        root.push_back("/home/dongrr/data/day/zhaozz/1363323247");
        root.push_back("/home/dongrr/data/day/zhaozz/1365488022");
        root.push_back("/home/dongrr/data/day/zhaozz/1365489585");

        root.push_back("/home/dongrr/data/day/zx/1364612318");
        root.push_back("/home/dongrr/data/day/zx/1364789768");
        root.push_back("/home/dongrr/data/day/zx/1364790372");
        root.push_back("/home/dongrr/data/day/zx/1365487719");

    }

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

    for(i=10;i<30;i++){
    l.start(lbpath[i], picdir[i], root[i]);
    }

}

