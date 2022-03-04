
from h2py.generic.ugraphc cimport UGraph

cdef class Certificate:
    def __cinit__(self, UGraph graph):
        self.graph = graph