#include "Greedy.h"
#include "KnapSack.h"
#include "bruteforce.h"
#include "Dynamic.h"
#include <fstream>
#include <iostream>

using namespace std;

int main ()
{
  const int   DATAPOINTS=8;
  const int   NUMPERPOINT=8;

  const float SIGMA = 10;
  const float MU = 50;
  const int   SIZE = 500;
  const int   MAXW = 5000;
  const int   SIGMAINCREMENT = 2;
  const int   INPUTINCREMENT = 100;
  const int   INITIALINPUTSIZE = 100;
  const int   INTIALSIGMA = 1;
  
  const int   BRUTETHRESHOLD = 31;
  const int   BRUTEMAXW = 1000;
  const int   BRUTEINITIALINPUTSIZE = 10;
  const int   BRUTEINPUTINCREMENT = 2;
  const int   BRUTEMU = 10;
  const int   BRUTESIGMA = 2;
  const int   BRUTESIZE  = 20;
  const int   BRUTESIGMAINCREMENT = 1;
  const int   BRUTEINITIALSIGMA = 1;

  
  clock_t t;
  ofstream fout;
  
  int resultv;
  int totalresultGreedy=0;
  int totalresultDynamic=0;
  int totalresultBrute=0;
  int totalclockGreedy=0;
  int totalclockDynamic=0;
  int totalclockBrute=0;
  int brutetooLarge=0;
  
  fout.open("result.csv");
  fout << "\"C/S\",\"" << CLOCKS_PER_SEC << "\"" << endl;
  
  KS_List BRUTEL_working(BRUTESIGMA, BRUTEMU, BRUTESIZE);
  KS_List L_working(SIGMA,MU,SIZE);
  
 //brute force tests/////////////////////////////////////////////////////////////////////////////

  BRUTEL_working.generate(BRUTEINITIALINPUTSIZE);

  for (int i = 0; i < DATAPOINTS*NUMPERPOINT; ++i) //denpendant->execution independant->inputsize
  {
    BRUTEL_working.generate();

    //BRUTE FORCE
    if(BRUTEL_working.getsize() < BRUTETHRESHOLD)
    {  
      cout << "BRUTE TEST 1: \t" << i << endl;
      t = clock();
      resultv = bruteforceKS(BRUTEL_working, BRUTEMAXW);
      t = clock() - t;
      fout << "\"BRUTE 1\",\"" << i << "\",\"" << t << "\",\"" << resultv << "\"" << endl;
      totalclockBrute+=t;
      totalresultBrute+=resultv;
    }
    else
    {
      cout << "BRUTE SKIP: \t" << i << endl;
      fout << "\"BRUTE 1\",\"" << i << "\",\"Input too large\"" << endl;
      brutetooLarge++;
    }

    
    if(i%NUMPERPOINT==0 && i!=0)
    {
      BRUTEL_working.generate(BRUTEL_working.getsize() + BRUTEINPUTINCREMENT);
  
      totalclockBrute   = totalclockBrute/(NUMPERPOINT- brutetooLarge);
      totalresultBrute  = totalresultBrute/(NUMPERPOINT- brutetooLarge);


      fout << "\"BRUTE DATA POINT 1\",\"" << i << "\",\"" << totalclockBrute << "\",\"" << totalresultBrute << "\"" << endl;
 

      totalclockBrute   = 0;
      totalresultBrute  = 0;
      brutetooLarge     = 0;
    }

  }

  //other tests//////////////////////////
  L_working.generate(INITIALINPUTSIZE);

  for (int i = 0; i < DATAPOINTS*NUMPERPOINT; ++i) //denpendant->execution independant->inputsize
  {
    
    L_working.generate();


    //GREEDY APPROACH
    cout << "GREEDY TEST 1: \t" << i << endl;
    L_working.generate();
    t = clock();
    resultv = GreedyKS(L_working, MAXW);
    t = clock() - t;
    fout << "\"GREEDY 1\",\"" << i << "\",\"" << t << "\",\"" << resultv << "\",\"" << L_working.getweights() << "\"" << endl;
    
    totalclockGreedy+=t;
    totalresultGreedy+=resultv;

    
    //DYNAMIC APPROACH
    cout << "DYNAMIC TEST 1: \t" << i << endl;
    t = clock();
    resultv = DP_KNAPSACK(L_working, MAXW);
    t = clock() - t;
    fout << "\"DYNAMIC 1\",\"" << i << "\",\"" << t << "\",\"" << resultv << "\",\"" << L_working.getweights() << "\"" << endl;
    
    totalclockDynamic+=t;
    totalresultDynamic+=resultv;

    
    if(i%NUMPERPOINT==0 && i!=0)
    {
      L_working.generate(L_working.getsize() + INPUTINCREMENT);
  
      totalclockGreedy  = totalclockGreedy/NUMPERPOINT;
      totalresultGreedy = totalresultGreedy/NUMPERPOINT;
      totalclockDynamic = totalclockDynamic/NUMPERPOINT;
      totalresultDynamic= totalresultDynamic/NUMPERPOINT;

      fout << "\"GREEDY DATA POINT 1\",\"" << i << "\",\"" << totalclockGreedy << "\",\"" << totalresultGreedy << "\"" << endl;
      fout << "\"DYNAMIC DATA POINT 1\",\"" << i << "\",\"" << totalclockDynamic << "\",\"" << totalresultDynamic << "\"" << endl;
      
      totalclockGreedy  = 0;
      totalresultGreedy = 0;
      totalclockDynamic = 0;
      totalresultDynamic= 0;
    }

  }
 


  fout.close();

  return 0;
}