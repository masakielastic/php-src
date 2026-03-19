#include "php.h"
#include "ext/standard/basic_functions.h"
#include "ext/standard/php_string.h"

typedef struct _php_str_iter_object {
    zend_object std;
    zend_string *str;
    size_t offset;
    zend_long index;
} php_str_iter_object;

static zend_class_entry *php_str_iter_ce;
static zend_object_handlers php_str_iter_object_handlers;


static inline php_str_iter_object *php_str_iter_from_obj(zend_object *obj)
{
    return (php_str_iter_object *)((char *)(obj) - XtOffsetOf(php_str_iter_object, std));
}

#define Z_STR_ITER_OBJ_P(zv) php_str_iter_from_obj(Z_OBJ_P((zv)))

static zend_object *php_str_iter_create_object(zend_class_entry *ce);
static void php_str_iter_free_object(zend_object *object);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_str_iter_current, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_str_iter_next, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_str_iter_valid, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_str_iter_rewind, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_str_iter_key, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(InternalStrIterator, current);
PHP_METHOD(InternalStrIterator, next);
PHP_METHOD(InternalStrIterator, valid);
PHP_METHOD(InternalStrIterator, rewind);
PHP_METHOD(InternalStrIterator, key);

static const zend_function_entry php_str_iter_class_methods[] = {
    ZEND_ME(InternalStrIterator, current, arginfo_str_iter_current, ZEND_ACC_PUBLIC)
    ZEND_ME(InternalStrIterator, next, arginfo_str_iter_next, ZEND_ACC_PUBLIC)
    ZEND_ME(InternalStrIterator, valid, arginfo_str_iter_valid, ZEND_ACC_PUBLIC)
    ZEND_ME(InternalStrIterator, rewind, arginfo_str_iter_rewind, ZEND_ACC_PUBLIC)
    ZEND_ME(InternalStrIterator, key, arginfo_str_iter_key, ZEND_ACC_PUBLIC)
    ZEND_FE_END
};

PHP_MINIT_FUNCTION(str_iter)
{
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "InternalStrIterator", php_str_iter_class_methods);
    php_str_iter_ce = zend_register_internal_class(&ce);
    php_str_iter_ce->create_object = php_str_iter_create_object;

    memcpy(&php_str_iter_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
    php_str_iter_object_handlers.offset = XtOffsetOf(php_str_iter_object, std);
    php_str_iter_object_handlers.free_obj = php_str_iter_free_object;

    return SUCCESS;
}

PHP_FUNCTION(str_iter)
{
    zend_string *string;
    zval obj;
    php_str_iter_object *intern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(string)
    ZEND_PARSE_PARAMETERS_END();

    object_init_ex(&obj, php_str_iter_ce);

    intern = Z_STR_ITER_OBJ_P(&obj);
    intern->str = zend_string_copy(string);
    intern->offset = 0;
    intern->index = 0;

    RETURN_ZVAL(&obj, 0, 1);
}

static zend_object *php_str_iter_create_object(zend_class_entry *ce)
{
    php_str_iter_object *intern;

    intern = zend_object_alloc(sizeof(php_str_iter_object), ce);
    intern->str = NULL;
    intern->offset = 0;
    intern->index = 0;

    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);

    intern->std.handlers = &php_str_iter_object_handlers;

    return &intern->std;
}

static void php_str_iter_free_object(zend_object *object)
{
    php_str_iter_object *intern = php_str_iter_from_obj(object);

    if (intern->str != NULL) {
        zend_string_release(intern->str);
    }

    zend_object_std_dtor(&intern->std);
}

static size_t php_str_iter_utf8_char_len(const unsigned char *p, size_t remaining)
{
    if (remaining == 0) {
        return 0;
    }

    if (p[0] < 0x80) {
        return 1;
    }

    if ((p[0] & 0xE0) == 0xC0) {
        return remaining >= 2 ? 2 : 1;
    }

    if ((p[0] & 0xF0) == 0xE0) {
        return remaining >= 3 ? 3 : 1;
    }

    if ((p[0] & 0xF8) == 0xF0) {
        return remaining >= 4 ? 4 : 1;
    }

    return 1;
}

static void php_str_iter_reset(php_str_iter_object *intern)
{
    intern->offset = 0;
    intern->index = 0;
}

static zend_bool php_str_iter_valid(php_str_iter_object *intern)
{
    if (intern->str == NULL) {
        return 0;
    }

    return intern->offset < ZSTR_LEN(intern->str);
}


static void php_str_iter_move_forward(php_str_iter_object *intern)
{
    const unsigned char *buf;
    size_t len;
    size_t char_len;

    if (intern->str == NULL) {
        return;
    }

    len = ZSTR_LEN(intern->str);

    if (intern->offset >= len) {
        return;
    }

    buf = (const unsigned char *) ZSTR_VAL(intern->str);
    char_len = php_str_iter_utf8_char_len(buf + intern->offset, len - intern->offset);

    intern->offset += char_len;
    intern->index++;
}

static zend_string *php_str_iter_current(php_str_iter_object *intern)
{
    const unsigned char *buf;
    size_t len;
    size_t char_len;

    if (intern->str == NULL) {
        return NULL;
    }

    len = ZSTR_LEN(intern->str);

    if (intern->offset >= len) {
        return NULL;
    }

    buf = (const unsigned char *) ZSTR_VAL(intern->str);
    char_len = php_str_iter_utf8_char_len(buf + intern->offset, len - intern->offset);

    return zend_string_init(
        ZSTR_VAL(intern->str) + intern->offset,
        char_len,
        0
    );
}

PHP_FUNCTION(str_iter_debug_first)
{
    zval *obj;
    php_str_iter_object *intern;
    zend_string *current;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(obj, php_str_iter_ce)
    ZEND_PARSE_PARAMETERS_END();

    intern = Z_STR_ITER_OBJ_P(obj);
    php_str_iter_reset(intern);

    current = php_str_iter_current(intern);
    if (current == NULL) {
        RETURN_NULL();
    }

    RETVAL_STR(current);
}


PHP_FUNCTION(str_iter_debug_next)
{
    zval *obj;
    php_str_iter_object *intern;
    zend_string *current;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(obj, php_str_iter_ce)
    ZEND_PARSE_PARAMETERS_END();

    intern = Z_STR_ITER_OBJ_P(obj);
    php_str_iter_move_forward(intern);

    current = php_str_iter_current(intern);
    if (current == NULL) {
        RETURN_NULL();
    }

    RETVAL_STR(current);
}

PHP_METHOD(InternalStrIterator, current)
{
    php_str_iter_object *intern;
    zend_string *current;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = Z_STR_ITER_OBJ_P(ZEND_THIS);

    current = php_str_iter_current(intern);
    if (current == NULL) {
        RETURN_NULL();
    }

    RETURN_STR(current);
}

PHP_METHOD(InternalStrIterator, next)
{
    php_str_iter_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = Z_STR_ITER_OBJ_P(ZEND_THIS);
    php_str_iter_move_forward(intern);
}

PHP_METHOD(InternalStrIterator, valid)
{
    php_str_iter_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = Z_STR_ITER_OBJ_P(ZEND_THIS);
    RETURN_BOOL(php_str_iter_valid(intern));
}

PHP_METHOD(InternalStrIterator, rewind)
{
    php_str_iter_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = Z_STR_ITER_OBJ_P(ZEND_THIS);
    php_str_iter_reset(intern);
}

PHP_METHOD(InternalStrIterator, key)
{
    php_str_iter_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = Z_STR_ITER_OBJ_P(ZEND_THIS);
    RETURN_LONG(intern->index);
}