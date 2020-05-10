#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
//#include <opencv2/core/core.hpp>

using namespace std;

int stringToInt( const std::string &line ) {
    stringstream ss(line);
    int result;
    return ss >> result ? result : 0;
}

int stringToFloat( const std::string &line ) {
    stringstream ss(line);
    float result;
    return ss >> result ? result : 0;
}

int main() {
    int xres, yres;
    vector<float> list;

    std::string filename = "../output.pts";
    string line;
    ifstream myfile (filename);
    if (myfile.is_open()) {
        getline (myfile,line);
        xres = stringToInt(line);

        getline (myfile,line);
        yres = stringToInt(line);

        while ( getline (myfile,line) ){
            stringstream linestream(line);
            string x, y, z;
            getline(linestream, x, ' ');
            getline(linestream, y, ' ');
            getline(linestream, z);

            float xf,yf,zf;
            xf = (float)stringToFloat(x);
            yf = (float)stringToFloat(y);
            zf = (float)stringToFloat(z);
            list.push_back(xf);
        }
        myfile.close();
    }

    return 0;
}
