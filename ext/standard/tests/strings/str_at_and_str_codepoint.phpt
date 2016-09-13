--TEST--
Testing str_at() and str_codepoint()
--FILE--
<?php

// U+1D400: MATHEMATICAL BOLD CAPITAL A
$str = "\u{1D400}\u{1D401}\u{1D402}";
var_dump(
    "\u{1D400}" === str_at($str, 0),
    "\u{1D402}" === str_at($str, -1),
    false === str_at($str, 4),
    0x1D400 === str_codepoint_at($str, 0),
    0x1D402 === str_codepoint_at($str, -1),
    false === str_codepoint_at($str, 4)
);
?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
