all: edge_b_tree

edge_b_tree: edge_b_tree.cpp edge_b_tree.h
	g++ edge_b_tree.cpp -o edge_b_tree

clean:
	rm edge_b_tree
