#include "dataset1.h"
#include "util.h"
#include <math.h>

// Convert ordinary indices to BLAS-like packed indices
#define idx( i, j ) i* N + j - i*( i + 1 ) / 2

float dot( int i, int j ) {
  float d = 0.0f;
  for( int k = 0; k < N; k++ ) {
    d += test_graph[i][k] * test_graph[j][k];
  }
  return d;
}

void jaccard( float* jacc ) {
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
      jacc[idx( i, j )] = (float) jacc[idx( i, j )] / ( deg[i] + deg[j] - jacc[idx( i, j )] );
    }
  }
}

// Float equality based on equivalent NumPy function
int allClose( int n, float* test, float* verif ) {
  const float atol = 1e-8;
  const float rtol = 1e-5;
  int         i;

  for( i = 0; i < n / 2 * 2; i += 2 ) {
    int eq1 = fabs( test[i] - verif[i] ) <= atol + rtol * fabs( verif[i] );
    int eq2 = fabs( test[i + 1] - verif[i + 1] ) <= atol + rtol * fabs( verif[i + 1] );
    if( !( eq1 & eq2 ) ) {
      return i + 1 + eq1;
    }
  }

  if( n % 2 == 1 && fabs( test[n - 1] - verif[n - 1] ) > atol + rtol * fabs( verif[n - 1] ) ) {
    return n;
  }

  return 0;
}

int main() {
  float jacc[N * ( N + 1 ) / 2];
  jaccard( &jacc[0] );
  int check = allClose( N * ( N + 1 ) / 2, &jacc[0], &true_jacc[0] );
  assert( check == 0 );
  return 0;
}
