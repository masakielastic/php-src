--TEST--
InternalStrIterator::current returns null for empty string
--FILE--
<?php
$it = str_iter("");
var_dump($it->current());
?>
--EXPECT--
NULL
