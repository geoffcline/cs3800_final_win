#include "bruteforce.h"
#include <cmath>
#include <stack>

int bestValue;
int bestWeight;
pthread_mutex_t checkmx = PTHREAD_MUTEX_INITIALIZER;

struct bruteinp
{
  int testcase;
  int imaxw;
  const KS_List* testitem;
};

void bruteiter(bruteinp* in1);

int bruteforceKS(const KS_List& item, const int maxw)
{
  
  int n = item.getsize(); //used multiple times so variable for total amount of items
  
  thread *mythread;
  stack<thread*> tstack;
  bestValue=0;     //keeps track of max value obtained by a combo of items
  bestWeight=0;    //same as above except for weight
  bruteinp *build1;
  build1 = new bruteinp;
  build1 -> imaxw = maxw;
  string hold;
  
    
  for(int i=1;i<pow(2,n);i++)
  {
    build1 -> testcase = i;
    build1 -> testitem = &item;
    
    mythread = new thread(bruteiter, build1);
    tstack.push(mythread);
  }
  
  while(!tstack.empty())
  {
    tstack.top() -> join();
    //cout << "thread joined" << tstack.top() -> get_id() << endl;
    delete tstack.top();
    tstack.pop();
    
  }
  
  delete build1;
  
  return bestValue; //returns max set

}

void bruteiter(bruteinp* in1)
{
  
  int currentValue=0;     //resets variables so a new combo can be made
  int currentWeight=0;
  const KS_List* item = (in1 -> testitem);
  const int n = item -> getsize();
  const int i = in1 -> testcase;
  
  int* A = new int[n]; //array of binary values to create all combos
  
  for(int x=0;x<n;x++) //sets all elements to 0
  {
    A[x]=0;
  }
  
  //set value of binary in A to i;
  
  for (int j = 0; j < n; ++j)
  {
    A[j] = i & (1 << j) ? 1 : 0;
  }
  
  for(int k =0; k<n;k++) //creates combo out of items and keeps track of weight and value
  {
    if(A[k] == 1 && k < n)
    {
      currentWeight += (item -> getitem(k)) -> getweight();
      currentValue += (item -> getitem(k)) -> getvalue();
    }
    
  }
  
  pthread_mutex_lock( &checkmx );
  if((currentValue > bestValue) && (currentWeight <= in1 -> imaxw)) //replaces max if needed
  {
    bestValue = currentValue;
    bestWeight = currentWeight;
  }
  pthread_mutex_unlock( &checkmx );
  
  delete [] A;
  
  return;
}
