#include <fstream>
#include <vector>
//#include <string>

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;

class TimeSeries{

public:

    TimeSeries(const char* CSVfileName){
        string line;
        ifstream myFile (CSVfileName);
        vector<(pair<(string), (vector<float)>)> columns;
        int flag = 1;
        if (myFile.is_open())
        {
            while ( getline (myFile,line) )
            {
                std::istringstream ss(line);
                std::string token
                while(std::getline(ss, token, ',')) {
                    if (flag == 1) {
                        vector<float> v1
                        columns.push_bac(make_pair(token, v1));

                    }
                    else{
                        for(int i = 0, i < columns.size(), i++){
                            columns[i].second = token;
                        }
                    }
                }
                flag = 0;

            }
            myFile.close();
        }
    }
};



#endif /* TIMESERIES_H_ */
