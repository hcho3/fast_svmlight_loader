"""Setup script"""
import os
import shutil
from setuptools import setup, Distribution, find_packages

class BinaryDistribution(Distribution):
  """Overrides Distribution class to bundle platform-specific binaries"""
  # pylint: disable=R0201
  def has_ext_modules(self):
    """Has an extension module"""
    return True

LIBPATH_PY = os.path.abspath('./fast_svmlight_loader/libpath.py')
LIBPATH = {'__file__': LIBPATH_PY}
# pylint: disable=W0122
exec(compile(open(LIBPATH_PY, "rb").read(), LIBPATH_PY, 'exec'),
     LIBPATH, LIBPATH)

# Paths for C/C++ libraries
LIB_PATH = LIBPATH['find_lib_path']()

if not LIB_PATH:
  raise RuntimeError('Please compile the C++ package first')

# ignore libraries already in python/fast_svmlight_loader; only use ones in ../lib
if os.path.abspath(os.path.dirname(LIB_PATH[0])) \
   == os.path.abspath('./fast_svmlight_loader'):
  del LIB_PATH[0]

LIB_BASENAME = os.path.basename(LIB_PATH[0])
LIB_DEST = os.path.join('./fast_svmlight_loader', LIB_BASENAME)

# remove stale copies of library
if os.path.exists(LIB_DEST):
  os.remove(LIB_DEST)
shutil.copy(LIB_PATH[0], LIB_DEST)

with open('../VERSION', 'r') as f:
  VERSION = f.readlines()[0].rstrip('\n')

setup(
    name='fast_svmlight_loader',
    version=VERSION,
    description='fast_svmlight_loader: fast reader for SVMLight file',
    url='https://github.com/hcho3/fast_svmlight_loader',
    author='Philip Cho',
    maintainer='Philip Cho',
    maintainer_email='chohyu01@cs.washington.edu',
    packages=find_packages(),
    package_data={
        'fast_svmlight_loader': [LIB_BASENAME]
    },
    distclass=BinaryDistribution
)
