#include <cstdlib>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <fstream>

using namespace std;

//This structure declares all the functions under wave
struct wave
{
    double amp, freq, phase, dc;
    char form;
    wave(double,double,double,double,char);
    wave();
    double sim(double);
};

wave::wave()
{
  amp = freq = phase = dc = 0;
  char form = 'd';
}

wave::wave(double a, double b, double c, double d, char e)
{
  amp = a;
  freq = b;
  phase = c;
  dc = d;
  form = e;
}

double wave::sim(double t)
{
  double volt;
  if (form == 'a') //Sinusoid Wave Formula
    volt = dc + amp*(sin(2*M_PI*freq*t+phase));
  else if (form == 'b') //Square Wave Formula
  {
    if (sin(2*M_PI*freq*t+phase) >= 0)
      volt = dc + amp;
    else
      volt = dc - amp;
  }
  else if (form == 'c') //Triangle Wave Formula
    volt = dc + ((2*amp)/M_PI)*asin(sin(2*M_PI*freq*t+phase));
  else
    volt = 0;
  return volt;
}

wave inputs[5];
int counter = 0;
double R, C;

//Main UI of the Program That displays all the information
void display()
{
  cout << "Resistance : "<<R <<endl;
  cout << "Capacitance: "<<C <<endl;
  for (int i=0; i<5; i++)
        {
          cout << "----WAVE " <<i+1 <<"----" <<endl;
          cout << "Amplitude  : " <<inputs[i].amp <<endl;
          cout << "Frequency  : " <<inputs[i].freq <<endl;
          cout << "Phase Shift: " <<inputs[i].phase<<endl;
          cout << "DC Offset  : " <<inputs[i].dc <<endl;
          cout << "Wave Form  : " ;
          if (inputs[i].form == 'a')
          {
            cout << "Sinusoid" <<endl;
          }
          else if (inputs[i].form == 'b')
          {
            cout << "Square" <<endl;
          }
          else if (inputs[i].form == 'c')
          {
            cout << "Triangle" <<endl;
          }
          else
          {
            cout << "Uninitialized" <<endl;
          }
        }
}

//Main Menu that deals with asking the user for inputs
void menu()
{
      while(true)
      {
        int flag=0;
        while (flag == 0)
        {
          display();
          cout << "Enter the Wave Number of the wave you want to edit." <<endl;
          cout << "If you want to edit resistance, please input 6. " <<endl;
          cout << "If you want to edit capacitance, please input 7. " <<endl;
          cout << "If you want to start the simulation, please input 8" <<endl;
          cout << "Function: ";
          cin >> counter;
          if (counter <1 || counter >8)
          {
            cout <<"Invalid Input!" <<endl;
          }
          if (counter >=1 && counter <=5) //Edit the values of a specific wave.
          {
            cout << "Editing Wave # " <<counter << endl;
            cout << "Amplitude  : ";
            cin >> inputs[counter-1].amp;
            cout << "Frequency  : ";
            cin >> inputs[counter-1].freq;
            cout << "Phase Shift: ";
            cin >> inputs[counter-1].phase;
            cout << "DC Offset  : ";
            cin >> inputs[counter-1].dc;
            while(true)
            {
              cout << "Waveform  : \n(a) sin\n(b) square\n(c) triangle\n";
              cin >> inputs[counter-1].form;
              if (inputs[counter-1].form < 97 || inputs[counter].form > 99)
                cout << "Invalid Input\n";
              else
                break;
            }
          }
            if (counter == 6) //Edit Resistance Value
              {
                while (true)
                {
                  cout << "Enter the Resistance Value of the Resistor: ";
                  cin >> R;
                  if (R >= 0)
                    break;
                  else
                    cout <<"Resistor's Resistance cannot be negative." <<endl;

                }
              }
            if (counter == 7) //Edit Capacitance Value
              {
                while (true)
                {
                  cout << "Enter the Capacitance Value of the Capacitor: ";
                  cin >> C;
                  if (C >= 0)
                    break;
                  else
                    cout <<"Capacitor's Capacitance cannot be negative." <<endl;

                }
              }
            if (counter == 8)
            {
              cout << "Proceeding to Simulation" << endl;
              return;
            }
            system("CLS");
        }
      }
};

int main(int argc, char *argv[])
{
    double t, amp, freq, phase, dc, speriod, shperiod, sfreq, volt, volth;
    vector<double> voltinVect, timeVect, volthVect;
    menu();
    //Sets the frequency to the highest of all 5 waves.
    sfreq = inputs[0].freq;
    for (int s = 1; s < 5; s++)
    {
      if (inputs[s].freq > sfreq)
      {
        sfreq = inputs[s].freq;
      }
    }
    speriod = (1/sfreq)/16; //Calculates the time for 1/16 of a period
    shperiod = speriod/2; //Calculates the time for 1/32 of a period
    cout << "Simulate for how long? (s) ";
    cin >> t;
    for (double s = 0; s <= t; s+=speriod)
    {
      timeVect.push_back(s);
      volt = 0;
      volth= 0;
      for (int r = 0; r < 5; r++) //Computes the total voltage for time t.
      {
        volt += inputs[r].sim(s);
        volth += inputs[r].sim(s+shperiod); //Computes V_in for t+ 1/2 the step.
      }
      if (fabs(volt) < 1e-13) //Corrects very small numbers to be zero
      {
        volt = 0;
      }
      if (fabs(volth) < 1e-13)//Corrects very small numbers to be zero
      {
        volth = 0;
      }
      //Pushes the value inside the vector.
      voltinVect.push_back(volt);
      volthVect.push_back(volth);

    }

    double ReCa = R*C;
    vector<double> voltoutVect, chargeVect;
    chargeVect.push_back(0);
    double K1, K2, K3, K4, a1, a2, a3, a4, weight, chargenext, sixperiod;
    sixperiod = speriod/6;
    //Actual Runge-Kutta 4th Order process
    for (int iter=0; iter < voltinVect.size(); iter++)
    {
      K1 = (voltinVect[iter]/R) - (chargeVect[iter]/ ReCa);
      a1 = chargeVect[iter] + K1*shperiod;
      K2 = volthVect[iter]/R - a1 / ReCa;
      a2 = chargeVect[iter] + K2* shperiod;
      K3 = volthVect[iter]/R - a2/ ReCa;
      a3 = chargeVect[iter] + K3*speriod;
      K4 = voltinVect[iter+1]/R - a3/ ReCa;
      weight = K1 + 2*K2 + 2*K3 + K4;
      chargenext = chargeVect[iter] + weight * sixperiod;
      chargeVect.push_back(chargenext);
      voltoutVect.push_back(K1*R);
    }
    //saves the data computed to the file
    ofstream saveFile ("Results.csv");

    for (int i=0; i< voltinVect.size(); i++)
    {
      saveFile <<timeVect[i] <<"," <<voltinVect[i] <<","<<voltoutVect[i]<< endl;
    }

    cout << "Simulation Complete. Results can be found in ";
    cout << "\" Results.csv \"." <<endl;
    system("PAUSE");
    return EXIT_SUCCESS;
}

