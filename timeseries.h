#include <fstream>
#include <vector>
//#include <string>
#include <sstream>
#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;

class TimeSeries{

public:

    TimeSeries(const char* CSVfileName){
        string line;
        ifstream myFile (CSVfileName);
        vector<pair<string, vector<float>>> columns;
        int flag = 1;
        if (myFile.is_open())
        {
            getline (myFile,line);
            std::istringstream ss(line);
            std::string token;
            while (std::getline(ss, token, ',')) {
                vector<float> v1;
                columns.push_back(make_pair(token, v1));
            }

            while (std::getline (myFile,line)){
                std::if ss(line);
                std::string token;
                    if (flag == 1) {

                    }
                }


                    /*else{
                        for(int i = 0; i < columns.size(); i++){
                            v1.push_back();

                        }
                        columns[j].second = v1;
                    }
                }*/
                flag = 0;

            }
            myFile.close();
        }
    }
};



#endif /* TIMESERIES_H_ */
