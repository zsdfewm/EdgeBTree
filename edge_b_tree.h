
typedef unsigned int locint;

class EdgeBTreeBlock {
 public:
  // default ctor
  EdgeBTreeBlock();

 private:
  locint *ind;
  double *weight;
  EdgeBTreeBlock *child_block;
}

class EdgeBTree {
 public:
  // default ctor, initialize the root_block;
  EdgeBTree();

  // update an entry, for non-existing entries, create one with 0.0
  void update(int i, double w);

  // find/create an entry, return the reference to the weight location;
  double* find(int i);


 private:
  EdgeBTreBlock *root_block;

}
