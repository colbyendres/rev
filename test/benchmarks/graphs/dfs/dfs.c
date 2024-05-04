#include "dataset1.h"
#include "util.h"

// Simple depth-first search
int idx = 0;

void dfs( int* traversal, int* seen, int v ) {
  seen[v]          = 1;
  traversal[idx++] = v;
  for( int i = 0; i < N; i++ ) {
    if( test_graph[v][i] == 1 && seen[i] == 0 ) {
      dfs( traversal, seen, i );
    }
  }
}

int main() {
  int dfs_traversal[N];
  int dfs_seen[N] = { 0 };
  int src         = 0;

  dfs( &dfs_traversal[0], &dfs_seen[0], src );

  int check = verify( N, dfs_traversal, true_dfs );
  assert( check == 0 );

  return 0;
}
