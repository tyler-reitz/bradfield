import { plot } from "nodeplotlib"

const data = [
  { x: [1,2,3,4,5], y: [1,4,9,16,25], type: 'scatter' },
  { x: [1,2,3,4,5], y: [1,2,3,4,5], type: 'scatter' }
]

plot(data)
