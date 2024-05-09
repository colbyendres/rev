#include "dataset1.h"
#include "util.h"

#define UNVISITED   0
#define IN_PROGRESS 1
#define VISITED     2

int idx = N - 1;

void visit( int v, int* traversal, int* status ) {
  status[v] = IN_PROGRESS;
  for( int i = 0; i < N; i++ ) {
    if( test_graph[v][i] == 1 && status[i] == UNVISITED ) {
      visit( i, traversal, status );
    }
  }
  status[v]        = VISITED;
  traversal[idx--] = v;
}

void topo_sort( int* traversal ) {
  int status[N] = { UNVISITED };
  for( int i = 0; i < N; i++ ) {
    if( status[i] == UNVISITED ) {
      visit( i, traversal, status );
    }
  }
}

int main() {
  int traversal[N];
  topo_sort( &traversal[0] );

  int check = verify( N, traversal, true_topo );
  assert( check == 0 );
  return 0;
}
