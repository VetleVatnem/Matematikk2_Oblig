#pragma once
#include <Fourier.h>
#include <MainWindow.h>
#include <cmath>



int main()
{
    MainWindow A(TDT4102::Point(1290,0) , 2580 , 1440 , "DFT");
    while(!A.should_close()) {
        A.drawAxes();
        //A.plot();
        A.next_frame();
    }
    
    
    
    std::vector<std::complex<double>> fouriercoeff = A.getFourierCoeffisients();
    
    unsigned int k = 50; //endre på denne for å skrive ut amplituden til en annen frekvens med formel som er angitt
    unsigned int Sf = A.getTime().size()/A.getTime()[A.getTime().size()-1]; //Samplefrekvens i Hz 
    
    std::cout << "Samplefrekvens[Hz]: " << Sf << std::endl;
    std::cout << "Indeks[k]: " << k << std::endl;
    std::cout << "Frekvens[Hz]: " << (k * Sf)/static_cast<double>(fouriercoeff.size()) << std::endl; 
    std::cout << "Amplitude: " << std::abs(fouriercoeff[k]) << std::endl;
    
    
    return 0;
}
