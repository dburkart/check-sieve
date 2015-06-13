#include <python.h>

#include "checksieve.h"
#include "sieve_driver.hh"

static PyObject *parse_string(PyObject *self, PyObject *args) {
    const char *sieve;
    PyObject *quiet;
    
    if (!PyArg_ParseTuple(args, "sO:parse_string", &sieve, &quiet))
        return NULL;
    
    sieve_driver driver(PyObject_IsTrue(quiet));
    return Py_BuildValue("i", driver.parse_string(sieve));
}

static PyMethodDef checksieve_methods[] = {
    {"parse_string", parse_string, METH_VARARGS, "guhhh"},
    {NULL, NULL}
};

PyMODINIT_FUNC initchecksieve(void) {
    Py_InitModule3( "checksieve", checksieve_methods, "syntax-check a sieve");
}