from distutils.core import setup, Extension

module = Extension('checksieve',
                    sources = ['src/python.cc'],
                    include_dirs = ['src', 'gen'],
                    libraries = ['checksieve'],
                    library_dirs = ['./'] );

setup (name = 'CheckSieve',
        version = '0.1',
        description = 'Python interface to libchecksieve',
        author = 'Dana Burkart',
        author_email = 'dana.burkart@gmail.com',
        ext_modules = [module] );