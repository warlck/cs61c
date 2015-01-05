#include <stdio.h>
#include <stdlib.h>

int mode(int* arr, int len) {
  int currNum = arr[0];
  int count = 1;
  int mode = currNum;
  int modeCount = 0;
  int i;
  for (i = 1; i < len; i++) {
    if (arr[i] == currNum) {   //commnents I add, increment count by 1 if they match
      count++;
    }
    if (i == len - 1 || arr[i] != currNum) {   //if we have reached last index compare curr count with modecount, if curr beats its
      if (count > modeCount) {   //reset modeCount with new count
        modeCount = count;    //reset mode with currNum returning that this # is mode
        mode = currNum;
      }
      currNum = arr[i];
      count = 1;
    }
  }
  return mode;
}

int main(void) {
  int arr[] = {2, 2, 2, 3, 4, 6, 7, 9, 9, 9, 9};
  printf("%d\n", mode(arr, sizeof(arr) / sizeof(int)));
}


