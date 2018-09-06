#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <iomanip>
#include "./sorter.h"

using namespace std;

void Merge(double** arr, long l, long m, long r, int sortPar, int NoProperties) {
  long i, j, k;
  long n1 = m - l + 1;
  long n2 =  r - m;

  /* create temporary arrays */
  double** L = new double*[n1];
  for (long ind = 0; ind < n1; ind++) {
    L[ind] = new double[NoProperties];
  }
  double** R = new double*[n2];
  for (long ind = 0; ind < n2; ind++) {
    R[ind] = new double[NoProperties];
  }

  /* Copy data to temporary arrays L[] and R[] */
  for (i = 0; i < n1; i++) {
    for (int index = 0; index < NoProperties; index++) {
      L[i][index] = arr[l + i][index];
    }
  }
  for (j = 0; j < n2; j++) {
    for (int index = 0; index < NoProperties; index++) {
      R[j][index] = arr[m + 1+ j][index];
    }
  }

  /* Merge the temporary arrays back into arr[l..r]*/
  i = 0;  // Initial index of first subarray
  j = 0;  // Initial index of second subarray
  k = l;  // Initial index of merged subarray
  while (i < n1 && j < n2) {
    if(sortPar == 21) {
      if (L[i][sortPar] >= R[j][sortPar]) {
        for (int index = 0; index < NoProperties; index++) {
          arr[k][index] = L[i][index];
        }
        i++;
      } else {
        for (int index = 0; index < NoProperties; index++) {
          arr[k][index] = R[j][index];
        }
        j++;
      }
      k++;
    } else {
      if (L[i][sortPar] <= R[j][sortPar]) {
        for (int index = 0; index < NoProperties; index++) {
          arr[k][index] = L[i][index];
        }
        i++;
      } else {
        for (int index = 0; index < NoProperties; index++) {
          arr[k][index] = R[j][index];
        }
        j++;
      }
      k++;
    }
  }

  /* Copy the remaining elements of L[], if there
     are any */
  while (i < n1) {
    for (int index = 0; index < NoProperties; index++) {
      arr[k][index] = L[i][index];
    }
    i++;
    k++;
  }

  /* Copy the remaining elements of R[], if there
     are any */
  while (j < n2) {
    for (int index = 0; index < NoProperties; index++) {
      arr[k][index] = R[j][index];
    }
    j++;
    k++;
  }

  // Delete temporary arrays to free memory
  for (long ind = 0; ind < n1; ind++) {
    delete[] L[ind];
  }

  for (long ind = 0; ind < n2; ind++) {
    delete[] R[ind];
  }
  delete [] L;
  delete [] R;
}

void Sort(double** arr, long l, long r, int sortPar, int NoProperties) {
  if (l < r) {
    // Same as (l+r)/2, but avoids overflow for
    // large l and h
    long m = l+(r-l)/2;

    // Sort first and second halves
    Sort(arr, l, m, sortPar, NoProperties);
    Sort(arr, m+1, r, sortPar, NoProperties);

    Merge(arr, l, m, r, sortPar, NoProperties);
  }
}
