import fs from "node:fs"

const eol = b => b === 0x0a

const f = fs.readFileSync("cases")

for (let i=0,n=f.findIndex(eol); f.subarray(i, n).length; i=++n,n=i+f.subarray(i).findIndex(eol)) {
  let [first, ...rest] = f.subarray(i, n)

  let j=first
  for (; rest[j] >> 6 == 2; j--) {}

  process.stdout.write(Buffer.from(rest).subarray(0, j)+"\n")
}

