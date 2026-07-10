--TEST--
filter_descriptors() rejects invalid specifications
--EXTENSIONS--
filter
--FILE--
<?php
$cases = [
    'filter key' => ['field' => ['filter' => FILTER_VALIDATE_EMAIL]],
    'multiple filters' => ['field' => ['validate' => 'email', 'sanitize' => 'email']],
    'validate callback' => ['field' => ['validate' => 'email', 'callback' => 'trim']],
    'no filter' => ['field' => []],
    'non-array field' => ['field' => 'email'],
    'non-string validate' => ['field' => ['validate' => 1]],
    'non-string sanitize' => ['field' => ['sanitize' => 1]],
    'unknown validate' => ['field' => ['validate' => 'string']],
    'unknown sanitize' => ['field' => ['sanitize' => 'string']],
    'validate options' => ['field' => ['validate' => 'int', 'options' => 1]],
    'validate flags' => ['field' => ['validate' => 'int', 'flags' => 'x']],
    'sanitize options' => ['field' => ['sanitize' => 'url', 'options' => 1]],
    'sanitize flags' => ['field' => ['sanitize' => 'url', 'flags' => 'x']],
    'invalid callback' => ['field' => ['callback' => 'does_not_exist']],
    'callback options' => ['field' => ['callback' => 'trim', 'options' => []]],
    'callback flags' => ['field' => ['callback' => 'trim', 'flags' => 0]],
    'unknown key' => ['field' => ['validate' => 'email', 'unknown' => true]],
];

foreach ($cases as $name => $spec) {
    try {
        filter_descriptors($spec);
    } catch (ValueError $e) {
        echo "$name: ", $e->getMessage(), "\n";
    }
}
?>
--EXPECTF--
filter key: filter_descriptors(): Field "field" must not contain the "filter" key
multiple filters: filter_descriptors(): Field "field" must contain exactly one of "validate", "sanitize", or "callback"
validate callback: filter_descriptors(): Field "field" must contain exactly one of "validate", "sanitize", or "callback"
no filter: filter_descriptors(): Field "field" must contain exactly one of "validate", "sanitize", or "callback"
non-array field: filter_descriptors(): Field "field" must be an array
non-string validate: filter_descriptors(): Field "field" validate must be a string
non-string sanitize: filter_descriptors(): Field "field" sanitize must be a string
unknown validate: filter_descriptors(): Field "field" has an unknown validate filter
unknown sanitize: filter_descriptors(): Field "field" has an unknown sanitize filter
validate options: filter_descriptors(): Field "field" options must be an array
validate flags: filter_descriptors(): Field "field" flags must be an int
sanitize options: filter_descriptors(): Field "field" options must be an array
sanitize flags: filter_descriptors(): Field "field" flags must be an int
invalid callback: filter_descriptors(): Field "field" callback must be callable
callback options: filter_descriptors(): Field "field" callback must not contain "options" or "flags"
callback flags: filter_descriptors(): Field "field" callback must not contain "options" or "flags"
unknown key: filter_descriptors(): Field "field" contains an unknown key
