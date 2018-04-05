#include <cstdlib>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <fstream>

using namespace std;

class wave
{
  public:
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
  if (form == 'a')
    volt = dc + amp*(sin(2*M_PI*freq*t+phase));
  else if (form == 'b')
  {
    if (sin(2*M_PI*freq*t+phase) >= 0)
      volt = dc + amp;
    else
      volt = dc - amp;
  }
  else if (form == 'c')
    volt = dc + ((2*amp)/M_PI)*asin(sin(2*M_PI*freq*t+phase));
  else
    volt = 0;
  return volt;
}

wave inputs[5];
int counter = 0;
double R, C;

void display()
{
  cout << "Resistance : "<<R <<endl;
  cout << "Capacitance: "<<C <<endl;
  for (int i=0; i<5; i++)
        {
          cout << "----WAVE " <<i+1 <<"----" <<endl;
          cout << "Amplitude: " <<inputs[i].amp <<endl;
          cout << "Frequency: " <<inputs[i].freq <<endl;
          cout << "Phase    : " <<inputs[i].phase<<endl;
          cout << "DC Offset: " <<inputs[i].dc <<endl;
          cout << "Wave Form: " ;
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

void menu()
{
  string ask = "yes";
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
          if (counter >=1 && counter <=5)
          {
            cout << "Editting Wave # " <<counter << endl;
            cout << "Amplitude: ";
            cin >> inputs[counter-1].amp;
            cout << "Frequency: ";
            cin >> inputs[counter-1].freq;
            cout << "Phase: ";
            cin >> inputs[counter-1].phase;
            cout << "DC Offset: ";
            cin >> inputs[counter-1].dc;
            while(true)
            {
              cout << "Waveform: \n(a) sin\n(b) square\n(c) triangle\n";
              cin >> inputs[counter-1].form;
              if (inputs[counter-1].form < 97 || inputs[counter].form > 99)
                cout << "Invalid Input\n";
              else
                break;
            }
          }
            if (counter == 6)
              {
                while (true)
                {
                  cout << "Enter the Resistance Value of the Resistor: ";
                  cin >> R;
                  if (R >= 0)
                    break;
                  else
                    cout <<"Resistances of a Resistor cannot be negative." <<endl;

                }
              }
            if (counter == 7)
              {
                while (true)
                {
                  cout << "Enter the Capacitance Value of the Capacitor: ";
                  cin >> C;
                  if (C >= 0)
                    break;
                  else
                    cout <<"Capacitance of a Capacitor cannot be negative." <<endl;

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
    sfreq = inputs[0].freq;
    for (int s = 1; s < 5; s++)
      if (inputs[s].freq > sfreq)
        sfreq = inputs[s].freq;
    speriod = (1/sfreq)/16;
    shperiod = speriod/2;
    cout << speriod << endl;
    cout << shperiod << endl;
    cout << "For how long? (s) ";
    cin >> t;
    for (double s = 0; s <= t; s+=speriod)
    {
      timeVect.push_back(s);
      volt = 0;
      volth= 0;
      for (int r = 0; r < 5; r++)
      {
        volt += inputs[r].sim(s);
        volth += inputs[r].sim(s+shperiod);
      }
      if (fabs(volt) < 1e-13)
        volt = 0;
      if (fabs(volth) < 1e-13)
        volth = 0;
   //   cout << "Full Time: " << s << endl << volt << endl;
   //   cout << "Half Time: " << s+shperiod << endl << volth <<endl << "--" << endl;
      voltinVect.push_back(volt);
      volthVect.push_back(volth);
     // cout << volth;
    //  system("PAUSE");
    }

    /*for (int i=0; i<voltinVect.size(); i++)
    {
      cout << timeVect[i] << endl;
      cout << voltinVect[i] << endl;
      cout << "--HALF TIME--" << endl;
      cout << volthVect[i] << endl;
      cout << "-------------" << endl;
    }*/
  //  cout << "Enter R";
  //  cin >> R;
  //  cout << "Enter C";
  //  cin >> C;
    double ReCa = R*C;
    vector<double> voltoutVect, chargeVect;
    chargeVect.push_back(0);
    double K1, K2, K3, K4, a1, a2, a3, a4, weight, chargenext, sixperiod;
    sixperiod = speriod/6;
    for (int iter=0; iter < voltinVect.size(); iter++)
    {
      K1 = (voltinVect[iter]/R) - (chargeVect[iter]/ ReCa);
 //     cout << "K1 = " << K1 <<endl;
 //     system("PAUSE");
      a1 = chargeVect[iter] + K1*shperiod;
 //     cout << "a1 = " << a1 <<endl;
 //     system("PAUSE");
      K2 = volthVect[iter]/R - a1 / ReCa;
 //     cout << "K2 = " << K2 <<endl;
//      system("PAUSE");
      a2 = chargeVect[iter] + K2* shperiod;
 //     cout << "a2 = " << a2 <<endl;
  //    system("PAUSE");
      K3 = volthVect[iter]/R - chargeVect[iter]/ ReCa;
//      cout << "K3 = " << K3 << endl;
  //    system("PAUSE");
      a3 = chargeVect[iter] + K3*speriod;
  //    cout << "a3 = " << a3 <<endl;
    //  system("PAUSE");
      K4 = voltinVect[iter+1]/R - a3/ ReCa;
 //     cout << "K4 = " << K4 <<endl;
      //system("PAUSE");
      weight = K1 + 2*K2 + 2*K3 + K4;
      chargenext = chargeVect[iter] + weight * sixperiod;
  //    cout << "CHARGE NEXT = " <<chargenext <<endl;
  //    system("PAUSE");
 //     cout << "Vo = " <<K1*R <<endl;
 //     system("PAUSE");
      chargeVect.push_back(chargenext);
      voltoutVect.push_back(K1*R);
    }

 //   for (int i=0; i<voltinVect.size(); i++)
 //   {
 //     cout << "-------------" << endl;
 //     cout << timeVect[i] << endl;
 //     cout << voltinVect[i] << endl;
 //     cout << voltoutVect[i] << endl;
 //   }

    ofstream saveFile ("Results.csv");

    for (int i=0; i< voltinVect.size(); i++)
    {
      saveFile << timeVect[i] <<"," <<voltinVect[i] <<"," <<voltoutVect[i] << endl;
    }

    cout << "Simulation Complete. Results can be found in \" Results.cvs \"." <<endl;
    system("PAUSE");
    return EXIT_SUCCESS;
}
