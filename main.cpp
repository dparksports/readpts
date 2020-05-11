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

double stringToFloat( const std::string &line ) {
    stringstream ss(line);
    double result;
    return ss >> result ? result : 0;
}

static void print(const string name, const cv::Mat m) {
    std::cout << name << ": ( " << m.size()  <<  " ), type: " <<   m.type() << " = " << std::endl << m << std::endl << std::endl;
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

int main() {
    int xres, yres;
    cv::Mat xyz;

    std::string filename = "output.pts";
    string line;
    ifstream myfile (filename);
    if (myfile.is_open()) {
        getline (myfile,line);
        xres = stringToInt(line);

        getline (myfile,line);
        yres = stringToInt(line);

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

            cv::Mat insert = (cv::Mat_<float>(1,3)<<x , y , z);
            xyz.push_back(insert);
            if (x > 0 ) {
//                print(insert);
//                print(xyz);
            }
        }
        myfile.close();
    }

    saveXYZ("output.xyz", xyz);

    return 0;
}
