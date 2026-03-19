--TEST--
InternalStrIterator::key resets after rewind
--FILE--
<?php
$it = str_iter("Aé");

var_dump($it->key());
$it->next();
var_dump($it->key());
$it->rewind();
var_dump($it->key());
var_dump($it->current());
?>
--EXPECT--
int(0)
int(1)
int(0)
string(1) "A"
