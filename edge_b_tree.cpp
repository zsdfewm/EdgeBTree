#include <stdlib.h>
#include <stdio.h>
#include "edge_b_tree.h"

#define EdgeBTreeBlockSizeT 2
#define EdgeBTreeBlockSize 5

EdgeBTreeBlock::EdgeBTreeBlock() {
// TODO: Allocate a continguous memory;
  ind = (locint*) malloc(sizeof(locint) * EdgeBTreeBlockSize);
  weight = (double*) malloc(sizeof(double) * EdgeBTreeBlockSize);
  child_block = (EdgeBTreeBlock**) malloc(sizeof(EdgeBTreeBlock*) * (EdgeBTreeBlockSize + 1));
  len = 0;
  is_leaf = false;
}

EdgeBTreeBlock::~EdgeBTreeBlock() {
  free(ind);
  free(weight);
  free(child_block);
}

EdgeBTreeBlock::EraseBlock() {
  int i;
  for(i=0;i<=len;i++) {
    if (chlid_block[i] != NULL && !is_leaf) {
      chlid_block[i]->EraseBlock();
      delete chlid_block[i];
    }
  }
}

EdgeBTree::EdgeBTree() {
  root = new EdgeBTreeBlock();
  root->is_leaf = true;
}

EdgeBTree::~EdgeBTree() {
  root->EraseBlock();
  delete root;
}

void EdgeBTree::update(locint index, double w) {
  double *weight = find(index);
  *weight+=w;
}

void EdgeBTree::erase(locint index) {
  double *weight = find(index);
  *weight = 0.0;
}

double* EdgeBTree::find(locint index) {
  EdgeBTreeBlock *prev_block = NULL;
  size_t prev_ind = 0;
  EdgeBTreeBlock *curr_block = root;

  while () {
    if (curr_block->len == EdgeBTreeBlockSize) {
      //split two blocks;
      EdgeBTreeBlock *new_block = new EdgeBTreeBlock();
      size_t i,j;
      for(i = EdgeBTreeSizeT+1, j=0; i < EdgeBTreeSize; ++i, ++j) {
        new_block->ind[j] = curr_block->ind[i];
        curr_block->ind[i] = 0;
        new_block->weight[j] = curr_block->weight[i];
        curr_block->weight[i] = 0.0;
        new_block->child_block[j] = curr_block->chlid_block[i];
        curr_block->child_block[i] = NULL;
      }
      new_block->chlid_block[EdgeBTreeSizeT] = curr_block->child_block[EdgeBTreeSize];
      curr_block->child_block[EdgeBTreeSize] = NULL;
      new_block->is_leaf = curr_block->is_leaf;

      new_block->len = EdgeBTreeSize;
      curr_block->len = EdgeBTreeSize;

      // put mid to prev block
      locint mid_ind = curr_block->ind[EdgeBTreeSizeT];
      curr_block->ind[EdgeBTreeSizeT] = 0;
      double mid_weight = curr_block->weight[EdgeBTreeSizeT];
      curr_block->weight[EdgeBTreeSizeT] = 0.0;

      if (prev_block = NULL) {
        //Build a new root;
        root = new EdgeBTreeBlock();
        root->ind[0] = mid_ind;
        root->weight[0] = mid_weight;
        root->child_block[0] = curr_block;
        root->chile_block[1] = new_block;
        root->len = 1;
      } else {
        size_t i;
        prev_block->len++;
        for(i = prev_block->len; i > prev_ind; --i) {
          prev_block->ind[i]=prev_block->ind[i-1];
          prev_block->weight[i]=prev_block->weight[i-1];
          prev_block->chlid_block[i+1] = prev_block->child_block[i];
        }
        prev_block->ind[prev_ind] = mid_ind;
        prev_block->weight[prev_ind] = mid_weight;
        prev_block->weight[prev_ind+1] = new_block;
      }
      if (mid_ind == index) {
        return &prev_block->weight[prev_ind];
      } else if (mid_ind < index) {
        curr_block = new_block;
      }
    }
    prev_ind=0;
    while(curr_block->ind[prev_ind] < index && prev_ind < curr_block->len) {
      prev_ind++;
    }
    if (prev_ind != curr_block->len && curr_block->ind[prev_ind] == index) {
      return &(curr_block->weight[prev_ind]);
    }
    if (curr_block->is_leaf) {
      curr_block->len++;
      size_t i;
      for(i = curr_block->len; i > prev_ind; i--) {
        curr_block->ind[i] = curr_block->ind[i-1];
        curr_block->weight[i] = curr_block->weight[i-1];
      }
      curr_block->ind[prev_ind] = index;
      curr_block->weight[prev_ind] = 0.0;
      return &(curr_block->weight[prev_ind]);
    } else {
      prev_block = curr_block;
      curr_block = prev_block->child_block[prev_ind];
    }
  }
}

int main() {
  //test edge_b_tree;

}
