import assert from "node:assert/strict"
import fs from "node:fs"

const fd = fs.openSync("lossy.pcap")

const file_hdr = Buffer.alloc(4*6)

let pos = 0;

pos += fs.readSync(fd, file_hdr, 0, 4*6, 0)  
console.log(file_hdr)
assert.equal(file_hdr.readUInt32LE(0), 0xa1b2c3d4)

let pckt_hdr = Buffer.alloc(4*4)
//while(1) {
  const n = fs.readSync(fd, pckt_hdr, 0, 4*4, pos)
  // if (!n) break
  pos += n
  console.log(pckt_hdr)
  console.log(pckt_hdr.readUInt32LE(4*2))

  let foo = Buffer.alloc(n)
  pos += fs.readSync(fd, foo, 0, n, pos)
  console.log(foo)
// }
