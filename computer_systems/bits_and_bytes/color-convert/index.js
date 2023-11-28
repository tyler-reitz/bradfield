const hexRegEx = /#([0-9a-fA-F])+/g

process.stdin.setEncoding('utf8')

const hex2dec = (xx) => {
  xx = xx.toLowerCase().charCodeAt(0)

  if (97 <= xx && xx <= 102)
    return xx - 87

  return parseInt(String.fromCodePoint(xx))
}

const hex2rgb = (match) => {
  const rgb = []

  for (let i=1; i<match.length; i+=2) {
    rgb.push((hex2dec(match[i]) << 4) + hex2dec(match[i+1]))
  }

  return `rgb(${rgb.join(' ')})`
}

process.stdin.on('data', (chunk) => {
  process.stdout.write(chunk.replaceAll(hexRegEx, hex2rgb))
})
