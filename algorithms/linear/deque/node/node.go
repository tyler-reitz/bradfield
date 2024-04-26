// Node for Deque's doubly-linked list package
package node

type Node struct {
  Prev, Next *Node
  Val interface{}
}

func New(val interface{}) (Node) {
  return Node{Val: val}
}
