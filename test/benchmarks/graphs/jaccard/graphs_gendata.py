import numpy as np
import sys

np.set_printoptions(threshold=sys.maxsize)

# Generate bipartite graph on n nodes with edge density e
def gen_adjacency_matrix(n, e):
	if e < 0 or e > 1:
		print('ERROR: Invalid density (0 <= e <= 1)')
		sys.exit()
	G = np.random.binomial(1, e, (n, n))
	np.fill_diagonal(G,0) # Prevent self-loops
	return G

def print_arr(a, n):
	pos = 0
	for i in range(n):
		for j in range(i,n):
			print(f'{a[pos]} ' , end='')
			pos += 1
		print()

# Computes all Jaccard coefficients for G, defined as:
# J(u,v) = |N(u) ? N(v)| / (d(u)+d(v)-|N(u) ? N(v)|)
# where N(u) is the neighborhood of u, and d(u) its degree
def jaccard(G, n):
	jacc = (G @ G.T).astype(float) # Computes intersection of N(i) and N(j)
	deg = [jacc[i][i] for i in range(n)] # Get outdegree of each vertex

	for i in range(n):
		for j in range(i,n):
			jacc[i][j]  = jacc[i][j] / (deg[i] + deg[j] - jacc[i][j])

	return jacc[np.triu_indices_from(jacc)]

# Print np array as a C-style array
def np_to_carr(A):
	s = np.array2string(A, separator=',').replace('[','{').replace(']','}')
	return s

def write_file(name):
	e = 0.2
	n = 32
	with open(name, 'w') as f:
		f.write(f'#define N {n}\n')
		G = gen_adjacency_matrix(n, e)
		J = jaccard(G, n)
		f.write(f'int test_graph[N][N] =\n {np_to_carr(G)};\n')
		f.write(f'float true_jacc[N*(N+1)/2] =\n {np_to_carr(J)};\n')

	print('dataset1.h created successfully')

write_file('dataset1.h')
