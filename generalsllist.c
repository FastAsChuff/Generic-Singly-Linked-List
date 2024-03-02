
#ifndef alignof 
  #define alignof _Alignof
#endif

/*
  Author: Simon Goater March 2024
  
  For a singly linked list of mystructnode_t structs, with struct node *next as first element, e.g.
  
typedef struct node {
  struct node *next; // Must be first member
  int16_t someval;
  ...
} mystructnode_t;
  
  functions can be used as follows....
    
  mystructnode_t *mystructnoderoot = NULL;
  mystructnode_t *mystructnodehead = NULL;
  mystructnode_t newnode;
  newnode.next = NULL;
  newnode.someval = ...  // Initialise struct and add as first node
  handleerror(appendnode((void **)&mystructnoderoot, (void **)&mystructnodehead, (void *)&newnode, sizeof(newnode), alignof(newnode)));
  newnode.someval = ...  // Initialise struct and add as second node
  handleerror(appendnode((void **)&mystructnoderoot, (void **)&mystructnodehead, (void *)&newnode, sizeof(newnode), alignof(newnode)));
  newnode.someval = ...  // Initialise struct and add as third node
  handleerror(appendnode((void **)&mystructnoderoot, (void **)&mystructnodehead, (void *)&newnode, sizeof(newnode), alignof(newnode)));
  ...etc.
  --- Do Stuff ---
  freelist((void**)&mystructnoderoot, (void**)&mystructnodehead);
*/

void freelist(void **rootptr, void **headptr) {
  if (rootptr == NULL) return;
  if (*rootptr == NULL) return;
  void *node = *rootptr;
  void *nextnode;
  memcpy(&nextnode, node, sizeof(void*));
  while (1) {
    free(node);
    if (nextnode == NULL) break;
    node = nextnode;
    memcpy(&nextnode, node, sizeof(void*));
  }
  *rootptr = NULL;
  if (headptr == NULL) return;
  *headptr = NULL;
}

_Bool appendnode(void **rootptr, void **headptr, void *newnode, size_t newnodesize, size_t newnodealign) {
  if (headptr == NULL) return false;
  void *newnodeptr = aligned_alloc(newnodealign, newnodesize);
  if (newnodeptr == NULL) return false;
  memcpy(newnodeptr, newnode, newnodesize);
  if (*headptr != NULL) {
    memcpy(*headptr, &newnodeptr, sizeof(void*));
  }
  memcpy(headptr, &newnodeptr, sizeof(void*));
  if (rootptr != NULL) {
    if (*rootptr == NULL) {
      memcpy(rootptr, &newnodeptr, sizeof(void*));
    }
  }
  return true;
}
