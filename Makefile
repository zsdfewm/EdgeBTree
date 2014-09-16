all: edge_b_tree_test

edge_b_tree.o: edge_b_tree.cpp edge_b_tree.h
	g++ -g edge_b_tree.cpp -c -o edge_b_tree.o

edge_b_tree_test: edge_b_tree.o edge_b_tree_test.cpp
	g++ -g $^ -o $@

test:
	./edge_b_tree_test
clean:
	rm *.o edge_b_tree_test
