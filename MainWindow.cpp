#include "MainWindow.h"
#include <iostream>
#include <filesystem>
#include <fstream>

//Constructor
MainWindow::MainWindow(TDT4102::Point position, int width, int height, const std::string& title):
AnimationWindow{position.x, position.y, width, height, title},
quitBtn(TDT4102::Point{QbtnX, QbtnY}, btnWidth, btnHeight, "Quit"),
fileInput(TDT4102::Point{FinpX, FinpY}, FbtnWidth, btnHeight, "CSV file path"),
loadBtn(TDT4102::Point{LbtnX, LbtnY}, btnWidth, btnHeight, "Load"),
minLim{TDT4102::Point{mininpX, mininpY}, btnWidth, btnHeight, "Min. freq."},
maxLim{TDT4102::Point{maxinpX, maxinpY}, btnWidth, btnHeight, "Max. freq."},
dummyArgument{0},
width(width),
height(height)
{
	add(quitBtn);
    quitBtn.setCallback(std::bind(&MainWindow::cb_quit, this));

    add(fileInput);
    add(loadBtn);
    loadBtn.setCallback(std::bind(&MainWindow::cb_loadBtn, this));

    add(minLim);
    add(maxLim);
}

//Quit button code
void MainWindow::cb_quit(){
	close();
}

//Load button code
void MainWindow::cb_loadBtn() {
    std::ifstream inputStream{fileInput.getText()};

    if (!inputStream) { // Sjekker om strømmen ble åpnet
        std::cout << "Could not open file" << std::endl;
        return;
    }
    readCSV(inputStream);
    Matrix<std::complex<double>> transformMatrise = DFT(time.size());
    fourierCoeffisients = transformMatrise * channel1;

    scaleTime();
    
    
    }

// Functions
void MainWindow::readCSV(std::ifstream& inputStream) {
    std::string line;
    std::vector<int> row; //endret for å minske minnebruk pga reallokering i while løkken...
    row.reserve(3);

    int validlines = 0;
    while (std::getline(inputStream, line)) {
        std::stringstream ss(line);
        std::string cell;
        row.clear(); //rensker vektoren for hver iterasjon

        while (std::getline(ss, cell, ',')) {
            try {
                row.push_back(std::stod(cell)); 
            } 
            catch (const std::invalid_argument&) {
                row.clear();
                break;
            }
        }
        if (row.size() >= 2) {
            validlines++;
        }
    }


    //reserves places for the vectors
    time.resize(validlines , 0);
    channel1.resize(validlines , 0);
    channel2.resize(validlines , 0);
    
    //reset inputstream
    inputStream.clear();
    inputStream.seekg(0, std::ios::beg);

    //fills the vectors
    int i = 0;
    while (std::getline(inputStream, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<double> row;

        while (std::getline(ss, cell, ',')){
            try{
                row.push_back(std::stod(cell));
            }
            catch(const std::invalid_argument&){
                row.clear();
                break;
            }
        }

        if (row.size() >= 2) {
            time[i] = row[0];      
            channel1[i] = (row[1]);  
            if (row.size() == 3) {            
                channel2[i] = (row[2]);
            }
            i++;
        }
    }

    //normalices time
    if (time[0] != 0){
        double dt = time[2]-time[1];
        for (auto i = 0; i < time.size() ; i++){
               time[i] = i*dt;
        }
    } 

}
const std::vector<double>& MainWindow::getTime() const {
    return time;
}
const std::vector<double>& MainWindow::getChannel1() const {
    return channel1;
}
const std::vector<double>& MainWindow::getChannel2() const {
    return channel2;
}
const std::vector<std::complex<double>>& MainWindow::getFourierCoeffisients() const{
    return fourierCoeffisients;
}
void MainWindow::fourierTransform(){
    int N = time.size();
    auto transformMatrise = DFT(N);
    fourierCoeffisients = std::move(transformMatrise * channel1);
}
void MainWindow::drawAxes(){
        //Akser for plot 1
        TDT4102::Point BLP1{300 , 1300};
        TDT4102::Point BRP1{2260 , 1300};
        TDT4102::Point TLP1{300 , 840};
        draw_line(BLP1 , BRP1);
        draw_line(BLP1 , TLP1);
    
        for (auto i = 1 ; i < 70 ; i++){
            TDT4102::Point a{300 + i * 28 , 1295};
            TDT4102::Point b{300 + i * 28 , 1305};
            draw_line(a , b);
        }
    
    
        //Akser for plot 2
        TDT4102::Point MLP2{300 , 370};
        TDT4102::Point MRP2{2260 , 370};
        TDT4102::Point TLP2{300 , 140};
        TDT4102::Point BLP2{300 , 600};
        draw_line(MLP2 , MRP2);
        draw_line(BLP2 , TLP2);
    
        for (auto i = 1 ; i < 70 ; i++){
            TDT4102::Point a{300 + i * 28 , 365};
            TDT4102::Point b{300 + i * 28 , 375};
            draw_line(a , b);
        }
}
void MainWindow::plot(){

}
void MainWindow::scaleTime(){
    unsigned int xLength = 1960;
    std::vector<double> dummyvector = time;
    unsigned int heltallIntervall;

    while (xLength % dummyvector.size() != 0) { 
        dummyvector.pop_back();
    }
    heltallIntervall = xLength / dummyvector.size();

    for (auto i = 0; i <= dummyvector.size(); i += heltallIntervall) {
        scaledTimeVector.push_back(dummyvector[i]);
        scaledTimeIndexVector.push_back(i);
    }
}
void MainWindow::scaleChannel(int channel){
    if (channel == 1){
        
    }
    else{

    }
}




