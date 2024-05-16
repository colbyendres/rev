#include "printf.h"
#include "rev-macros.h"
#include "syscalls.h"

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
  TRACE_ON;
  Node* n = (Node*) new( sizeof( Node ) );
  if( empty( q ) ) {
    q->head = n;
    q->tail = n;
  } else {
    q->tail->next = n;
    q->tail       = n;
  }
  q->size++;
  TRACE_OFF;
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

int main() {
  // Partial initialization to force q.size to start at 0
  Queue q = { 0 };
  printf( "Queue status before push: %p, %p, %d\n", q.head, q.tail, q.size );
  push( &q, 42 );
  printf( "Queue status after push: %p, %p, %d\n", q.head, q.tail, q.size );
  pop( &q );
  return 0;
}
