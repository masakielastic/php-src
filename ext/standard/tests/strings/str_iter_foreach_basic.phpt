--TEST--
str_iter supports foreach iteration over UTF-8 code points
--FILE--
<?php
foreach (str_iter("Aéあ🍣") as $k => $v) {
    var_dump($k, $v);
}
?>
--EXPECT--
int(0)
string(1) "A"
int(1)
string(2) "é"
int(2)
string(3) "あ"
int(3)
string(4) "🍣"
