/*
 * python.cc
 *
 * This python module lets us easily test libchecksieve using python. The
 * 'test' target will automatically compile this against libchecksieve.a
 */

#include <Python.h>

#include "checksieve.h"
#include "sieve_driver.hh"

static PyObject *parse_string(PyObject *, PyObject *);

static PyMethodDef checksieve_methods[] = {
    {"parse_string", parse_string, METH_VARARGS, "guhhh"},
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

#define PY_MODCREATE() PyModule_Create(&moduledef)
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
    return Py_BuildValue("i", driver.parse_string(sieve).status);
}

PY_MODINIT(checksieve) {
    PY_MODCREATE();
}
