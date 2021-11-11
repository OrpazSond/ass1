#include <fstream>
#include <vector>
//#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;

class TimeSeries {
private:
    vector<pair<string, vector<float>>> columns;


public:

    TimeSeries(const char *CSVfileName) {


        string line;

        std::ifstream myFile(CSVfileName);

        if (myFile.is_open()) {
            getline(myFile, line, '\r');
            std::istringstream ss(line);
            std::string token;
            while (std::getline(ss, token, ',')) {
                vector<float> v1;
                columns.push_back(make_pair(token, v1));
            }

            while (std::getline(myFile, line, '\r')) {
                std::istringstream ss(line);
                std::string token;
                int index = 0;
                while (std::getline(ss, token, ',')) {
                    float num = std::stof(token);
                    columns[index].second.push_back(num);
                }
            }



            /*else{
                for(int i = 0; i < columns.size(); i++){
                    v1.push_back();

                }
                columns[j].second = v1;
            }
        }*/

        }
        myFile.close();
    }

    vector<pair<string, vector<float>>> getVector() {
        return columns;
    };
};
#endif /* TIMESERIES_H_ */
