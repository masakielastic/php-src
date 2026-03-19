--TEST--
InternalStrIterator::valid becomes false at the end
--FILE--
<?php
$it = str_iter("A");

var_dump($it->valid());
$it->next();
var_dump($it->valid());
$it->next();
var_dump($it->valid());
?>
--EXPECT--
bool(true)
bool(false)
bool(false)
