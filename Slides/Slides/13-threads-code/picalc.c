// This version demonstrates use of the thread safe rand_r() function
// for random number generation. It is not needed as there is only one
// thread but shows the interface of the function.
// 
// Sample program to estimate PI = 3.14159... by randomly generating
// points in the positive quadrant and testing whether they are
// distance <= 1.0 from the origin.  The ratio of "hits" to tries is
// an approximation for the area of the quarter circle with radius 1
// so multiplying the ratio by 4 estimates pi.
// 
// usage: picalc <num_samples> 
//   num_samples: int, how many sample points to try, higher gets closer to pi

#include <stdio.h>
#include <stdlib.h> 

int main(int argc, char **argv) { 
  if(argc < 2){
    printf("usage: %s <num_samples>\n",argv[0]);
    printf("  num_samples: int, how many sample points to try, higher gets closer to pi\n");
    return 1;
  }

  unsigned int rstate = 123456789; // seed state for random num generator

  int npoints = atoi(argv[1]);
  int total_hits=0;

  for (int i = 0; i < npoints; i++) { 
    double x = ((double) rand_r(&rstate)) / ((double) RAND_MAX);
    double y = ((double) rand_r(&rstate)) / ((double) RAND_MAX);
    if (x*x + y*y <= 1.0){
      total_hits++;
    }
  } 

  double pi_est = ((double)total_hits) / npoints * 4.0;
  printf("npoints: %8d\n",npoints);
  printf("hits:    %8d\n",total_hits);
  printf("pi_est:  %f\n",pi_est);
  return 0;
}
