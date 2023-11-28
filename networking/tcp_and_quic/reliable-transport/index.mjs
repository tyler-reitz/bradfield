import util from "node:util"
import dgram from "node:dgram"

const MAX_PAYLOAD_SIZE = 4

class ReliableDelivery extends dgram.Socket {
  constructor(data="") {
    super({type: 'udp4'})

    this.chunks = this.chunk(data)

    this.seq = 0
    this.ack = 0

    this.payloads = []

    super.on('message', this.recv)
  }

  *chunk(data="") {
    const buf = Buffer.from(data)

    for (let i=0; i<buf.length; i+=MAX_PAYLOAD_SIZE) {
      yield buf.subarray(i, i+MAX_PAYLOAD_SIZE)
    }
  }

  send() {
    const {value, done} = this.chunks.next()

    if (!done) {
      const hdrs = Buffer.from([this.seq, this.ack])

      super.send([hdrs, value])
      this.timeout = setInterval(super.send.bind(this), 500, [hdrs, value])}
  }

  recv(msg, {port, address}) {
    let [seq, ack, ...payload] = msg;

    if (payload.length) {
      if (this.ack <= seq) {
        super.send(Buffer.from([this.seq, this.ack=seq+1]), port, address) 
        console.log(seq, this.seq,  ack, this.ack, Buffer.from(payload))
      } else {
        super.send(Buffer.from([this.seq, this.ack]), port, address) 
      }
    } else {
      if (process.argv[2] && this.seq < ack) {
        console.log(seq, this.seq++,  ack, this.ack, Buffer.from(payload))
        clearTimeout(this.timeout)
        this.send()
      }
    }
  }
}

/** Main **/
if (process.argv.length > 2) {
  const rd = new ReliableDelivery("0123456789".repeat(3))
  rd.connect(process.argv[2], () => {
    rd.send() 
  })
} else {
  const rd = new ReliableDelivery()
  rd.bind(8000)
}

