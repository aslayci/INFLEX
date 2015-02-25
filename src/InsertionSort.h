#ifndef _INSERTIONSORT_H
#define _INSERTIONSORT_H

typedef struct sList {
  struct sList *pNext;  
  double iValue;
  int index; 
} List;

List* SortList(List*);
//typedef struct sList List;

#endif