--TEST--
str_iter() exists
--FILE--
<?php
var_dump(function_exists('str_iter'));
?>
--EXPECT--
bool(true)
