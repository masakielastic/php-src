--TEST--
InternalStrIterator::next is stable for empty string
--FILE--
<?php
$it = str_iter("");

var_dump($it->current());
$it->next();
var_dump($it->current());
?>
--EXPECT--
NULL
NULL
