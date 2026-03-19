--TEST--
str_iter_debug_first() returns first code point
--FILE--
<?php
$it = str_iter("Aéあ🍣");
var_dump(str_iter_debug_first($it));
?>
--EXPECT--
string(1) "A"
