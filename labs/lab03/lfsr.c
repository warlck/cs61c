#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void lfsr_calculate(uint16_t *reg) {

    /* YOUR CODE HERE */
	uint16_t num = (((*reg & 1) ^ ((*reg >> 2) & 1)) ^ ((*reg >> 3) & 1)) ^ ((*reg >> 5) & 1); //11, 13, 14, 16
    //printf("num is %hu\n", num);
    *reg = *reg >> 1;
    //printf("shifted reg is %hu\n", *reg);
    if (num == 1) {
        *reg = *reg | (1 << 15); 
    }
}
//else { *reg = *reg & (0xFFFF - (1 << 15)); }
// printf("set reg is %hu\n", *reg);
//} 

/* static uint16_t reg = 0x1;   //this is the way it has it on project 1-2
 
for (int i = 0; i < 16; i++) {
  uint16_t highest = ((reg >> 0) ^ (reg >> 2) ^ (reg >> 3) ^ (reg >> 5));
  reg = (reg >> 1) | (highest << 15);
}

return reg; */
//}

int main() {
  int8_t *numbers = (int8_t*) malloc(sizeof(int8_t) * 65535);
  if (numbers == NULL) {
    printf("Memory allocation failed!");
    exit(1);
  }

  memset(numbers, 0, sizeof(int8_t) * 65535);
  uint16_t reg = 0x1;
  uint32_t count = 0;
  int i;

  do {
    count++;
    numbers[reg] = 1;
    if (count < 24) {
      printf("My number is: %u\n", reg);
    } else if (count == 24) {
      printf(" ... etc etc ... \n");
    }
    for (i = 0; i < 32; i++)
      lfsr_calculate(&reg);
  } while (numbers[reg] != 1);

  printf("Got %u numbers before cycling!\n", count);

  if (count == 65535) {
    printf("Congratulations! It works!\n");
  } else {
    printf("Did I miss something?\n");
  }

  free(numbers);

  return 0;
}
