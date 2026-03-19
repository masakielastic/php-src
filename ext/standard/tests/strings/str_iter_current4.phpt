--TEST--
InternalStrIterator::current handles multibyte character
--FILE--
<?php
$it = str_iter("🍣");
var_dump($it->current());
?>
--EXPECT--
string(4) "🍣"
