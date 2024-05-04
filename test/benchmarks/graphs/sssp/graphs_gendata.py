import numpy as np
import sys
from heapq import *

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

def gen_weight_matrix(N,A):
	W = np.zeros((N,N)).astype(int)
	for i in range(N):
		for j in range(i+1,N):
			if A[i][j] == 1:
				W[i][j] = np.random.randint(1,20)
				W[j][i] = W[i][j]
	return W

# Print np array as a C-style array
def np_to_carr(A):
	s = np.array2string(A, separator=',').replace('[','{').replace(']','}')
	return s

def dijkstra(A, W, src):
	n = len(A)
	dist = n * [np.inf]
	dist[src] = 0
	Q = [(i, dist[i]) for i in range(n)]
	heapify(Q) # priority queue

	while len(Q) > 1:
		u = heappop(Q)[0]
		for i in range(n): # Examine all neighbors of u
			if A[u][i] == 1 and dist[u] + W[u][i] < dist[i]: # relax
				dist[i] = dist[u] + W[u][i]
				heappush(Q, (i,dist[i]))

	return '{' + str(dist)[1:-1] + '}'

def write_file(name):
	N = 32
	P = int(0.75*N*(N-1)//2) # 75% density of edges

	with open(name, 'w') as f:
		f.write(f'#define N {N}\n')
		A = gen_adjacency_matrix(N,P)
		W = gen_weight_matrix(N,A)
		f.write(f'int test_graph[N][N] =\n {np_to_carr(A)};\n')
		f.write(f'int w[N][N] =\n {np_to_carr(W)};\n')
		f.write(f'int true_dists[N] = ' + dijkstra(A, W, 0) + ';\n\n')

	print('dataset1.h created successfully')

write_file('dataset1.h')
