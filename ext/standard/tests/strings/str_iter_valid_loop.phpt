--TEST--
InternalStrIterator::valid works with current and next
--FILE--
<?php
$it = str_iter("Aéあ");

while ($it->valid()) {
    var_dump($it->current());
    $it->next();
}
?>
--EXPECT--
string(1) "A"
string(2) "é"
string(3) "あ"
