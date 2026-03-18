#include "php.h"
#include "ext/standard/basic_functions.h"
#include "ext/standard/php_string.h"

typedef struct _php_str_iter_object {
    zend_object std;
    zend_string *str;
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

PHP_MINIT_FUNCTION(str_iter)
{
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "InternalStrIterator", NULL);
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

    RETURN_ZVAL(&obj, 0, 1);
}

static zend_object *php_str_iter_create_object(zend_class_entry *ce)
{
    php_str_iter_object *intern;

    intern = zend_object_alloc(sizeof(php_str_iter_object), ce);
    intern->str = NULL;

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
