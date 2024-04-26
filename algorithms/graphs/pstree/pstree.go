package main

import (
  "fmt"
  "strings"
)

type Process struct {
  pid, ppid int
  cmd string
  cids []int
}

type ProcessTree map[int]Process

func (pt *ProcessTree) AddProcess(p Process) {
  (*pt)[p.pid] = p

  pp := (*pt)[p.ppid]

  if p.pid > 0 {
    pp.cids = append(pp.cids, p.pid)
  }
  
  (*pt)[p.ppid] = pp
}

func ParseProcess(l string) Process {
  var p Process

  fmt.Sscanf(l, "%d %d %s", &p.pid, &p.ppid, &p.cmd)

  if _, after, ok := strings.Cut(l, p.cmd); ok==true {
    p.cmd += after
  }

  return p
}

