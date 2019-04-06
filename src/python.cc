/*
 * python.cc
 *
 * This python module lets us easily test libchecksieve using python. The
 * 'test' target will automatically compile this against libchecksieve.a
 */

#include <iostream>
#include <Python.h>

#include "checksieve.h"
#include "sieve_driver.hh"

static PyObject *parse_string(PyObject *, PyObject *);
static PyObject *parse_string_with_options(PyObject *self, PyObject *args);

static PyMethodDef checksieve_methods[] = {
    {"parse_string", parse_string, METH_VARARGS, "guhhh"},
    {"parse_string_with_options", parse_string_with_options, METH_VARARGS, "Parse a sieve buffer with the specified options"},
    {NULL, NULL}
};

#define PY_MODNAME "checksieve"
#define PY_MODDESC "syntax-check a mail sieve"

#if PY_MAJOR_VERSION >= 3
#define PY_MODINIT(name) PyMODINIT_FUNC PyInit_##name(void)

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    PY_MODNAME,
    PY_MODDESC,
    -1,
    checksieve_methods,
    NULL,
    NULL,
    NULL,
    NULL,
};

#define PY_MODCREATE() return PyModule_Create(&moduledef)
#else
#define PY_MODINIT(name) PyMODINIT_FUNC init##name(void)
#define PY_MODCREATE() Py_InitModule3( PY_MODNAME, checksieve_methods, PY_MODDESC )
#endif // PY_MAJOR_VERSION >= 3

static PyObject *parse_string(PyObject *self, PyObject *args) {
    const char *sieve;
    PyObject *quiet;

    if (!PyArg_ParseTuple(args, "sO:parse_string", &sieve, &quiet))
        return NULL;

    sieve::driver driver;

    sieve::parse_result result = driver.parse_string(sieve);

    if (quiet == Py_False && result.status) {
        std::cerr << result.error << std::endl;
    }

    return Py_BuildValue("i", result.status);
}

static PyObject *parse_string_with_options(PyObject *self, PyObject *args) {
    const char *sieve;
    PyObject *options;
    PyObject *value;
    struct sieve::parse_options opts;

    if (!PyArg_ParseTuple(args, "sO:parse_string", &sieve, &options))
        return NULL;

    // TODO: Set error.
    if (!PyDict_Check(options))
        return NULL;

    // Pull out max line length
    value = PyDict_GetItem(options, PyUnicode_FromString("string_list_max_length"));
    if (value != NULL) {
        opts.string_list_max_length = int(PyLong_AsLong(value));
    }

    sieve::driver driver(opts);
    return Py_BuildValue("i", driver.parse_string(sieve).status);
}

PY_MODINIT(checksieve) {
    PY_MODCREATE();
}
