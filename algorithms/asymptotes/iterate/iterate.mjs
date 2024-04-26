import assert from "node:assert"
import { plot } from "nodeplotlib"

export function bs(nums, n) {
  let lo=0, hi=nums.length

  while(hi > lo) {
    const mid = Math.floor((lo+hi)/2)
    const x = nums[mid]

    if (x === n) return mid
    if (n < x) hi = mid
    if (n > x) lo = mid+1
  }

  return null
}

assert.equal(bs([-5,-3,0,3,5], -5), 0)
assert.equal(bs([-5,-3,0,3,5], -3), 1)
assert.equal(bs([-5,-3,0,3,5], 0), 2)
assert.equal(bs([-5,-3,0,3,5], 3), 3)
assert.equal(bs([-5,-3,0,3,5], 5), 4)
assert.equal(bs([-5,-3,0,3,5], 9), null)

let n = 200
const labels = Array(n).fill().map((_, i) => i)

const y1 = []
const y2 = []

for (let i=0; i<n; i++) {
  let arr = Array(n).fill(Math.floor(Math.random()*n))

  for (let j=0; j<i; j++) {
    arr[j]=j
  }

  const target = Math.floor(Math.random()*i)

  const t1 = performance.now()
  for (let k=0; k<i; k++) {
    if (arr[k]===target) break
  }
  const t2 = performance.now()
  y1.push(t2-t1)

  const t3 = performance.now()
  bs(arr, target)
  const t4 = performance.now()
  y2.push(t4-t3)
}

plot([{x: labels, y: y1},{x: labels, y: y2}])

