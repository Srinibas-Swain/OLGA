
from distutils.core import setup
from Cython.Build import cythonize

setup(
    name = 'h2py',
    packages = ['generic'],
    ext_modules=cythonize("**/*pyx")
)