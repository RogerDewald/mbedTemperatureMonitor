#include "mbed.h"

InterruptIn switch22(p22);
InterruptIn switch24(p24);

BusOut display(p5,p6,p7,p8,p9,p10,p11,p12);
//BusOut display(p12,p11,p10,p9,p8,p7,p6,p5);

AnalogIn tempProbe(p19);

volatile bool celsius;

int segConvert(int digit) {
    switch(digit){
        case 0: return 0b0111111;
        case 1: return 0b0110000;
        case 2: return 0b1011011;
        case 3: return 0b1001111;
        case 4: return 0b1100110;
        case 5: return 0b1101101;
        case 6: return 0b1111101;
        case 7: return 0b0000111;
        case 8: return 0b1111111;
        case 9: return 0b1100111;
        case '-': return 0b1000000;
        case '.': return 0b10000000;
        default: return 0;
    }
}

void displayInt(int n){
    int q,r;
    if (n<0){
        display = segConvert('-');
        wait(1);
        n=-n;
    }
    if (n<10){
        display = segConvert(n);
    }
    else {
        q = n / 10;
        r = n % 10;
        if (q != 0){
            displayInt(q);
        }
        display = segConvert(r);
    }
    wait(1);
    display = 0;
    wait(0.1);
}

void changeToCelsius (){
    celsius = true;
}

void changeToFahrenheit(){
    celsius = false;
}

int main() {
  switch22.mode(PullDown);
  switch24.mode(PullDown);

  switch22.rise(&changeToCelsius);
  //switch22.fall(changeToCelsius());

  switch24.rise(&changeToFahrenheit);
  //switch24.fall(changeToFahrenheit());

  celsius = true;
  
  while(1) {


      if (celsius == true){
            float meanTemp = 0;
            float volts = 0;

            for (int i = 0; i <1000; i++){

                volts = tempProbe.read();
                meanTemp = meanTemp + (3.3*volts-0.4)/0.0195;

            }
        
            meanTemp = meanTemp/1000;

            if ((meanTemp - (int)meanTemp) < 0.5) {
                displayInt((int)meanTemp);
            }
            else {
                displayInt((int)meanTemp + 1);
            }
            display = 0b00111001;
            wait(1);
            display = 0;
            }

      if (celsius == false){
            float fTemp = 0;
            float meanTemp = 0;
            float volts = 0;

            for (int i = 0; i <1000; i++){

                volts = tempProbe.read();
                meanTemp = meanTemp + (3.3*volts-0.4)/0.0195;

            }

            meanTemp = meanTemp/1000;

            fTemp = (9.0/5)*(meanTemp) + 32;
            if (fTemp - (int)fTemp < 0.5) {
                displayInt((int)fTemp);
            }
            else {
                displayInt((int)fTemp+1);
            }
            
        
            display = 0b01110001;
            wait(1);
            display = 0;
        }




  }
}