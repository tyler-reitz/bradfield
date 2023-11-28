const fs = require("node:fs")
const {chunk, zip} = require("lodash")

const buf = fs.readFileSync(process.argv[2])

const off = buf.readInt32LE(10)
const w = buf.readUInt16LE(18)
const h = buf.readUInt16LE(22)
const l = buf.readInt32LE(34)

const hdr = buf.subarray(0, off)
const pxl = buf.subarray(off)

const pxls = chunk(pxl, 3).map(Buffer.from)
const rows = chunk(pxls, w) 
const cols = zip(...rows)
const flip = cols.map(c => Buffer.concat(c)).reverse()

const rot90 = Buffer.concat(flip)

fs.writeFileSync('flipped.bmp', Buffer.concat([hdr, rot90]))

