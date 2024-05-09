import numpy as np
import sys

np.set_printoptions(threshold=sys.maxsize)

# Generate acyclic graph on n vertices
def gen_adjacency_matrix(n):
    # Create a tree to ensure G is connected
    G = np.zeros((n,n)).astype(int)
    V = [i for i in range(n)] # unvisited vertices
    src = np.random.choice(V)
    V.remove(src) # mark src as visited

    # Random walk to generate tree
    for i in range(n-1):
        dest = np.random.choice(V)
        G[src][dest] = 1
        V.remove(dest)
        src = dest

    return G

# Print np array as a C-style array
def np_to_carr(A):
    s = np.array2string(A, separator=',').replace('[','{').replace(']','}')
    return s

# Give a topological sorting of the DAG A
def topo_sort(A):
    N = len(A)

    # Status values for each node
    UNVISITED = 0
    IN_PROGRESS = 1
    VISITED = 2

    status = N * [UNVISITED]
    traversal = []

   # DFS traversal
    def visit(v):
        status[v] = IN_PROGRESS
        for i in range(N):
            if A[v][i] == 1 and status[i] == UNVISITED:
                visit(i)
        status[v] = VISITED
        traversal.append(v)

    # Start traversal until all nodes are seen
    for i in range(N):
        if status[i] == UNVISITED:
            visit(i)

    # Sort by decreasing finishing time
    traversal = traversal[::-1]
    return '{' + str(traversal)[1:-1] + '}'

def write_file(name):
    N = 32
    with open(name, 'w') as f:
        f.write(f'#define N {N}\n')
        A = gen_adjacency_matrix(N)
        f.write(f'int test_graph[N][N] =\n {np_to_carr(A)};\n')
        f.write(f'int true_topo[N] = ' + topo_sort(A) + ';\n')
    print('dataset1.h created successfully')

write_file('dataset1.h')
