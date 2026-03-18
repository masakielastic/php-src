--TEST--
str_iter() returns an object
--FILE--
<?php
$it = str_iter("abc");
var_dump(is_object($it));
?>
--EXPECT--
bool(true)
