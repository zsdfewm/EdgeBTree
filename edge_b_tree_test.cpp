#include <stdlib.h>
#include <stdio.h>
#include <map>

#include "edge_b_tree.h"

int main() {
  EdgeBTree *edge_b_tree = new EdgeBTree();
//  std::map<locint, double> edge_map;
  size_t n=5;
  double w;
  for(size_t i=0;i<n;i++) {
    w=-1.0*i;
    edge_b_tree->update(i,w);
//    edge_map[i]+=w;
  }
  edge_b_tree->Print();
/*
  size_t len;
  locint *ind;
  double *weight;

  edge_b_tree->ToArray(&len, &ind, &weight);
  if (len!=n) {
    printf("ERROR LENGTH\n");
  }
  for(size_t i=0;i<len;i++){
    printf("EdgeBTre[%d]=%.5f, ", ind[i], weight[i]);
    printf("edge_map[%d]=%.5f\n", ind[i], edge_map[ind[i]]);
    if (weight[i] != edge_map[ind[i]]) {
      printf("ERROR VALUE\n");
    }
  }

  free(ind);
  free(weight);
*/
  delete edge_b_tree;
}
