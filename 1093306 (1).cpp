#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2\imgproc\types_c.h>
#include <vector>
#include<iostream>
#include<fstream>
using namespace cv;
using namespace std;
int CHECKERBOARD[2]{ 6,9 };
int main(int argc, char* argv[])
{
    ifstream fin1;
    //fin1.open("test.txt");
    fin1.open(argv[1]);
    vector<string> name;
    //string data[100];
    int numLines = 0;
    if (!fin1) {
        cout << "File could not be open!\n";
    }
    else {
        std::string s;
        while (getline(fin1, s)) {
            cout << s << "\n";
            name.push_back(s);
        }
        fin1.close();
    }
    fin1.close();
    Size image_size=Size(640,480);//影象的尺寸
    Size board_size = Size(9, 6);     //標定板上每行、列的角點數
    vector<Point2f> output;  //快取每幅影象上檢測到的角點
    vector<vector<Point2f>> savepoint; //儲存檢測到的所有角點
    vector<vector<Point3f>>objpoints;
    /*提取角點*/
   // char filename[10]; 
    vector<Point3f>objectpoint;
    for (int i = 0; i < CHECKERBOARD[1]; i++) {
        for (int j = 0; j < CHECKERBOARD[0]; j++) {
            objectpoint.push_back(Point3f(j, i, 0));
        }
    }
    for (int image_num = 0; image_num < 10; image_num++)
    {
        //sprintf_s(filename, "%d_L.jpg", image_num);
        Mat Input = imread(name[image_num]);
        bool ok = findChessboardCorners(Input, board_size, output);
        if (1 == ok)
        {
            Mat gray;
            cvtColor(Input, gray, CV_RGB2GRAY);
            cornerSubPix(gray, output, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 40, 0.01));
            /*亞畫素精確化*/
            //find4QuadCornerSubpix(view_gray, output, Size(3, 3));//對粗提取的角點進行精確化  
            //drawChessboardCorners(gray, board_size, output, true);//用於在圖片中標記角點  
            savepoint.push_back(output);//儲存亞畫素角點  
            //objpoints.push_back(objectpoint);
            // 0("Camera Calibration", view_gray);//顯示圖片  
           //  waitKey(500);//停半秒
        }
        image_size.width = Input.cols;
        image_size.height = Input.rows;
        Input.release();
    }
    //內外引數物件
    Mat cam_Mat = Mat(3, 3, CV_32FC1, Scalar::all(0));//K 為相機矩陣，
    Mat discoef_Mat = Mat(1, 5, CV_32FC1, Scalar::all(0));//D 為 Mat[1][5]的畸變係數矩陣
    Mat tMat;//每幅影象的旋轉向量
    Mat rMat;//每幅影象的平移向量
    calibrateCamera(objectpoint, savepoint, image_size, cam_Mat, discoef_Mat, rMat, tMat);//相機標定
    /*用標定的結果矯正影象*/
    //for (int image_num = 1; image_num <= 10; image_num++)
    //{
        //sprintf_s(filename, "%d_L.jpg", image_num);
        //Mat imageSource = imread(data[image_num]);
        Mat imageSource = imread(argv[2]);
        Mat newimage = imageSource.clone();
        undistort(imageSource, newimage, cam_Mat, discoef_Mat);
        //sprintf_s(filename, "%d_L.jpg", image_num);
        //imwrite("d%d_L.jpg", newimage);
        imwrite(argv[3], newimage);
        imageSource.release();
        newimage.release();

    //}
}
/*相機標定*/
    /*vector<vector<Point3f>> object_points; //儲存標定板上角點的三維座標,為標定函式的第一個引數
    Size square_size = Size(20, 20);//實際測量得到的標定板上每個棋盤格的大小，這裡其實沒測，就假定了一個值，感覺影響不是太大，後面再研究下
    for (int t = 0; t < 10; t++)
    {
        vector<Point3f> tempPointSet;
        for (int i = 0; i < board_size.height; i++)
        {
            for (int j = 0; j < board_size.width; j++)
            {
                Point3f realPoint;
                //假設標定板放在世界座標系中z=0的平面上
                realPoint.x = i * square_size.width;
                realPoint.y = j * square_size.height;
                realPoint.z = 0;
                tempPointSet.push_back(realPoint);
            }
        }
        object_points.push_back(tempPointSet);
    }*/
