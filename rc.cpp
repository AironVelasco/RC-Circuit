#include <cstdlib>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>

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
    double t, amp, freq, phase, dc, speriod, sfreq, volt;
    vector<double> voltVect;
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
    cout << speriod << endl;
    cout << "For how long? (s) ";
    cin >> t;
    for (double s = 0; s < t; s+=speriod)
    {
      volt = 0;
      for (int r = 0; r < 5; r++)
        volt += inputs[r].sim(s);
      voltVect.push_back(volt);
    }
    
    for (int i=0; i<voltVect.size(); i++)
      cout << voltVect[i] << endl;
      
    system("PAUSE");
    return EXIT_SUCCESS;
}
