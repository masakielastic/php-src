#include "php.h"
#include "ext/standard/basic_functions.h"
#include "ext/standard/php_string.h"

PHP_FUNCTION(str_iter)
{
    zend_string *string;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(string)
    ZEND_PARSE_PARAMETERS_END();

    zend_throw_error(NULL, "str_iter() is not implemented yet");
}