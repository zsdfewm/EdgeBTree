#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <sys/time.h>
#include "edge_b_tree.h"


//#define EDGE_B_TREE
#define EDGE_MAP

int main() {
#ifdef EDGE_B_TREE
  EdgeBTree *edge_b_tree = new EdgeBTree();
#endif

#ifdef EDGE_MAP
  std::map<locint, double> edge_map;
#endif

  size_t n = 1000;
  locint m = 1000000;
  srand(12345);
  locint i;
  double w;
  struct timeval tv1, tv2;
  gettimeofday(&tv1, NULL);
  for(size_t j=0; j<n; ++j) {
    i=(rand() % m) + 1;
    w=(double)(rand() % m)/m;
#ifdef EDGE_B_TREE
    edge_b_tree->update(i,w);
#endif
#ifdef EDGE_MAP
    edge_map[i]+=w;
#endif
  }
  gettimeofday(&tv2, NULL);
  double time = (tv2.tv_sec-tv1.tv_sec)+ (double)(tv2.tv_usec-tv1.tv_usec)/1000000;
#ifdef EDGE_B_TREE
  printf("EDGE_B_TREE takes %.5f seconds for %d insertions with in range [1, %d]\n", time, n, m);
#endif
#ifdef EDGE_MAP
  printf("Edge_MAP takes %.5f seconds for %d insertrions with in range [1, %d]\n", time, n, m);
#endif
//  edge_b_tree->Print();


#ifdef EDGE_B_TREE
  size_t len;
  locint *ind;
  double *weight;

  edge_b_tree->ToArray(&len, &ind, &weight);
#ifdef EDGE_MAP
  if (len!=edge_map.size()) {
    printf("ERROR LENGTH\n");
  }

  for(size_t i=0;i<len;i++){
//    printf("EdgeBTre[%d]=%.5f, ", ind[i], weight[i]);
//    printf("edge_map[%d]=%.5f\n", ind[i], edge_map[ind[i]]);
    if (weight[i] != edge_map[ind[i]]) {
      printf("ERROR VALUE\n");
    }
  }
#endif
  free(ind);
  free(weight);
  delete edge_b_tree;
#endif
}
