#ifndef _OMPLOOP_
#define _OMPLOOP_

#include <functional>

class OpenMPLoop
{
public:
  int nbthreads;

  OpenMPLoop() : nbthreads(1)
  {
  }

  void nbthread(int n)
  {
    nbthreads = n;
  }

  void parfor (size_t beg,size_t end,size_t increment, std::function<void(int)>f)
  {
#pragma omp parallel num_threads(nbthreads)
    {
#pragma omp for schedule(static)
      for (size_t i = beg;i<end;i+=increment)
	{
	  f(i);
	}
    }
  }
};

#endif
  
  
