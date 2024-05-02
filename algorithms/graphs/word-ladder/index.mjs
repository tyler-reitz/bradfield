import fs from "node:fs";

const node = (_, i, arr) =>
  arr.map((ch, j) => (i === j ? "?" : ch)).join("");

const vertice = (acc, curr) => (tmpl) =>
  acc[tmpl] ? acc[tmpl].push(curr) : (acc[tmpl] = [curr]);

const toGraph = (acc, curr) => {
  curr.split("").map(node).forEach(vertice(acc, curr));

  return acc;
};

const words = fs
  .readFileSync("./words.txt", "utf8")
  .split("\n")
  .reduce(toGraph, {});

const ladder = (from, to) => {
  const seen = new Set()
  const queue = [from]
  const path = []

  while (queue.length) {
    for (const tmpl of queue.shift().split('').map(node)) {
      if (words[tmpl].includes(to)) {
        path.push(words[tmpl])
        return path
      }


      if (!seen.has(tmpl)) {
        seen.add(tmpl)
        queue.push(...words[tmpl])
        path.push(words[tmpl])
      }
    }
  }

  return []
}

const from = 'freak'
const to = 'grant'

const nodes = ladder(from, to)

const tail = nodes.pop()

const path = [to]

let a = new Set(tail)

while (!path.flat().includes(from)) {
  for (const node of nodes) {
    const b = new Set(node)
    const x = a.intersection(b)

    if (x.size) {
      if (x.size > 1) {
        path.push(from)
      } else {
        path.push(...x.values())
      }
      a = b 
      break
    } 
  }
}

console.log(path.reverse().join(' -> '))