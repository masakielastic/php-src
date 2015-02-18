--TEST--
Bug #68846	False detection of CJK Unified Ideographs Extension E
--SKIPIF--
<?php extension_loaded('mbstring') or die('skip mbstring not available'); ?>
--FILE--
<?php
var_dump(
	"\x00\x02\x0b\xb7" === mb_convert_encoding("\x95\x34\xb2\x35", 'UTF-32', 'GB18030')
);
?>
--EXPECT--
bool(true)