

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

struct Anomalyseq{
    int start;
    int end;
    string description;
};


struct data {
    float THRESHOLD;
    std::vector<AnomalyReport> AnomalyReportList;
    std::vector<Anomalyseq> anomalysequences;
    int fileSize;
    data() {
        THRESHOLD = 0.9;
        fileSize = 0;
    }
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
///
class Correlation:public Command{
public:
    Correlation(DefaultIO* dio):Command(dio){}
    void execute(data *d){
        this->dio->write("The current correlation threshold is ");
        this->dio->write(d->THRESHOLD);
        this->dio->write("\nType a new threshold\n");
        float number =  std::stof(this->dio->read());
        while (number >= 1 || number <= 0){
            this->dio->write("please choose a value between 0 and 1.\n");
            float number =  stof(this->dio->read());
        }
        d->THRESHOLD = number;
    }
};


class detect:public Command{
public:
    detect(DefaultIO* dio):Command(dio){}
    void execute(data *d) {
        TimeSeries tsTrain = TimeSeries("anomalyTrain.csv");
        TimeSeries tsTest = TimeSeries("anomalyTest.csv");
        d->fileSize = tsTest.getSize();
        HybridAnomalyDetector hd;
        hd.setThreshold(d->THRESHOLD);
        hd.learnNormal(tsTrain);
        d->AnomalyReportList = hd.detect(tsTest);;
        vector <AnomalyReport> ar = d->AnomalyReportList;
        if(ar.size() == 0){
            return;
        }
        
        // saving the sequences of anomalies into vector "anomalysequences" in struct "data"
        Anomalyseq anomalyseq;
        anomalyseq.start = ar.at(0).timeStep;
        anomalyseq.end = anomalyseq.start;
        anomalyseq.description=ar.at(0).description;

        int anomalyVecLen = ar.size();
        for(size_t i = 1; i<anomalyVecLen; i++){
            AnomalyReport currentRep = ar.at(i);
            if(currentRep.timeStep == anomalyseq.end + 1 && currentRep.description == anomalyseq.description){
                anomalyseq.end ++;
            }
            else {
                d->anomalysequences.push_back(anomalyseq);
                anomalyseq.start = currentRep.timeStep;
                anomalyseq.end = anomalyseq.start;
                anomalyseq.description = currentRep.description;
            }
        }
        d->anomalysequences.push_back(anomalyseq);
        this->dio->write("anomaly detection complete.\n");
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
        this->dio->write("Done\n.");
    }
};

class UplAnm:public Command{
public:
    UplAnm(DefaultIO* dio):Command(dio){}

    bool isTP(int start, int end,data* data){
        for(size_t i=0;i< data->anomalysequences.size();i++){
            Anomalyseq anomalyseq = data->anomalysequences[i];
            if(start <= anomalyseq.end && end >= anomalyseq.start){
                return true;
            }
        }
        return false;
    }

    virtual void execute(data* data){
        dio->write("Please upload your local anomalies file.\n");

        float P=0,TP=0,toSub=0;
        string line = this->dio->read();
        while (line.compare("done") != 0){
            P++;
            size_t t=0;
            while(line[t] != ','){
                t++;
            }
            int startTime = stoi(line.substr(0,t));
            int endTime = stoi(line.substr(t+1,line.length()));
            if(isTP(startTime,endTime,data))
                TP++;
            toSub += endTime - startTime + 1;
            line = this->dio->read();
        }
        dio->write("Upload complete.\n");
        float FP= data->anomalysequences.size() - TP;
        float N= data->fileSize - toSub;
        float truePositiveRate= TP/P;
        float tpr3DigAcc = ((int) (1000 * truePositiveRate))/ 1000.0f;
        float falsePositiveRate = FP/N;
        float fpr3DigAcc = ((int) (1000 * falsePositiveRate))/ 1000.0f;
        dio->write("True Positive Rate: ");
        dio->write(tpr3DigAcc);
        dio->write("\nFalse Positive Rate: ");
        dio->write(fpr3DigAcc);
        dio->write("\n");

    }
};

class Exit:public Command{
public:
    Exit(DefaultIO* dio):Command(dio){}
    virtual void execute(data* data){
    }
};


#endif /* COMMANDS_H_ */
