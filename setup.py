import os
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import sys
import setuptools
import subprocess as sp


class get_pybind_include(object):
    """Helper class to determine the pybind11 include path

    The purpose of this class is to postpone importing pybind11
    until it is actually installed, so that the ``get_include()``
    method can be invoked. """
    def __init__(self, user=False):
        self.user = user

    def __str__(self):
        import pybind11
        return pybind11.get_include(self.user)


def get_jpp_include():
    return os.path.join(os.getenv("JPP_DIR", ""), "software")


ext_modules = [
    Extension(
        'jppy.pdf',
        ['src/pdf.cc'],
        include_dirs=[
            get_pybind_include(),
            get_pybind_include(user=True),
            get_jpp_include()
        ],
        language='c++')
]

# Populating the __init__.py with submodule imports, so that one can import
# the package and use the submodules directly with the dot-sytax.
with open("jppy/__init__.py", "w") as fobj:
    fobj.write("""from pkg_resources import get_distribution, DistributionNotFound
try:
    version = get_distribution(__name__).version
except DistributionNotFound:
    version = "unknown version"
""")
    for module in ext_modules:
        fobj.write("from . import {}\n".format(module.name.split('.')[1]))
        

# As of Python 3.6, CCompiler has a `has_flag` method.
# cf http://bugs.python.org/issue26689
def has_flag(compiler, flagname):
    """Return a boolean indicating whether a flag name is supported on
    the specified compiler.
    """
    import tempfile
    with tempfile.NamedTemporaryFile('w', suffix='.cpp') as f:
        f.write('int main (int argc, char **argv) { return 0; }')
        try:
            compiler.compile([f.name], extra_postargs=[flagname])
        except setuptools.distutils.errors.CompileError:
            return False
    return True


def cpp_flag(compiler):
    """Return the -std=c++[11/14/17] compiler flag.

    The newer version is prefered over c++11 (when it is available).
    """
    flags = ['-std=c++14', '-std=c++11']

    for flag in flags:
        if has_flag(compiler, flag): return flag

    raise RuntimeError('Unsupported compiler -- at least C++11 support '
                       'is needed!')


class BuildExt(build_ext):
    """A custom build extension for adding compiler-specific options."""
    c_opts = {
        'msvc': ['/EHsc'],
        'unix': [],
    }
    l_opts = {
        'msvc': [],
        'unix': [],
    }

    if sys.platform == 'darwin':
        darwin_opts = ['-stdlib=libc++', '-mmacosx-version-min=10.7']
        c_opts['unix'] += darwin_opts
        l_opts['unix'] += darwin_opts

    def build_extensions(self):
        ct = self.compiler.compiler_type
        opts = self.c_opts.get(ct, [])
        link_opts = self.l_opts.get(ct, [])
        if ct == 'unix':
            opts.append('-DVERSION_INFO="%s"' %
                        self.distribution.get_version())
            opts.append(cpp_flag(self.compiler))
            if has_flag(self.compiler, '-fvisibility=hidden'):
                opts.append('-fvisibility=hidden')
        elif ct == 'msvc':
            opts.append('/DVERSION_INFO=\\"%s\\"' %
                        self.distribution.get_version())
        for ext in self.extensions:
            ext.extra_compile_args = opts
            ext.extra_link_args = link_opts
        build_ext.build_extensions(self)


setup(
    name='jppy',
    author='Tamas Gal',
    author_email='tgal@km3net.de',
    url='https://git.km3net.de/km3py/jppy',
    description='Jpp Python Package',
    packages=["jppy"],
    long_description="jppy - Jpp Python Package",
    ext_modules=ext_modules,
    install_requires=['pybind11>=2.4'],
    setup_requires=['pybind11>=2.4', 'setuptools_scm'],
    use_scm_version=True,
    cmdclass={'build_ext': BuildExt},
    zip_safe=False,
)
