import fs from "node:fs";

const template = (_, i, arr) =>
  arr.map((ch, j) => (i === j ? "?" : ch)).join("");

const graphEntry = (acc, curr) => (tmpl) =>
  acc[tmpl] ? acc[tmpl].push(curr) : (acc[tmpl] = [curr]);

const toGraph = (acc, curr) => {
  curr.split("").map(template).forEach(graphEntry(acc, curr));

  return acc;
};

const words = fs
  .readFileSync("./words.txt", "utf8")
  .split("\n")
  .reduce(toGraph, {});

const ladder = (from, to) => {
  const seen = new Set(from)
  const queue = [from]

  while (queue.length) {
    for (const tmpl of queue.shift().split('').map(template)) {
      console.log(tmpl, words[tmpl])
    }
  }
}

ladder('head', 'tail')

