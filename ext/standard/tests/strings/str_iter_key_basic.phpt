--TEST--
InternalStrIterator::key returns the current code point index
--FILE--
<?php
$it = str_iter("Aéあ");

while ($it->valid()) {
    var_dump($it->key(), $it->current());
    $it->next();
}
?>
--EXPECT--
int(0)
string(1) "A"
int(1)
string(2) "é"
int(2)
string(3) "あ"
