from distutils.core import setup, Extension
from distutils.sysconfig import get_python_inc

module = Extension('checksieve',
                    sources = ['src/python.cc'],
                    include_dirs = ['src', 'gen', get_python_inc(), get_python_inc(plat_specific=True)],
                    libraries = ['checksieve'],
                    library_dirs = ['./'] );

setup (name = 'CheckSieve',
        version = '0.1',
        description = 'Python interface to libchecksieve',
        author = 'Dana Burkart',
        author_email = 'dana.burkart@gmail.com',
        ext_modules = [module] );
