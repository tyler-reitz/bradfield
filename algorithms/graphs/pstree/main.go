package main

import (
  "bufio"
  "fmt"
  "os"
  "slices"
)

func main() {
  scanner := bufio.NewScanner(os.Stdin)

  pt := make(ProcessTree)

  for scanner.Scan() {
    p := ParseProcess(scanner.Text())
    pt.AddProcess(p)    
  }

  fmt.Println(pt)

  dfs := []int{0}

  for len(dfs) > 0 {
    l := len(dfs)-1
    top := dfs[l]
    // fmt.Println(pt[top].cmd)
    dfs = dfs[:l]

    cids := pt[top].cids
    // fmt.Println(dfs)


    if len(cids) < 1 {
      continue
    }

    slices.Reverse(cids)

    for _, pid := range cids {
      dfs = append(dfs, pid)
    } 
  }


  // prefix := ""
  // 
  // if p.ppid > depth {
  //   prefix += "\t"
  // }  

  // if p.ppid < depth {
  //   if len(prefix) > 0 {
  //     prefix = prefix[:len(prefix)-1]
  //   }
  // }

  // fmt.Println(prefix+p.cmd)
  // depth = p.ppid

  // fmt.Println(pt) 

  if err := scanner.Err(); err != nil {
    fmt.Fprintln(os.Stderr, "reading standard input:", err)
  }
}

