#include <stdio.h>

void display(int);

int main()
{
  int ageArray[] = {2, 15, 4, 5};
  display(ageArray[0]);

  size_t len = sizeof(ageArray) / sizeof(ageArray[0]);
  printf("Size of the array is %zu\n", len);

  return 0;
}

void display(int age)
{
  printf("%d\n", age);
}
