import assert from "node:assert/strict"

function roman(n: number) {
  if (n === 0) return ''
  if (n === 4) return 'IV'
  if (n <   5) return 'I'  + roman(n-1)
  if (n === 9) return 'IX'
  if (n <  10) return 'V'  + roman(n%5)
  if (n <  40) return 'X'  + roman(n-10)
  if (n <  50) return 'XL' + roman(n-40)
  if (n <  90) return 'L'  + roman(n-50)
  if (n < 100) return 'XC' + roman(n-90)
  if (n < 400) return 'C'  + roman(n-100)
  if (n < 500) return 'CD' + roman(n-400)
  if (n < 900) return 'D'  + roman(n-500)
  if (n <1000) return 'CM' + roman(n-900)
  if (n <4000) return 'M'  + roman(n-1000)
}

assert.equal(roman(0), '')

assert.equal(roman(1), 'I')
assert.equal(roman(2), 'II')
assert.equal(roman(3), 'III')
assert.equal(roman(4), 'IV')

assert.equal(roman(5), 'V')
assert.equal(roman(6), 'VI')
assert.equal(roman(7), 'VII')
assert.equal(roman(8), 'VIII')
assert.equal(roman(9), 'IX')

assert.equal(roman(10), 'X')
assert.equal(roman(11), 'XI')
assert.equal(roman(18), 'XVIII')
assert.equal(roman(19), 'XIX')
assert.equal(roman(20), 'XX')
assert.equal(roman(30), 'XXX')
assert.equal(roman(38), 'XXXVIII')
assert.equal(roman(39), 'XXXIX')
assert.equal(roman(40), 'XL')
assert.equal(roman(41), 'XLI')
assert.equal(roman(49), 'XLIX')

assert.equal(roman(50), 'L')
assert.equal(roman(51), 'LI')
assert.equal(roman(60), 'LX')
assert.equal(roman(70), 'LXX')
assert.equal(roman(80), 'LXXX')
assert.equal(roman(81), 'LXXXI')
assert.equal(roman(89), 'LXXXIX')

assert.equal(roman(90), 'XC')
assert.equal(roman(91), 'XCI')
assert.equal(roman(98), 'XCVIII')
assert.equal(roman(99), 'XCIX')

assert.equal(roman(100), 'C')
assert.equal(roman(101), 'CI')
assert.equal(roman(200), 'CC')
assert.equal(roman(300), 'CCC')

assert.equal(roman(400), 'CD')
assert.equal(roman(500), 'D')
assert.equal(roman(600), 'DC')
assert.equal(roman(700), 'DCC')
assert.equal(roman(800), 'DCCC')

assert.equal(roman(900), 'CM')

assert.equal(roman(1000), 'M')
assert.equal(roman(2000), 'MM')
assert.equal(roman(3000), 'MMM')

assert.equal(roman(39), 'XXXIX')
assert.equal(roman(246), 'CCXLVI')
assert.equal(roman(789), 'DCCLXXXIX')
assert.equal(roman(2421), 'MMCDXXI')

