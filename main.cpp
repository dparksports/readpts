#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/utility.hpp"

using namespace cv;
using namespace std;

int stringToInt( const std::string &line ) {
    stringstream ss(line);
    int result;
    return ss >> result ? result : 0;
}

string type2str(int type) {
    string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch ( depth ) {
        case CV_8U:  r = "8U"; break;
        case CV_8S:  r = "8S"; break;
        case CV_16U: r = "16U"; break;
        case CV_16S: r = "16S"; break;
        case CV_32S: r = "32S"; break;
        case CV_32F: r = "32F"; break;
        case CV_64F: r = "64F"; break;
        default:     r = "User"; break;
    }

    r += "C";
    r += (chans+'0');

    return r;
}

static void print(const string name, const cv::Mat m) {
    std::cout << name << ": ( " << m.size()  <<  " ), type: " <<   m.type() << " = " << std::endl << m << std::endl << std::endl;
    fflush(stdout);
}

void printRow(const string name, const cv::Mat m) {
    std::cout << name << ": (" << m.size()  <<  "), type:" <<   type2str(m.type()) << " = " << std::endl << m.row(0) << std::endl << std::endl;
    fflush(stdout);
}

static void saveXYZ(const char* filename, const Mat& mat)
{
    const double max_z = 1.0e4;
    FILE* fp = fopen(filename, "wt");
    for(int y = 0; y < mat.rows; y++)
    {
        for(int x = 0; x < mat.cols; x++)
        {
            Vec3f point = mat.at<Vec3f>(y, x);
//            if(fabs(point[2] - max_z) < FLT_EPSILON || fabs(point[2]) > max_z) continue;
            fprintf(fp, "%f %f %f\n", point[0], point[1], point[2]);
        }
    }
    fclose(fp);
}

static void normalizeSaveXYZ(const char* filename, const Mat& mat, const Vec3f& meanVector)
{
    bool printed = false;
    cout << meanVector << std::endl;

    const double max_z = 1.0e4;
    FILE* fp = fopen(filename, "wt");
    for(int y = 0; y < mat.rows; y++)
    {
        for(int x = 0; x < mat.cols; x++)
        {
            Vec3f point = mat.at<Vec3f>(y, x);
            cout << point << std::endl;
            if (! printed) {
            }

            point[0] -= meanVector[0];
            point[1] -= meanVector[1];
            point[2] -= meanVector[2];
            if (! printed) {
                printed = true;
                cout << point << std::endl;
            }

            fprintf(fp, "%f %f %f\n", point[0], point[1], point[2]);
        }
    }
    fclose(fp);
}


int main() {
    int printmax = 10;
    bool printed = false;
    int xres, yres;
    cv::Mat xyz;

    int maxpoints = 0;
    int numpoint = 0;
    float sumx = 0,sumy = 0,sumz = 0;
    std::string filename = "output.pts";
    string line;
    ifstream myfile (filename);
    if (myfile.is_open()) {
        getline (myfile,line);
        xres = stringToInt(line);

        getline (myfile,line);
        yres = stringToInt(line);
        maxpoints = xres * yres;

        while ( getline (myfile,line) ){
            stringstream linestream(line);
            string xs, ys, zs, ws;
            getline(linestream, xs, ' ');
            getline(linestream, ys, ' ');
            getline(linestream, zs, ' ');
            getline(linestream, ws);

            float x, y, z, w;
            x = std::stof(xs);
            y = std::stof(ys);
            z = std::stof(zs);
            w = std::stof(ws);

            if (x == 0 && y == 0 && z == 0) continue;

            sumx += x;
            sumy += y;
            sumz += z;

            cv::Mat insert = (cv::Mat_<float>(1,3)<<x , y , z);
            xyz.push_back(insert);
            numpoint++;

            if (numpoint < printmax) {
                print("insert", insert);
                print("xyz", xyz);
            }
        }
        myfile.close();
    }

    saveXYZ("shadow.xyz", xyz);
    printRow("xyz", xyz);

    cv::Vec3f meanVector;
    meanVector[0] = sumx / (float) numpoint;
    meanVector[1] = sumy / (float) numpoint;
    meanVector[2] = sumz / (float) numpoint;
    normalizeSaveXYZ("mean.xyz", xyz, meanVector);
    printRow("xyz", xyz);

    cv:Mat disp8;
    int numberOfDisparities = 112 - 16;
//    xyz.convertTo(disp8, CV_8U, 255/(numberOfDisparities*16.));
    xyz.convertTo(disp8, CV_8U);
    saveXYZ("output8U.xyz", disp8);
    printRow("disp8", disp8);

//    cv::Scalar avg,sdv;
//    cv::meanStdDev(xyz, avg, sdv);
//    sdv.val[0] = sqrt(xyz.cols*xyz.rows*sdv.val[0]*sdv.val[0]);
//
//    cv::Mat image_32f;
//    xyz.convertTo(image_32f,CV_32F,1/sdv.val[0],-avg.val[0]/sdv.val[0]);
//    saveXYZ("normalized.xyz", image_32f);
    return 0;
}
