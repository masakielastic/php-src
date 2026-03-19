--TEST--
InternalStrIterator::next is stable past the end
--FILE--
<?php
$it = str_iter("A");

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
NULL
NULL
NULL
