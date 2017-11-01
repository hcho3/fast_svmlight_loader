# coding: utf-8
"""Find the path to fast_svmlight_loader dynamic library files."""

import os
import platform
import sys

class FastSVMLightLoaderLibraryNotFound(Exception):
  """Error thrown by when fast_svmlight_loader is not found"""
  pass


def find_lib_path():
  """Find the path to fast_svmlight_loader dynamic library files.

  Returns
  -------
  lib_path: :py:class:`list <python:list>` of :py:class:`str <python:str>`
     List of all found library path to fast_svmlight_loader
  """
  basename = 'fast_svmlight_loader'
  if sys.platform == 'win32':
    lib_name = '{}.dll'.format(basename)
  elif sys.platform.startswith('linux'):
    lib_name = 'lib{}.so'.format(basename)
  elif sys.platform == 'darwin':
    lib_name = 'lib{}.dylib'.format(basename)
  else:
    raise RuntimeError('Unsupported operating system')

  curr_path = os.path.dirname(os.path.abspath(os.path.expanduser(__file__)))
  # List possible locations for the library file
  dll_path = [curr_path, os.path.join(curr_path, '../../lib/'),
              os.path.join(curr_path, './lib/'),
              os.path.join(sys.prefix, 'fast_svmlight_loader')]
  # Windows hack: additional candidate locations
  if sys.platform == 'win32':
    if platform.architecture()[0] == '64bit':
      dll_path.append(os.path.join(curr_path, '../../windows/x64/Release/'))
      # hack for pip installation when copy all parent source directory here
      dll_path.append(os.path.join(curr_path, './windows/x64/Release/'))
    else:
      dll_path.append(os.path.join(curr_path, '../../windows/Release/'))
      # hack for pip installation when copy all parent source directory here
      dll_path.append(os.path.join(curr_path, './windows/Release/'))
  # Now examine all candidate locations for the library file
  dll_path = [os.path.join(p, lib_name) for p in dll_path]
  lib_path = [p for p in dll_path if os.path.exists(p) and os.path.isfile(p)]

  if not lib_path:
    raise FastSVMLightLoaderLibraryNotFound(
        'Cannot find library {} in the candidate path: '.format(lib_name) +
        'List of candidates:\n' + ('\n'.join(dll_path)))
  return lib_path

__all__ = []
