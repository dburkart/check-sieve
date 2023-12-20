from distutils.core import setup, Extension
from distutils.sysconfig import get_python_inc

module = Extension('checksieve',
                    extra_compile_args = ['-std=c++17'],
                    sources = ['src/python.cc'],
                    include_dirs = ['src', 'src/AST', 'gen', get_python_inc(), get_python_inc(plat_specific=True)],
                    libraries = ['checksieve'],
                    library_dirs = ['./'] );

setup (name = 'CheckSieve',
        version = '1.0-dev',
        description = 'Python interface to libchecksieve',
        author = 'Dana Burkart',
        author_email = 'dana.burkart@gmail.com',
        ext_modules = [module] );
