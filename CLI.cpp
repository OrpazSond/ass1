#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    this->theCommands.push_back(new Upload(this->dio));
    this->theCommands.push_back(new Correlation(this->dio));
    this->theCommands.push_back(new HybridAlgo(this->dio));
    this->theCommands.push_back(new printAnomaly(this->dio));
}

void CLI::start(){
    data d;
    d.THRESHOLD;
    int number = 1;
    while (number != 6) {
        this->dio->write("Welcome to the Anomaly Detection Server.\n"
                         "Please choose an option:\n"
                         "1. upload a time series csv file\n"
                         "2. algorithm settings\n"
                         "3. detect anomalies\n"
                         "4. display results\n"
                         "5. upload anomalies and analyze results\n"
                         "6. exit\n");
        number = std::stoi(this->dio->read());
        if (number >= 1 && number <= 5) {
            this->theCommands[number - 1]->execute(&d);
        }
        else{
            return;
        }
    }
}


CLI::~CLI() {
}

