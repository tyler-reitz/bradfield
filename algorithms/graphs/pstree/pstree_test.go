package main

import (
  "slices"
  "testing"
)

func TestProcessPrintLeaf(t *testing.T) {
  pt := make(ProcessTree)

  p0 := Process{pid: "1", ppid: "0", cmd: "echo"}
  
  pt.AddProcess(p0)

  pt.BuildLeaf()
}

func TestProcessTreeAddProcess(t *testing.T) {
  pt := make(ProcessTree)

  cases := []struct{
    proc Process
  }{
    {
      proc: Process{pid: "1", ppid: "0", cmd: "echo"},
    },
    {
      proc: Process{pid: "2", ppid: "1", cmd: "cat"},
    },
    {
      proc: Process{pid: "3", ppid: "1", cmd: "cut"},
    },
    {
      proc: Process{pid: "4", ppid: "2", cmd: "zip"},
    },
  }

  for _, _case := range cases {
    pt.AddProcess(_case.proc)

    if pt[_case.proc.pid].pid != _case.proc.pid {
      t.Errorf("ptree.AddProcess(%v) = %v, want %v", _case.proc, pt[_case.proc.pid], _case.proc.pid)
    }

    if slices.Contains(pt[_case.proc.ppid].cids, _case.proc.pid) != true {
      t.Errorf("ptree.AddProcess(%v) = %v, want %v", _case.proc, pt, _case.proc.pid)
    }
  }

  t.Errorf("%v", pt)
}

func TestParseProcess(t *testing.T) {
  cases := []struct{
    line string
    want Process
  }{
    {
      line: "",
      want: Process{},
    },
    {
      line: "1 0 echo foo bar",
      want: Process{pid: "1", ppid: "0", cmd: "echo", args: "foo bar"},
    },
  }

  for _, _case := range cases {
    got := ParseProcess(_case.line)

    if got.pid != _case.want.pid {
      t.Errorf("ParseProcess(%v) = %v, want %v", _case.line, got, _case.want)
    }

    if got.ppid != _case.want.ppid {
      t.Errorf("ParseProcess(%v) = %v, want %v", _case.line, got, _case.want)
    }

    if got.cmd != _case.want.cmd {
      t.Errorf("ParseProcess(%v) = %v, want %v", _case.line, got, _case.want)
    }

    if got.args != _case.want.args {
      t.Errorf("ParseProcess(%v) = %v, want %v", _case.line, got, _case.want)
    }
  }
}

