package node

import (
  "testing"
)

func TestNodeNew(t *testing.T) {
  val := "42"
  n := New(val)

  if n.prev != nil || n.next != nil || n.val != val {
    t.Fatalf("No new node")
  }
}

