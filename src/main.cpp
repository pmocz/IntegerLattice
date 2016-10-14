#include <ctime>
#include <iostream>

#include "method.h"

/** Main Function */
int main() {

  clock_t clock_begin, clock_end;
  double elapsed_secs;
  
  clock_begin = clock();
  run_simulation();
  clock_end = clock();
  
  elapsed_secs = double(clock_end - clock_begin) / CLOCKS_PER_SEC;
  
  std::cout << "Simulation done! Took " << elapsed_secs << " seconds\n";
  
}
