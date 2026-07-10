--TEST--
filter_descriptors() builds filter_var_array() runtime descriptors
--EXTENSIONS--
filter
--FILE--
<?php
function filter_descriptors_callback(string $value): string {
    return strtoupper($value);
}

$closure = static fn (string $value): string => trim($value);
$descriptors = filter_descriptors([
    'email' => ['validate' => 'email'],
    'age' => ['validate' => 'int', 'options' => ['min_range' => 18, 'max_range' => 120]],
    'bool' => ['validate' => 'bool'],
    'boolean' => ['validate' => 'boolean'],
    'homepage' => ['sanitize' => 'url'],
    'amount' => ['sanitize' => 'number_float', 'flags' => FILTER_FLAG_ALLOW_FRACTION],
    'message' => ['callback' => 'filter_descriptors_callback'],
    'trimmed' => ['callback' => $closure],
]);

var_dump(
    $descriptors['email']['filter'] === FILTER_VALIDATE_EMAIL,
    $descriptors['age'],
    $descriptors['bool']['filter'] === FILTER_VALIDATE_BOOL,
    $descriptors['boolean']['filter'] === FILTER_VALIDATE_BOOL,
    $descriptors['homepage']['filter'] === FILTER_SANITIZE_URL,
    $descriptors['amount'],
    $descriptors['message']['filter'] === FILTER_CALLBACK,
    $descriptors['message']['options'] === 'filter_descriptors_callback',
    $descriptors['trimmed']['filter'] === FILTER_CALLBACK,
    $descriptors['trimmed']['options'] === $closure,
);

var_dump(filter_var_array([
    'email' => 'user@example.com',
    'age' => '21',
    'homepage' => 'https://example.com/?a=<b>',
    'message' => 'hello',
    'trimmed' => ' hello ',
], $descriptors));
?>
--EXPECT--
bool(true)
array(2) {
  ["filter"]=>
  int(257)
  ["options"]=>
  array(2) {
    ["min_range"]=>
    int(18)
    ["max_range"]=>
    int(120)
  }
}
bool(true)
bool(true)
bool(true)
array(2) {
  ["filter"]=>
  int(520)
  ["flags"]=>
  int(4096)
}
bool(true)
bool(true)
bool(true)
bool(true)
array(8) {
  ["email"]=>
  string(16) "user@example.com"
  ["age"]=>
  int(21)
  ["bool"]=>
  NULL
  ["boolean"]=>
  NULL
  ["homepage"]=>
  string(26) "https://example.com/?a=<b>"
  ["amount"]=>
  NULL
  ["message"]=>
  string(5) "HELLO"
  ["trimmed"]=>
  string(5) "hello"
}
