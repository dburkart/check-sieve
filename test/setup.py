from setuptools import Extension, setup

module = Extension('checksieve',
                    extra_compile_args = ['-std=c++17'],
                    sources = ['src/python.cc'],
                    include_dirs = ['src', 'src/AST', 'gen'],
                    libraries = ['checksieve'],
                    library_dirs = ['./'] );

setup (name = 'CheckSieve',
        version = '0.11-dev',
        description = 'Python interface to libchecksieve',
        author = 'Dana Burkart',
        author_email = 'dana.burkart@gmail.com',
        ext_modules = [module] );
