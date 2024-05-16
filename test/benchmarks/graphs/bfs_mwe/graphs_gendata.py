import numpy as np
import sys

np.set_printoptions(threshold=sys.maxsize)

# Generate undirected graph with e edges on n vertices
def gen_adjacency_matrix(n, e):
	if e < 0 or e > n*(n-1)/2:
		print('ERROR: Invalid number of edges')
		sys.exit()
	elif e < n-1:
		print('WARNING: Graph is unconnected')

	# Create a tree to ensure G is connected
	G = np.zeros((n,n)).astype(int)
	V = [i for i in range(n)] # unvisited vertices
	src = np.random.choice(V)
	V.remove(src) # mark src as visited

	# Random walk to generate tree
	for i in range(n-1):
		dest = np.random.choice(V)
		G[src][dest] = 1
		G[dest][src] = 1 # G=G^T for undirected graphs
		V.remove(dest)
		src = dest

	# Find all missing edges
	missing_edges = []
	for i in range(n):
		for j in range(i+1,n):
			if G[i][j] == 0:
				missing_edges.append((i,j))

	# Randomly add edges to meet quota
	# Use indices instead of (i,j) to allow for random.choice
	edges_to_add = np.random.choice(len(missing_edges), size=e-n+1, replace=False)
	for idx in edges_to_add:
		i = missing_edges[idx][0]
		j = missing_edges[idx][1]
		G[i][j] = 1
		G[j][i] = 1

	return G

# Print np array as a C-style array
def np_to_carr(A):
	s = np.array2string(A, separator=',').replace('[','{').replace(']','}')
	return s

# Breadth-first search
def bfs(A, src):
	Q = [src]
	visited = set([src])
	traversal = []
	n = len(A)

	while Q:
		v = Q.pop(0)
		traversal.append(v)
		for i in range(n):
			if A[v][i] == 1 and i not in visited:
				Q.append(i)
				visited.add(i)

	return '{' + str(traversal)[1:-1] + '}'

def write_file(name):
	N = 32
	P = int(0.75*N*(N-1)//2) # 75% density of edges

	with open(name, 'w') as f:
		f.write(f'#define N {N}\n')
		A = gen_adjacency_matrix(N,P)
		f.write(f'int test_graph[N][N] =\n {np_to_carr(A)};\n')
		f.write(f'int true_bfs[N] = ' + bfs(A, 0) + ';\n')

	print('dataset1.h created successfully')

write_file('dataset1.h')
