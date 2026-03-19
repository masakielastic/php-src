--TEST--
InternalStrIterator::next advances to the next code point
--FILE--
<?php
$it = str_iter("Aéあ🍣");

var_dump($it->current());
$it->next();
var_dump($it->current());
$it->next();
var_dump($it->current());
$it->next();
var_dump($it->current());
$it->next();
var_dump($it->current());
?>
--EXPECT--
string(1) "A"
string(2) "é"
string(3) "あ"
string(4) "🍣"
NULL
