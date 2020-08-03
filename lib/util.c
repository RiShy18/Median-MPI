
//E: Two integers
//R: NO negative numbers
//O: Argument for Qsort
int cmpfunc(const void *a, const void *b)
{
  return (*(int *)a - *(int *)b);
}

