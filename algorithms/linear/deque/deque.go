// Deque implements a doubly-linked list
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

func (d *Deque) Push(val interface{}) {
  n := node.New(val)

  if d.Size == 0 {
    d.Tail, d.Head = &n, &n
  }

  n.Prev = d.Tail
  d.Tail.Next, d.Tail = &n, &n

  d.Size++
}

func (d *Deque) Pop() interface{} {
  if d.Size == 0 {
    return nil
  }

  n := d.Tail
  d.Tail, n.Prev = n.Prev, nil

  d.Size--

  return n.Val
}

func (d *Deque) Shift(val interface{}) {
  n := node.New(val)

  if d.Size == 0 {
    d.Head, d.Tail = &n, d.Head
  }

  d.Head.Prev = &n
  n.Next = d.Head
  d.Head = &n

  d.Size++
}

func (d *Deque) Unshift() interface{} {
  if d.Size == 0 {
    return nil
  }

  n := d.Head
  d.Head = n.Next
  n.Next = nil
  
  d.Size--
  
  return n.Val
}

func (d *Deque) Enque(val interface{}) {
  d.Push(val)
}

func (d *Deque) Deque() interface{} {
  return d.Unshift()
}

func (d *Deque) Find(val interface{}) interface{} {
  for n:=d.Head; n != nil; n=n.Next {
    if n.Val == val {
      n.Prev.Next = n.Next
      if n.Next != nil {
        n.Next.Prev = n.Prev
      }
      return n.Val
    }
  }

  return nil
}

