"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
var strict_1 = __importDefault(require("node:assert/strict"));
function roman(n) {
    if (n === 0)
        return '';
    if (n === 4)
        return 'IV';
    if (n < 5)
        return 'I' + roman(n - 1);
    if (n === 9)
        return 'IX';
    if (n < 10)
        return 'V' + roman(n % 5);
    if (n < 40)
        return 'X' + roman(n - 10);
    if (n < 50)
        return 'XL' + roman(n - 40);
    if (n < 90)
        return 'L' + roman(n - 50);
    if (n < 100)
        return 'XC' + roman(n - 90);
    if (n < 400)
        return 'C' + roman(n - 100);
    if (n < 500)
        return 'CD' + roman(n - 400);
    if (n < 900)
        return 'D' + roman(n - 500);
    if (n < 1000)
        return 'CM' + roman(n - 900);
    if (n < 4000)
        return 'M' + roman(n - 1000);
}
strict_1.default.equal(roman(0), '');
strict_1.default.equal(roman(1), 'I');
strict_1.default.equal(roman(2), 'II');
strict_1.default.equal(roman(3), 'III');
strict_1.default.equal(roman(4), 'IV');
strict_1.default.equal(roman(5), 'V');
strict_1.default.equal(roman(6), 'VI');
strict_1.default.equal(roman(7), 'VII');
strict_1.default.equal(roman(8), 'VIII');
strict_1.default.equal(roman(9), 'IX');
strict_1.default.equal(roman(10), 'X');
strict_1.default.equal(roman(11), 'XI');
strict_1.default.equal(roman(18), 'XVIII');
strict_1.default.equal(roman(19), 'XIX');
strict_1.default.equal(roman(20), 'XX');
strict_1.default.equal(roman(30), 'XXX');
strict_1.default.equal(roman(38), 'XXXVIII');
strict_1.default.equal(roman(39), 'XXXIX');
strict_1.default.equal(roman(40), 'XL');
strict_1.default.equal(roman(41), 'XLI');
strict_1.default.equal(roman(49), 'XLIX');
strict_1.default.equal(roman(50), 'L');
strict_1.default.equal(roman(51), 'LI');
strict_1.default.equal(roman(60), 'LX');
strict_1.default.equal(roman(70), 'LXX');
strict_1.default.equal(roman(80), 'LXXX');
strict_1.default.equal(roman(81), 'LXXXI');
strict_1.default.equal(roman(89), 'LXXXIX');
strict_1.default.equal(roman(90), 'XC');
strict_1.default.equal(roman(91), 'XCI');
strict_1.default.equal(roman(98), 'XCVIII');
strict_1.default.equal(roman(99), 'XCIX');
strict_1.default.equal(roman(100), 'C');
strict_1.default.equal(roman(101), 'CI');
strict_1.default.equal(roman(200), 'CC');
strict_1.default.equal(roman(300), 'CCC');
strict_1.default.equal(roman(400), 'CD');
strict_1.default.equal(roman(500), 'D');
strict_1.default.equal(roman(600), 'DC');
strict_1.default.equal(roman(700), 'DCC');
strict_1.default.equal(roman(800), 'DCCC');
strict_1.default.equal(roman(900), 'CM');
strict_1.default.equal(roman(1000), 'M');
strict_1.default.equal(roman(2000), 'MM');
strict_1.default.equal(roman(3000), 'MMM');
strict_1.default.equal(roman(39), 'XXXIX');
strict_1.default.equal(roman(246), 'CCXLVI');
strict_1.default.equal(roman(789), 'DCCLXXXIX');
strict_1.default.equal(roman(2421), 'MMCDXXI');
