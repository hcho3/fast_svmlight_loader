Fast SVMLight Loader
====================

A light-weight package to support fast loading of large SVMLight / LibSVM files.

Installation
------------
Download it from PyPI:
```bash
pip3 install --user fast_svmlight_loader
```

Usage
-----
```python
from fast_svmlight_loader import load_svmlight
data = load_svmlight('big_libsvm_file.libsvm', verbose=True)
# set verbose=False to remove "progress bar"
X, y = data['data'], data['labels']
# X is the data matrix (scipy.sparse.csr_matrix)
# y is the label vector (numpy.ndarray)
```