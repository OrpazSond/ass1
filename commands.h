

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	// you may add additional methods here

};

// you may add here helper classes

struct data {
    float THRESHOLD;
    std::vector<AnomalyReport> AnomalyReportList;
};

// you may edit this class
class Command{
protected:
    DefaultIO* dio;
public:
    Command(DefaultIO* dio):dio(dio){}
    virtual void execute(data *pData)=0;
    virtual ~Command(){}
};

// implement here your command classes
class Upload:public Command{
public:
    Upload(DefaultIO* dio):Command(dio){}
    void execute(data *d) {
        this->dio->write("Please upload your local train CSV file.\n");
        makeFile("anomalyTrain.csv");
        this->dio->write("Upload complete.\n");
        this->dio->write("Please upload your local test CSV file.\n");
        makeFile("anomalyTest.csv");
        this->dio->write("Upload complete.\n");
    }

    void makeFile(string name){
        std::ofstream myFile(name);
        string line = this->dio->read();
        while (line.compare("done") != 0) {
            myFile<<line;
            myFile<<"\n";
            line = this->dio->read();
        }
    }
};

class Correlation:public Command{
public:
    Correlation(DefaultIO* dio):Command(dio){}
    void execute(data *d){
        this->dio->write("The current correlation threshold is 0.9\n");
        float number =  std::stof(this->dio->read());
        while (number >= 1 || number <= 0){
            this->dio->write("please choose a value between 0 and 1.\n");
            float number =  std::stof(this->dio->read());
        }
        d->THRESHOLD = number;
    }
};


class HybridAlgo:public Command{
public:
    HybridAlgo(DefaultIO* dio):Command(dio){}
    void execute(data *d) {
        TimeSeries tsTrain = TimeSeries("anomalyTrain.csv");
        TimeSeries tsTest = TimeSeries("anomalyTest.csv");
        HybridAnomalyDetector hd = HybridAnomalyDetector();
        hd.setThreshold(d->THRESHOLD);
        hd.learnNormal(tsTrain);
        d->AnomalyReportList = hd.detect(tsTest);
    }
};

class printAnomaly:public Command{
public:
    printAnomaly(DefaultIO* dio):Command(dio){}
    void execute(data *d) {
        int i = 0;
        while(i < d->AnomalyReportList.size()){
            this->dio->write(d->AnomalyReportList[i].timeStep);
            this->dio->write("\t");
            this->dio->write(d->AnomalyReportList[i].description);
            this->dio->write("\n");
            i++;
        }
    }
};

#endif /* COMMANDS_H_ */
