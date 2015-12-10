//Algorithms 2500
//Project 1
//Dynamic Programming
//CPP File

#include "KnapSack.h"
#include "Dynamic.h"
#include <stack>
#include <atomic>

void dynamiciter(const KS_List& A, const int maxw, const int i,atomic_int** sackValues);

int DP_KNAPSACK(const KS_List& A, const int maxw)
{

//Variable to be used later
	int currentW = 0;
	int currentV = 0;
	int sizeOfList = A.getsize();
	int final = 0;
    stack<thread*> tstack;
    thread *mythread;
	
//Creates a 2D Dynamic Array to store the Values with sizeOfList and maxw
	atomic_int **sackValues;
	sackValues = new atomic_int* [maxw+1];
	for (int i = 0; i <= maxw; i++)
	{
		sackValues[i] = new atomic_int[sizeOfList+1];
	}
  
  for(int m = 0; m <= sizeOfList; m++)
  {
    for (int k = 0; k <= maxw; k++)
    {
      sackValues[k][m] = 0;
    }
  }

	
//Goes through the entire 2D array starting at row 1 to set the the max value
	for (int i = 1; i <= sizeOfList; i++)
	{
        mythread = new thread(dynamiciter, maxw, i, sackValues);
        tstack.push(mythread);
	}
    
    while(!tstack.empty())
    {
        tstack.top() -> join();
        tstack.pop();
    }
	
  final = sackValues[maxw][sizeOfList];
  
//Delete 2D Dynamic Array
	for (int i = 0; i < maxw; i++)
	{
		delete [] sackValues[i];
	}
	delete [] sackValues;
	
	
	return final;

}

void dynamiciter(const KS_List& A, const int maxw, const int i,atomic_int** sackValues)
{
    for (int j = 0; j <= maxw; j++)
    {
        int currentW = A[i-1] -> getweight();
        int currentV = A[i-1] -> getvalue();
        
        if ( (currentW <= j) && ( (currentV + sackValues[j-currentW][i-1].load()) > sackValues[j][i-1].load() ))
        {
            sackValues[j][i].store(currentV + sackValues[j-currentW][i-1].load());
        }
        else
        {
            sackValues[j][i].store(sackValues[j][i-1].load());
        }
    }
    return;
}
