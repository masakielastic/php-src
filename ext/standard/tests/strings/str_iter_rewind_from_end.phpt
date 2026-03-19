--TEST--
InternalStrIterator::rewind works after reaching the end
--FILE--
<?php
$it = str_iter("Aé");

while ($it->valid()) {
    var_dump($it->current());
    $it->next();
}

var_dump($it->current());

$it->rewind();

var_dump($it->valid());
var_dump($it->current());
?>
--EXPECT--
string(1) "A"
string(2) "é"
NULL
bool(true)
string(1) "A"
