#include "dataset1.h"
#include "util.h"
#include <limits.h>

// Single source shortest path
void sssp( int* d, int src ) {
  // Initialize distances
  for( int i = 0; i < N; i++ ) {
    d[i] = INT_MAX;
  }
  d[src] = 0;

  // Use Bellman-Ford for SSSP, since priority_queue isn't readily available
  int has_changed;
  for( int it = 0; it < N - 1; it++ ) {
    has_changed = 0;
    for( int i = 0; i < N; i++ ) {
      for( int j = 0; j < N; j++ ) {
        if( test_graph[i][j] == 1 && d[i] != INT_MAX && d[i] + w[i][j] < d[j] ) {  // relax
          d[j]        = d[i] + w[i][j];
          has_changed = 1;
        }
      }
    }
    if( has_changed == 0 ) {  // no relaxation, can terminate early
      return;
    }
  }
}

int main() {
  int dists[N];
  int src = 0;

  sssp( &dists[0], src );
  assert( 0 == verify( N, dists, true_dists ) );

  return 0;
}
