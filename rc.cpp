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


int main(int argc, char *argv[])
{
    int counter = 0;
    double t, amp, freq, phase, dc, speriod, shperiod, sfreq, volt, volth;
    vector<double> voltinVect, timeVect, volthVect;
    string ask;
    wave inputs[5];
    while(true)
    {
      cout << "Wave "<< counter+1 << endl;
      cout << "Amplitude: ";
      cin >> inputs[counter].amp;
      cout << "Frequency: ";
      cin >> inputs[counter].freq;
      cout << "Phase: ";
      cin >> inputs[counter].phase;
      cout << "DC Offset: ";
      cin >> inputs[counter].dc;
      cout << "Waveform: \n(a) sin\n(b) square\n(c) triangle\n";
      cin >> inputs[counter].form;

      cout << "Inputted Amplitude: " << inputs[counter].amp << endl;
      cout << "Inputted Frequency: " << inputs[counter].freq << endl;
      cout << "Inputted Phase: " << inputs[counter].phase << endl;
      cout << "Inputted DC Offset: " << inputs[counter].dc << endl;
      cout << "Inputted Waveform: " << inputs[counter].form << endl;

      while(true)
      {
        cout << "Input another wave? (yes/no)\n";
        cin >> ask;
        if (ask != "yes" && ask != "no")
          cout << "Invalid Input\n";
        else
          break;
      };
      if (ask == "no")
        break;
      counter++;
    };
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
      cout <<s <<endl <<volt <<endl << s+shperiod <<endl << volth <<endl <<"--" << endl;
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
    cout << "Enter R";
    double R, C;
    cin >> R;
    cout << "Enter C";
    cin >> C;
    double ReCa = R*C;
    vector<double> voltoutVect, chargeVect;
    chargeVect.push_back(0);
    double K1, K2, K3, K4, a1, a2, a3, a4, weight, chargenext, sixperiod;
    sixperiod = speriod/6;
    for (int iter=0; iter < voltinVect.size(); iter++)
    {
      K1 = (voltinVect[iter]/R) - (chargeVect[iter]/ ReCa);
      cout << "K1 = " << K1 <<endl; 
 //     system("PAUSE");
      a1 = chargeVect[iter] + K1*shperiod;
      cout << "a1 = " << a1 <<endl; 
 //     system("PAUSE");
      K2 = volthVect[iter]/R - a1 / ReCa;
      cout << "K2 = " << K2 <<endl; 
//      system("PAUSE");
      a2 = chargeVect[iter] + K2* shperiod;
      cout << "a2 = " << a2 <<endl; 
  //    system("PAUSE");
      K3 = volthVect[iter]/R - chargeVect[iter]/ ReCa;
      cout << "K3 = " << K3 << endl; 
  //    system("PAUSE");
      a3 = chargeVect[iter] + K3*speriod;
      cout << "a3 = " << a3 <<endl; 
    //  system("PAUSE");
      K4 = voltinVect[iter+1]/R - a3/ ReCa;
      cout << "K4 = " << K4 <<endl; 
      //system("PAUSE");
      weight = K1 + 2*K2 + 2*K3 + K4;
      chargenext = chargeVect[iter] + weight * sixperiod;
      cout << "CHARGE NEXT = " <<chargenext <<endl;
  //    system("PAUSE");
      cout << "Vo = " <<K1*R <<endl;
 //     system("PAUSE");
      chargeVect.push_back(chargenext);
      voltoutVect.push_back(K1*R);
    }
    
    for (int i=0; i<voltinVect.size(); i++)
    {
      cout << "-------------" << endl;
      cout << timeVect[i] << endl;
      cout << voltinVect[i] << endl;
      cout << voltoutVect[i] << endl;
    }
    
    ofstream saveFile ("Results.csv");
    
    for (int i=0; i< voltinVect.size(); i++)
    {
      saveFile << timeVect[i] <<"," <<voltinVect[i] <<"," <<voltoutVect[i] << endl;
    }
    
    
    system("PAUSE");
    return EXIT_SUCCESS;
}
