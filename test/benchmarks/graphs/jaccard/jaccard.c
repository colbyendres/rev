#include "dataset1.h"
#include "util.h"
#include <math.h>

// Convert ordinary indices to BLAS-like packed indices
#define idx( i, j ) i* N + j - i*( i + 1 ) / 2

double dot( int i, int j ) {
  double d = 0.0;
  for( int k = 0; k < N; k++ ) {
    d += test_graph[i][k] * test_graph[j][k];
  }
  return d;
}

void jaccard( double* jacc ) {
  // Compute G*G^T
  for( int i = 0; i < N; i++ ) {
    for( int j = i; j < N; j++ ) {
      jacc[idx( i, j )] = dot( i, j );
    }
  }

  // Get outdegrees
  int deg[N];
  for( int i = 0; i < N; i++ ) {
    deg[i] = jacc[idx( i, i )];
  }

  // Find Jaccard coefficients
  for( int i = 0; i < N; i++ ) {
    for( int j = i; j < N; j++ ) {
      jacc[idx( i, j )] = (double) jacc[idx( i, j )] / ( deg[i] + deg[j] - jacc[idx( i, j )] );
    }
  }
}

int main() {
  double jacc[N * ( N + 1 ) / 2];
  jaccard( &jacc[0] );
  int check = verifyDouble( N * ( N + 1 ) / 2, &jacc[0], &true_jacc[0] );
  assert( check == 0 );
  return 0;
}
