"""compatibility layer"""
import sys
import ctypes

PY3 = (sys.version_info[0] == 3)
# String handling for Python 2 and 3
if PY3:
  STRING_TYPES = (str,)
  def py_str(string):
    """Convert C string back to Python string"""
    return string.decode('utf-8')
else:
  STRING_TYPES = (basestring,)   # pylint: disable=E0602
  def py_str(string):
    """Convert C string back to Python string"""
    return string

def assert_python_min_ver(py2_ver, py3_ver, info_str):
  """Enforce minimum Python version for Python 2.x and 3.x"""
  py2_ver_ = py2_ver.split('.')
  py3_ver_ = py3_ver.split('.')
  if len(py2_ver_) != 2 or len(py3_ver_) != 2 or \
     py2_ver_[0] != '2' or py3_ver_[0] != '3':
    raise ValueError('Incorrect version format')
  if PY3:
    if sys.version_info[1] < int(py3_ver_[1]):
      raise RuntimeError('Python {} or newer is required. Feature: {}'\
                         .format(py3_ver, info_str))
  else:
    if sys.version_info[1] < int(py2_ver_[1]):
      raise RuntimeError('Python {} or newer is required. Feature: {}'\
                         .format(py2_ver, info_str))

# expose C buffer as Python buffer
assert_python_min_ver('2.5', '3.3', 'buffer_from_memory')
if PY3:
  def buffer_from_memory(ptr, size):
    """Make Python buffer from raw memory"""
    func = ctypes.pythonapi.PyMemoryView_FromMemory
    func.restype = ctypes.py_object
    PyBUF_READ = 0x100         # pylint: disable=C0103
    return func(ptr, size, PyBUF_READ)
else:
  def buffer_from_memory(ptr, size):
    """Make Python buffer from raw memory"""
    func = ctypes.pythonapi.PyBuffer_FromMemory
    func.restype = ctypes.py_object
    return func(ptr, size)
