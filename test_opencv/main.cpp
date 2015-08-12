#include <QCoreApplication>
#include<opencv2/opencv.hpp>


using namespace cv;
using namespace std;

int main()
{
    Mat a;
    a=imread("/home/dongrr/my project/test_opencv/test.jpg");
    imshow("test",a);
    waitKey();
}
