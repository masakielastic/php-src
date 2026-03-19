--TEST--
InternalStrIterator::valid is false for empty string
--FILE--
<?php
$it = str_iter("");

var_dump($it->valid());
var_dump($it->current());
?>
--EXPECT--
bool(false)
NULL
