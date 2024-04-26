import assert from "node:assert"

function bs(arr, val) {
  const mid = Math.floor(arr.length / 2)

  if (arr.length <= 1 && arr[mid] !== val) {
    return -Infinity
  }

  if (arr[mid] === val) {
    return mid || -1
  }

  if (arr[mid] < val) {
    return mid + bs(arr.splice(mid), val)
  }

  if (arr[mid] > val) {
    return mid - bs(arr.splice(0, mid), val)
  }
}

assert.equal(bs([-5,-3,0,3,5], -5), 0)
assert.equal(bs([-5,-3,0,3,5], -3), 1)
assert.equal(bs([-5,-3,0,3,5], 0), 2)
assert.equal(bs([-5,-3,0,3,5], 3), 3)
assert.equal(bs([-5,-3,0,3,5], 5), 4)
assert.equal(bs([-5,-3,0,3,5], 9), -Infinity)

