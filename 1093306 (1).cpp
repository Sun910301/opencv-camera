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
    Size image_size=Size(640,480);//�v�H���ؤo
    Size board_size = Size(9, 6);     //�Щw�O�W�C��B�C�����I��
    vector<Point2f> output;  //�֨��C�T�v�H�W�˴��쪺���I
    vector<vector<Point2f>> savepoint; //�x�s�˴��쪺�Ҧ����I
    vector<vector<Point3f>>objpoints;
    /*�������I*/
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
            /*�ȵe����T��*/
            //find4QuadCornerSubpix(view_gray, output, Size(3, 3));//��ʴ��������I�i���T��  
            //drawChessboardCorners(gray, board_size, output, true);//�Ω�b�Ϥ����аO���I  
            savepoint.push_back(output);//�x�s�ȵe�����I  
            //objpoints.push_back(objectpoint);
            // 0("Camera Calibration", view_gray);//��ܹϤ�  
           //  waitKey(500);//���b��
        }
        image_size.width = Input.cols;
        image_size.height = Input.rows;
        Input.release();
    }
    //���~�޼ƪ���
    Mat cam_Mat = Mat(3, 3, CV_32FC1, Scalar::all(0));//K ���۾��x�}�A
    Mat discoef_Mat = Mat(1, 5, CV_32FC1, Scalar::all(0));//D �� Mat[1][5]�����ܫY�Ưx�}
    Mat tMat;//�C�T�v�H������V�q
    Mat rMat;//�C�T�v�H�������V�q
    calibrateCamera(objectpoint, savepoint, image_size, cam_Mat, discoef_Mat, rMat, tMat);//�۾��Щw
    /*�μЩw�����G�B���v�H*/
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
/*�۾��Щw*/
    /*vector<vector<Point3f>> object_points; //�x�s�Щw�O�W���I���T���y��,���Щw�禡���Ĥ@�Ӥ޼�
    Size square_size = Size(20, 20);//��ڴ��q�o�쪺�Щw�O�W�C�ӴѽL�檺�j�p�A�o�̨��S���A�N���w�F�@�ӭȡA�Pı�v�T���O�Ӥj�A�᭱�A��s�U
    for (int t = 0; t < 10; t++)
    {
        vector<Point3f> tempPointSet;
        for (int i = 0; i < board_size.height; i++)
        {
            for (int j = 0; j < board_size.width; j++)
            {
                Point3f realPoint;
                //���]�Щw�O��b�@�ɮy�Шt��z=0�������W
                realPoint.x = i * square_size.width;
                realPoint.y = j * square_size.height;
                realPoint.z = 0;
                tempPointSet.push_back(realPoint);
            }
        }
        object_points.push_back(tempPointSet);
    }*/
