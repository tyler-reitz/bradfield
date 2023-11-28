const dgram = require('node:dgram')
const { Buffer } = require('node:buffer')

const client = dgram.createSocket('udp4')

const headers = Buffer.from([
  0x00, 0x2a,
  0x01, 0x00,
  0x00, 0x01,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
])

const qname = Buffer.concat(
  process.argv[2]
    .split('.')
    .map(b => [Buffer.from([b.length]), Buffer.from(b)])
    .flatMap(b => Buffer.concat(b))
    .concat(Buffer.from([null]))
)

const question = Buffer.concat([
  qname,
  Buffer.from([ 0x00, 0x01 ]), // qtype: A = 1
  Buffer.from([ 0x00, 0x01 ]), // qclass: IN = 1
])

const query = Buffer.concat([
  headers,
  question,
])

try {
  client.send(query, 53, '8.8.8.8') 
} catch (e) {
  console.log(e)
}

client.on('message', (msg, rinfo) => {
  const ip4 = msg.subarray(msg.length-4, msg.length) // quick & dirty
  console.log(ip4.join('.'))
  client.close()
})

