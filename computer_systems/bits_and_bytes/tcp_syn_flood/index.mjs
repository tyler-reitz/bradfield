import assert from 'node:assert/strict'
import { Buffer } from 'node:buffer'
import * as fs from 'node:fs'

const PCAP_HEADER_FIELD_BYTES = 4
const INTS = n => n*PCAP_HEADER_FIELD_BYTES

let offset = 0
let n = INTS(6)

const fd = fs.openSync('synflood.pcap')
const fd_size = fs.fstatSync(fd).size
const fileHdrbuf = Buffer.alloc(INTS(6))

offset += fs.readSync(fd, fileHdrbuf, offset, n)

const magic = fileHdrbuf.subarray(0, INTS(1))
const ssl = fileHdrbuf.subarray(INTS(4), INTS(5))

assert.deepEqual(magic.readUInt32LE(0), 0xa1b2c3d4)

const per_packet_hdr = Buffer.alloc(INTS(4))

let total, initd, acked
initd = acked = total = 0

while (offset < fd_size) {
  offset += fs.readSync(fd, per_packet_hdr, 0, INTS(4), offset)
  let pckt_len = per_packet_hdr.subarray(INTS(2), INTS(3)).readUInt32LE(0)

  const ip_pckt_buf = Buffer.alloc(pckt_len)
  offset += fs.readSync(fd, ip_pckt_buf, 0, pckt_len, offset)

  assert.deepEqual(ip_pckt_buf.readInt8(0), 2) // ipv4

  const ihl = ip_pckt_buf.readInt8(4) & 0x0F

  const src = ip_pckt_buf.readUInt16BE((ihl+1)*4)
  const dst = ip_pckt_buf.readUInt16BE((ihl+1)*4+2)

  const flags = ip_pckt_buf.readInt8((ihl+1+3)*4+1)

  const syn = (flags & 0x02) > 0
  const ack = (flags & 0x10) > 0

  total++
  if (syn && dst === 80) initd++
  if (ack && src === 80) acked++
}

console.log(`total: ${total}, syn: ${initd}, ack:${acked}, %: ${acked/initd}`)

