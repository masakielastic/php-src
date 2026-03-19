--TEST--
InternalStrIterator::rewind resets iteration to the beginning
--FILE--
<?php
$it = str_iter("Aéあ");

var_dump($it->current());
$it->next();
var_dump($it->current());
$it->rewind();
var_dump($it->current());
?>
--EXPECT--
string(1) "A"
string(2) "é"
string(1) "A"
