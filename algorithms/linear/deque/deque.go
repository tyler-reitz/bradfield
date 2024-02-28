package deque

import (
  "node"
)

type Deque struct {
  Size int
  Head, Tail *node.Node
}

func New() Deque {
  return Deque{}
}

func (d *Deque) Push(n *node.Node) {
  if d.Head == nil {
    d.Tail, d.Head = n, n
  }

  n.Prev = d.Tail
  d.Tail.Next = n
  d.Tail = n
  d.Size++
}

func (d *Deque) Pop() *node.Node {
  if d.Size == 0 || d.Tail == nil && d.Head == nil {
    return nil
  }

  n := d.Tail
  d.Tail, n.Prev = n.Prev, nil
  d.Size--

  return n
}

func (d *Deque) Shift(n *node.Node) {
  if d.Size == 0 {
    d.Head, d.Tail = n, d.Head
  }

  d.Head.Prev = n
  n.Next = d.Head
  d.Head = n

  d.Size++
}

func (d *Deque) Unshift() *node.Node {
  if d.Size == 0 {
    return nil
  }

  n := d.Head
  d.Head = n.Next
  n.Next = nil
  
  d.Size--
  
  return n
}

func (d *Deque) Enque(n *node.Node) {
  d.Push(n)
}

func (d *Deque) Deque() *node.Node {
  return d.Unshift()
}

func (d *Deque) Find(val interface{}) *node.Node {
  for n:=d.Head; n != nil; n=n.Next {
    if n.Val == val {
      n.Prev.Next = n.Next
      if n.Next != nil {
        n.Next.Prev = n.Prev
      }
      return n
    }
  }

  return nil
}

