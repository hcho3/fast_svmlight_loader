# coding: utf-8
"""
fast_svmlight_loader: fast reader for SVMLight file
"""

from __future__ import absolute_import as _abs
from importlib import import_module
from .compat import assert_python_min_ver
assert_python_min_ver('2.7', '3.1', 'importlib')

# package will re-export public members of the following scripts/subpackages:
CORE_PACKAGES = ['.core']

__all__ = []
for package in CORE_PACKAGES:
  module = import_module(package, __name__)
  for public_member in module.__all__:
    globals()[public_member] = vars(module).get(public_member)
  __all__ += module.__all__
