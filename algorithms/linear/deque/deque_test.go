package deque

import (
  "node"
  "testing"
)

func TestNewDeque(t *testing.T) {
  d := New()

  if d.Head != nil || d.Size != 0 {
    t.Fatalf("Deque initialization failed")
  }
}

func TestDequePushPop(t *testing.T) {
  d := New()

  n := node.New("42")
  d.Push(&n)

  if d.Head.Val != "42" || d.Size != 1 {
    t.Errorf("Empty Deque push failed: d.Head = %v, d.Size = %d", d.Head, d.Size)
  }

  m := node.New("24") 
  d.Push(&m)
  if d.Head.Val != "42" || d.Tail.Val != "24" || d.Size != 2 {
    t.Errorf("Deque push failed: d.Head = %v, d.Size = %d", d.Tail, d.Size)
  }

  o := node.New("1337") 
  d.Push(&o)
  if d.Head.Val != "42" || d.Tail.Val != "1337" || d.Size != 3 {
    t.Errorf("Deque push failed: d.Head = %v, d.Size = %d", d.Tail, d.Size)
  }

  p := d.Pop() 
  if p.Val != "1337" || d.Tail.Val != "24" || d.Size > 2  {
    t.Errorf("Deque pop failed: p = %v, d.Size = %d", d.Tail, d.Size)
  }

  q := d.Pop() 
  if q.Val != "24" || d.Size > 1 {
    t.Errorf("Deque pop failed: d.Head = %v, d.Size = %d", d.Head, d.Size)
  }

  r := d.Pop() 
  if r.Val != "42" || d.Size > 0 {
    t.Errorf("Deque pop failed: d.Head = %v, d.Size = %d", d.Head, d.Size)
  }

  s := d.Pop() 
  if s != nil || d.Size < 0 {
    t.Errorf("Deque pop failed: d.Head = %v, d.Tail = %v, d.Size = %d", d.Head, d.Tail, d.Size)
  }
}

func TestDequeShiftUnshift(t *testing.T) {
  cases := []struct{
    val string
    size int
  }{
    {val: "42", size: 1},
    {val: "1337", size: 2},
    {val: "24", size: 3},
  }

  d := New()  

  if d.Head != nil || d.Tail != nil || d.Size != 0  {
    t.Errorf("New() = %v, wanted nil", d)
  }

  for _,c := range cases {
    n := node.New(c.val)
    d.Shift(&n)

    if d.Head.Val != n.Val || d.Size != c.size {
      t.Errorf("d.Head = %v, wanted %v", d.Head, n)
    }
  }

  cases = []struct{
    val string
    size int
  } {
    {val: "24", size: 2},
    {val: "1337", size: 1},
    {val: "42", size: 0},
  }

  for _, e := range cases {
    m := d.Unshift()
    if m.Val != e.val || d.Size != e.size {
      t.Errorf("d.Unshift() = %v, wanted %v", m, e)
    }
  }
}

func TestEnqueuDeque(t *testing.T) {
  cases := []struct{
    val string
    size int
  }{
    {val: "42", size: 1},
    {val: "1337", size: 2},
    {val: "24", size: 3},
  }

  d := New()

  for _,c := range cases {
    n := node.New(c.val)
    d.Enque(&n)

    if d.Size != c.size || d.Tail.Val != c.val {
      t.Errorf("d.Enque(%v) = d.Size = %v, d.Tail = %v, wanted %v, %v",
                n, d.Size, d.Tail, c.size, c.val)
    }
  }

  cases = []struct{
    val string
    size int
  }{
    {val: "42", size: 2},
    {val: "1337", size: 1},
    {val: "24", size: 0},
  }

  for _,c := range cases {
    n := d.Deque() 
    if d.Size != c.size || n.Val != c.val {
      t.Errorf("d.Denque() = %v, d.Size = %v, wanted %v, %v",
                n, d.Size, c.size, c.val)
    }
  }
}

func TestDequeFind(t *testing.T) {
  d := New()

  for i:=0;i<10;i++ {
    n := node.New(i)
    d.Enque(&n)
  }  

  cases := []int{0,3,6,9,12}
  for _,val := range cases {
    n := d.Find(val)
    if n != nil && n.Val != val {
      t.Errorf("d.Find(%d) = %v, want %v, got %v", val, n, val, n.Val)
    }
  }
}

