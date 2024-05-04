#include "dataset1.h"
#include "syscalls.h"
#include "util.h"
#include <stddef.h>

void* new( size_t t ) {
  void* p = (void*) ( rev_mmap( 0, t, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0 ) );
  return p;
}

void del( void* addr, size_t sz ) {
  rev_munmap( (size_t) addr, sz );
}

// Simple queue implementation
typedef struct Node {
  int          val;
  struct Node* next;
} Node;

typedef struct Queue {
  Node* head;
  Node* tail;
  int   size;
} Queue;

int empty( Queue* q ) {
  return q->size == 0;
}

void push( Queue* q, int val ) {
  Node* n = (Node*) new( sizeof( Node ) );
  n->val  = val;
  if( empty( q ) ) {
    q->head = n;
    q->tail = n;
  } else {
    q->tail->next = n;
    q->tail       = n;
  }
  q->size++;
}

int pop( Queue* q ) {
  if( empty( q ) ) {
    return -1;
  }
  Node* oldHead = q->head;
  int   val     = oldHead->val;
  q->head       = oldHead->next;
  del( oldHead, sizeof( Node ) );
  q->size--;
  return val;
}

void bfs( int* traversal, int src ) {
  Queue q;
  int   seen[N] = { 0 };
  seen[src]     = 1;
  push( &q, src );
  int idx = 0;

  while( !empty( &q ) ) {
    int v            = pop( &q );
    traversal[idx++] = v;
    for( int i = 0; i < N; i++ ) {
      if( test_graph[v][i] == 1 && seen[i] == 0 ) {
        seen[i] = 1;
        push( &q, i );
      }
    }
  }
}

int main() {
  int bfs_traversal[N] = { 0 };
  int src              = 0;

  bfs( &bfs_traversal[0], src );

  int check = verify( N, bfs_traversal, true_bfs );
  assert( check == 0 );

  return 0;
}
