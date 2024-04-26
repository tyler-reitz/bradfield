import {stdin, stdout} from "node:process"
import readline from "node:readline"


(async function() {
  const rl = readline.createInterface({ input: stdin })

  const pmap = {}
  pmap[0] = { cmd: '', children: [] }

  for await (const line of rl) {
    const [ _, pid, ppid, ...cmd ] = line.split(/[ ]+/)

    if (pmap[pid]) {
      pmap[ppid].children.push(pid)
    } else {
      pmap[pid] = {cmd, children: []}
      pmap[ppid]?.children.push(pid)
    }
  }

  printree(pmap, pmap[0])
})()

const printree = (pmap, {cmd, children}, depth='\b\b') => {
  if (cmd) process.stdout.write((depth + cmd.join(' ')).trim() + '\n')

  if (!children.length) return
  
  for (let pid=children.pop(); pid; pid=children.pop()) {
    printree(pmap, pmap[pid], depth+'  ')
  }
}
