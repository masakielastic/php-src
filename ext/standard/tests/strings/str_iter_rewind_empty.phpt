--TEST--
InternalStrIterator::rewind is stable for empty string
--FILE--
<?php
$it = str_iter("");

var_dump($it->valid());
$it->rewind();
var_dump($it->valid());
var_dump($it->current());
?>
--EXPECT--
bool(false)
bool(false)
NULL
