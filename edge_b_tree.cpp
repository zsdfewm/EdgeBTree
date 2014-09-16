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

void EdgeBTreeBlock::EraseBlock() {
  int i;
  for(i=0;i<=len;i++) {
    if (child_block[i] != NULL && !is_leaf) {
      child_block[i]->EraseBlock();
      delete child_block[i];
    }
  }
}
void EdgeBTreeBlock::AppendToArray(size_t *len_array, locint *ind_array, double *weight_array) {
  for(size_t i = 0; i < len; i++) {
    ind_array[*len_array] = ind[i];
    weight_array[*len_array] = weight[i];
    *len_array++;
  }
  if (!is_leaf) {
    for(size_t i = 0; i <= len; i++) {
      child_block[i]->AppendToArray(len_array, ind_array, weight_array);
    }
  }
}

void EdgeBTreeBlock::PrintBlock() {
  if (!is_leaf) {
    for(size_t i = 0; i<=len; i++) {
      child_block[i]->PrintBlock();
    }
  }
  printf("B[%x]:",this);
  for(size_t i = 0; i<len; i++) {
    if (!is_leaf) {
      printf("%x, ", child_block[i]);
    }
    printf("%<%d, %10.5f>, ", ind[i], weight[i]);
  }
  if (!is_leaf) {
    printf("%x", child_block[len]);
  }
  printf("\n");
}

EdgeBTree::EdgeBTree() {
  root_block = new EdgeBTreeBlock();
  root_block->is_leaf = true;
  size = 0;
}

EdgeBTree::~EdgeBTree() {
  root_block->EraseBlock();
  delete root_block;
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
  EdgeBTreeBlock *curr_block = root_block;

  while (true) {
    if (curr_block->len == EdgeBTreeBlockSize) {
      //split two blocks;
      EdgeBTreeBlock *new_block = new EdgeBTreeBlock();
      size_t i,j;
      for(i = EdgeBTreeBlockSizeT+1, j=0; i < EdgeBTreeBlockSize; ++i, ++j) {
        new_block->ind[j] = curr_block->ind[i];
        curr_block->ind[i] = 0;
        new_block->weight[j] = curr_block->weight[i];
        curr_block->weight[i] = 0.0;
        new_block->child_block[j] = curr_block->child_block[i];
        curr_block->child_block[i] = NULL;
      }
      new_block->child_block[EdgeBTreeBlockSizeT] = curr_block->child_block[EdgeBTreeBlockSize];
      curr_block->child_block[EdgeBTreeBlockSize] = NULL;
      new_block->is_leaf = curr_block->is_leaf;

      new_block->len = EdgeBTreeBlockSizeT;
      curr_block->len = EdgeBTreeBlockSizeT;

      // put mid to prev block
      locint mid_ind = curr_block->ind[EdgeBTreeBlockSizeT];
      curr_block->ind[EdgeBTreeBlockSizeT] = 0;
      double mid_weight = curr_block->weight[EdgeBTreeBlockSizeT];
      curr_block->weight[EdgeBTreeBlockSizeT] = 0.0;

      if (prev_block = NULL) {
        //Build a new root;
        root_block = new EdgeBTreeBlock();
        root_block->ind[0] = mid_ind;
        root_block->weight[0] = mid_weight;
        root_block->child_block[0] = curr_block;
        root_block->chile_block[1] = new_block;
        root_block->len = 1;
      } else {
        size_t i;
        prev_block->len++;
        for(i = prev_block->len; i > prev_ind; --i) {
          prev_block->ind[i]=prev_block->ind[i-1];
          prev_block->weight[i]=prev_block->weight[i-1];
          prev_block->child_block[i+1] = prev_block->child_block[i];
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

      this->size++;

      return &(curr_block->weight[prev_ind]);
    } else {
      prev_block = curr_block;
      curr_block = prev_block->child_block[prev_ind];
    }
  }
}

void EdgeBTree::ToArray(size_t *len, locint **ind, double **weight) {

  (*ind) = (locint*) malloc(sizeof(locint) * size);
  (*weight) = (locint*) malloc(sizeof(locint) * size);
  *len=0;
  root->AppendToArray(len, *ind, *weight);
  if (*len != size) {
    fprintf(stderr, "FATAL ERROR: EdgeBTree Broken!\n");
    exit(-1);
  }
}

void EdgeBTree::Print() {
  root_block->PrintBlock();
}

int main() {
  //test edge_b_tree;
  EdgeBTree *edge_b_tree = new EdgeBTree();
  for(size_t i=0; i<10;i++) {
    edge_b_tree->update(i,-1.0*i);
  }
  edge_b_tree->Print();
}
