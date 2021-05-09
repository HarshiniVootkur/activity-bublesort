#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include "cmath"
#include "par_loop.hpp"

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (const int* arr, size_t n);

#ifdef __cplusplus
}
#endif

void swap(int *array, int i, int j);
{
  int temp = array[i];
  array[i] = array[j];
  array[j] = temp;
}

int main (int argc, char* argv[]) {
  if (argc < 3) { std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);

  auto startTime = std::chrono::steady_clock::now();
  
  // get arr data
  int * arr = new int [n];
  generateMergeSortData (arr, n);

  OpenMPLoop o;
  o.nbthread(nbthreads);
  
  bool swappedval = true;
  while(swappedval)
    {
      swappedval = false;
      o.parfor(1,n-1,2,[&](int i) -> void
		       {
			 if(arr[i]>arr[i+1])
			   {
			     swap(arr,i,i+1);
			     swappedval = true;
			   }
		       }
	);

      o.parfor(0,n-1,2,[&](int i) -> void
		       {
			 if(arr[i]>arr[i+1])
			   {
			     swap(arr,i,i+1);
			     swappedval = true;
			   }
		       });
    }

   auto endTime = std::chrono::steady_clock::now();

   std::chrono::duration<double> totalTime = endTime - startTime;
     
  checkMergeSortResult (arr, n);
  std::cerr<<totalTime.count()<<std::endl;
  
  delete[] arr;

  return 0;
}
