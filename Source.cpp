/*
#include <iostream>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
int main() {
    // 图像路径
    const string fileName = "C:\\hello_opencv\\hello_opencv\\opencv2.jpg";

    // 读取图像
    Mat origin = imread(fileName);

    // 开辟result存放结果
    Mat result;

    // RGB转换成GRAY
    cvtColor(origin,result,COLOR_BGR2GRAY);

    // 显示原图像与结果
    imshow("Origin Image", origin);
    imshow("Result", result);
    imwrite("C:\\hello_opencv\\hello_opencv\\opencv2_gray.jpg", result);
    waitKey(0);
}
*/

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    Mat image1, image2, image3,image1_gray,image2_gray;
    image1 = imread("C:\\hello_opencv\\hello_opencv\\opencv1_gray.jpg", IMREAD_COLOR);
    image2 = imread("C:\\hello_opencv\\hello_opencv\\opencv2_gray.jpg", IMREAD_COLOR);
  
    //检查两个图的大小与类型
    if (image1.type() != image2.type() || image1.size() != image2.size()) {
        cout << "两图类型或大小不同" << endl;
        return 1;
    }

    //图像的高，行数
    int numberRows = image1.rows;
    //图像的宽，列数
    int numberCols = image1.cols;
    int count = 0;

    while (count <=2) {
        //产生加密文件
        image3.create(numberRows, numberCols, image1.type());
        Mat tFront_image, tHidden_image;
        //要显示的图像
        Mat front_mask(numberRows, numberCols, image1.type(), Scalar(0xF0, 0xF0, 0xF0));
        //要隐藏得图像
        Mat hidden_mask(numberRows, numberCols, image1.type(), Scalar(0xF0, 0xF0, 0xF0));

        //前两图进行位的相加(and)处理，结果放入第三张图
        //因为之前声明资料为0xF0，相加之后只保留前4个位
        bitwise_and(image1, front_mask, tFront_image);
        bitwise_and(image2, hidden_mask, tHidden_image);
        //处理每个颜色通道，将左侧4个位移到右侧
       
        Mat dstimage0, dstimage1, dstimage2, fdstimage0, fdstimage1, fdstimage2;
        switch (count) {
            //前两图进行位的互补(or)处理，结果放入第三张图
            //要隐藏的图就是右侧的四个位       
        case 0:
            for (int j = 0; j < numberRows; j++) {
                for (int i = 0; i < numberCols; i++) {
                    tFront_image.at<Vec3b>(j, i)[0] &= 11111111;
                    tFront_image.at<Vec3b>(j, i)[1] &= 11111111;
                    tFront_image.at<Vec3b>(j, i)[2] &= 11111111;
                }
            }
            resize(tFront_image, fdstimage0, Size(image1.cols / 4, image1.rows / 4), 0, 0, INTER_LINEAR);
            imshow("image0", fdstimage0);
            bitwise_or(tFront_image, tHidden_image, image3);          
            resize(image3, dstimage0, Size(image1.cols / 4, image1.rows / 4), 0, 0, INTER_LINEAR);
             //  imshow("Steged-1 image", dstimage0);
            imwrite("C:\\hello_opencv\\hello_opencv\\staged-0.jpg", dstimage0);
            waitKey(0);
            break;
        case 1:
            for (int j = 0; j < numberRows; j++) {
                for (int i = 0; i < numberCols; i++) {
                    tFront_image.at<Vec3b>(j, i)[0] &= 11111100;
                    tFront_image.at<Vec3b>(j, i)[1] &= 11111100;
                    tFront_image.at<Vec3b>(j, i)[2] &= 11111100;
                }
            }
            resize(tFront_image, fdstimage1, Size(image1.cols / 4, image1.rows / 4), 0, 0, INTER_LINEAR);
            imshow("image1", fdstimage1);
            bitwise_or(tFront_image, tHidden_image, image3);        
            resize(image3, dstimage1, Size(image1.cols / 4, image1.rows / 4), 0, 0, INTER_LINEAR);
          //  imshow("Steged-2 image", dstimage1);
            imwrite("C:\\hello_opencv\\hello_opencv\\staged-1.jpg", dstimage1);
            waitKey(0);
            break;
        case 2:
            for (int j = 0; j < numberRows; j++) {
                for (int i = 0; i < numberCols; i++) {
                    tFront_image.at<Vec3b>(j, i)[0] &= 11111000;
                    tFront_image.at<Vec3b>(j, i)[1] &= 11111000;
                    tFront_image.at<Vec3b>(j, i)[2] &= 11111000;
                }
            }
            resize(tFront_image, fdstimage2, Size(image1.cols / 4, image1.rows / 4), 0, 0, INTER_LINEAR);
            imshow("image2", fdstimage2);
            bitwise_or(tFront_image, tHidden_image, image3);
            resize(image3, dstimage2, Size(image1.cols / 4, image1.rows / 4), 0, 0, INTER_LINEAR);
          //  imshow("Steged-3 image", dstimage3);
            imwrite("C:\\hello_opencv\\hello_opencv\\staged-2.jpg", dstimage2);
            waitKey(0);
            break;
        default :
                cout << "no match";
        }
        
        if (count == 0) {
            Mat image_o1, image_h1;
            image_h1 = imread("C:\\hello_opencv\\hello_opencv\\staged-0.jpg");
            int numberRows = image_h1.rows;
            //图像的宽，列数
            int numberCols = image_h1.cols;
            //产生解密文件
            image_o1.create(numberRows, numberCols, image_h1.type());
            Mat tHidden_image;
            Mat hidden_mast(numberRows, numberCols, image_h1.type(), Scalar(0x0F, 0x0F, 0x0F));
            bitwise_and(image_h1, hidden_mast, image_o1);
            //换原加密处理
            for (int j = 0; j < numberRows; j++) {
                for (int i = 0; i < numberCols; i++) {
                    image_o1.at<Vec3b>(j, i)[0] = image_o1.at<Vec3b>(j, i)[0] << 8;
                    image_o1.at<Vec3b>(j, i)[1] = image_o1.at<Vec3b>(j, i)[1] << 8;
                    image_o1.at<Vec3b>(j, i)[2] = image_o1.at<Vec3b>(j, i)[2] << 8;
                }
            }
            imshow("Hidden Image", image_o1);
            waitKey(0);

        }
        if (count == 1) {
            Mat image_o2, image_h1;
            image_h1 = imread("C:\\hello_opencv\\hello_opencv\\staged-1.jpg");
            int numberRows = image_h1.rows;
            //图像的宽，列数
            int numberCols = image_h1.cols;
            //产生解密文件
            image_o2.create(numberRows, numberCols, image_h1.type());
            Mat tHidden_image;
            Mat hidden_mast(numberRows, numberCols, image_h1.type(), Scalar(0x0F, 0x0F, 0x0F));
            bitwise_and(image_h1, hidden_mast, image_o2);
            //换原加密处理
            for (int j = 0; j < numberRows; j++) {
                for (int i = 0; i < numberCols; i++) {
                    image_o2.at<Vec3b>(j, i)[0] = image_o2.at<Vec3b>(j, i)[0] << 0;
                    image_o2.at<Vec3b>(j, i)[1] = image_o2.at<Vec3b>(j, i)[1] << 0;
                    image_o2.at<Vec3b>(j, i)[2] = image_o2.at<Vec3b>(j, i)[2] << 0;
                }
            }
            imshow("Hidden Image", image_o2);
            waitKey(0);

        }
        if (count == 2) {
            Mat image_o3, image_h1;
            image_h1 = imread("C:\\hello_opencv\\hello_opencv\\staged-2.jpg");
            int numberRows = image_h1.rows;
            //图像的宽，列数
            int numberCols = image_h1.cols;
            //产生解密文件
            image_o3.create(numberRows, numberCols, image_h1.type());
            Mat tHidden_image;
            Mat hidden_mast(numberRows, numberCols, image_h1.type(), Scalar(0x0F, 0x0F, 0x0F));
            bitwise_and(image_h1, hidden_mast, image_o3);
            //换原加密处理
            for (int j = 0; j < numberRows; j++) {
                for (int i = 0; i < numberCols; i++) {
                    image_o3.at<Vec3b>(j, i)[0] = image_o3.at<Vec3b>(j, i)[0] << 0;
                    image_o3.at<Vec3b>(j, i)[1] = image_o3.at<Vec3b>(j, i)[1] << 0;
                    image_o3.at<Vec3b>(j, i)[2] = image_o3.at<Vec3b>(j, i)[2] << 0;
                }
            }
            imshow("Hidden Image", image_o3);
            waitKey(0);

        }
        cout << count;
        count++;
    }
    return 0;
}
/*
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
    Mat image_o, image_h;
    image3 = imread("C:\\hello_opencv\\hello_opencv\\staged-3.jpg");
    //图像的高，行数
    int numberRows = image3.rows;
    //图像的宽，列数
    int numberCols = image3.cols;
    //产生解密文件
    image2.create(numberRows, numberCols, image3.type());
    Mat tHidden_image;
    Mat hidden_mast(numberRows, numberCols, image3.type(), Scalar(0x0F, 0x0F, 0x0F));
    bitwise_and(image3, hidden_mast, image2);
    //换原加密处理
    for (int j = 0; j < numberRows; j++) {
        for (int i = 0; i < numberCols; i++) {
            image2.at<Vec3b>(j, i)[0] = image2.at<Vec3b>(j, i)[0] << 7;
            image2.at<Vec3b>(j, i)[1] = image2.at<Vec3b>(j, i)[1] << 7;
            image2.at<Vec3b>(j, i)[2] = image2.at<Vec3b>(j, i)[2] << 7;
        }
    }
    imshow("Staged Image", image3);
    imshow("Hidden Image", image2);
    waitKey(0);

    return 0;
}

*/
