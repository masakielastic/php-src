--TEST--
InternalStrIterator::current returns first code point
--FILE--
<?php
$it = str_iter("Aéあ🍣");
var_dump($it->current());
?>
--EXPECT--
string(1) "A"
