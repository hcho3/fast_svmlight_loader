# coding: utf-8
"""Fast SVMLight loader"""
from __future__ import absolute_import as _abs

import ctypes
import numpy as np
from scipy.sparse import csr_matrix

from .libpath import find_lib_path
from .compat import py_str, buffer_from_memory

def c_str(string):
  """Convert a Python string to C string"""
  return ctypes.c_char_p(string.encode('utf-8'))

class FastSVMLightLoaderError(Exception):
  """Error thrown by fast_svmlight_loader"""
  pass

def _log_callback(msg):
  """Redirect logs from native library into Python console"""
  print("{0:s}".format(py_str(msg)))

def _get_log_callback_func():
  """Wrap log_callback() method in ctypes callback type"""
  #pylint: disable=invalid-name
  CALLBACK = ctypes.CFUNCTYPE(None, ctypes.c_char_p)
  return CALLBACK(_log_callback)

def _load_lib():
  """Load fast_svmlight_loader Library."""
  lib_path = find_lib_path()
  lib = ctypes.cdll.LoadLibrary(lib_path[0])
  lib.FastSVMLightLoaderGetLastError.restype = ctypes.c_char_p
  lib.callback = _get_log_callback_func()
  if lib.FastSVMLightLoaderRegisterLogCallback(lib.callback) != 0:
    raise FastSVMLightLoaderError(lib.FastSVMLightLoaderGetLastError())
  return lib

_LIB = _load_lib()

def _check_call(ret):
  """Check the return value of C API call

  This function will raise exception when error occurs.
  Wrap every API call with this function

  Parameters
  ----------
  ret : int
      return value from API calls
  """
  if ret != 0:
    raise FastSVMLightLoaderError(_LIB.FastSVMLightLoaderGetLastError())

def load_svmlight(filename, verbose=False, nthread=None):
  """
  Data matrix

  Parameters
  ----------
  filename : :py:class:`str <python:str>`
      name of a SVMLight data file
  verbose : :py:class:`bool <python:bool>`, optional
      Whether to print extra messages during construction
  nthread : :py:class:`int <python:int>`, optional
      Number of threads

  Returns
  -------
  csr_mat : :py:class:`scipy.sparse.csr_matrix` object
      loaded sparse matrix
  """
  handle = ctypes.c_void_p()
  nthread = nthread if nthread is not None else 0
  _check_call(_LIB.FastSVMLightLoaderDMatrixCreateFromFile(
      c_str(filename),
      ctypes.c_int(nthread),
      ctypes.c_int(1 if verbose else 0),
      ctypes.byref(handle)))

  # Extract internal arrays
  data = ctypes.POINTER(ctypes.c_float)()
  col_ind = ctypes.POINTER(ctypes.c_uint32)()
  row_ptr = ctypes.POINTER(ctypes.c_size_t)()
  labels = ctypes.POINTER(ctypes.c_float)()
  weights = ctypes.POINTER(ctypes.c_float)()
  _check_call(_LIB.FastSVMLightLoaderDMatrixGetArrays(
      handle,
      ctypes.byref(data),
      ctypes.byref(col_ind),
      ctypes.byref(row_ptr),
      ctypes.byref(labels),
      ctypes.byref(weights)))
  num_row = ctypes.c_size_t()
  num_col = ctypes.c_size_t()
  nelem = ctypes.c_size_t()
  _check_call(_LIB.FastSVMLightLoaderDMatrixGetDimension(
      handle,
      ctypes.byref(num_row),
      ctypes.byref(num_col),
      ctypes.byref(nelem)))

  # Return value dict
  retval = {}

  # Process internal arrays into Scipy format
  scipy_data = np.copy(np.frombuffer(
      buffer_from_memory(
          data,
          ctypes.sizeof(ctypes.c_float * nelem.value)),
      dtype=np.float32))
  scipy_indices = np.copy(np.frombuffer(
      buffer_from_memory(
          col_ind,
          ctypes.sizeof(ctypes.c_uint32 * nelem.value)),
      dtype=np.uint32))
  scipy_indptr = np.copy(np.frombuffer(
      buffer_from_memory(
          row_ptr,
          ctypes.sizeof(ctypes.c_size_t * (num_row.value + 1))),
      dtype=np.uintp))
  retval['data'] = csr_matrix((scipy_data, scipy_indices, scipy_indptr),
                              shape=(num_row.value, num_col.value))

  # Check existence of labels and weights
  if labels:
    retval['labels'] = np.copy(np.frombuffer(
        buffer_from_memory(
            labels,
            ctypes.sizeof(ctypes.c_float * num_row.value)),
        dtype=np.float32))
  if weights:
    retval['weights'] = np.copy(np.frombuffer(
        buffer_from_memory(
            weights,
            ctypes.sizeof(ctypes.c_float) * num_row.value),
        dtype=np.float32))
  _check_call(_LIB.FastSVMLightLoaderDMatrixFree(handle))
  handle = None

  return retval

__all__ = ['load_svmlight']
