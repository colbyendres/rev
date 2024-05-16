#include "dataset1.hpp"
#include <queue>

#define static_assert( cond ) \
  switch( 0 ) {               \
  case 0:                     \
  case !!(long) ( cond ):;    \
  }
#define assert( x )               \
  do                              \
    if( !( x ) ) {                \
      asm( ".dword 0x00000000" ); \
    }                             \
  while( 0 )

static int verify( int n, const volatile int* test, const int* verify ) {
  int i;
  // Unrolled for faster verification
  for( i = 0; i < n / 2 * 2; i += 2 ) {
    int t0 = test[i], t1 = test[i + 1];
    int v0 = verify[i], v1 = verify[i + 1];
    if( t0 != v0 )
      return i + 1;
    if( t1 != v1 )
      return i + 2;
  }
  if( n % 2 != 0 && test[n - 1] != verify[n - 1] )
    return n;
  return 0;
}

void bfs( int* traversal, int src ) {
  std::queue<int> q;
  int             seen[N] = { 0 };
  seen[src]               = 1;
  q.push( src );
  int idx = 0;

  while( !q.empty() ) {
    int v = q.front();
    q.pop();
    traversal[idx++] = v;
    for( int i = 0; i < N; i++ ) {
      if( test_graph[v][i] == 1 && seen[i] == 0 ) {
        seen[i] = 1;
        q.push( i );
      }
    }
  }
}

int main() {
  int bfs_traversal[N] = { 0 };
  int src              = 0;

  bfs( &bfs_traversal[0], src );

  int check = verify( N, &bfs_traversal[0], &true_bfs[0] );
  assert( check == 0 );

  return 0;
}
