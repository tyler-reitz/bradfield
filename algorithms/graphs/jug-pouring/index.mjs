const q = ['00']
const seen = new Set(q)

const fill = (n) => {
  let [a,b,c] = n.split('')

  a = 3
  b = 5

  a = `${a}0`
  b = `0${b}`

  return [a,b]
}

const dump = (n) => {
  let [a,b] = n.split('')

  a = `${a}0`
  b = `0${b}`

  return [a,b]
}

while (q.length) {
  console.log(q, seen)

  const n = q.shift()

  for (const m of fill(n)) {
    console.log(m)
    if (!seen.has(m)) {
      seen.add(m)
      q.push(m)
    }
  }

  for (const m of dump(n)) {
    console.log(m)
    if (!seen.has(m)) {
      seen.add(m)
      q.push(m)
    }
  }
}

console.log('ok')

