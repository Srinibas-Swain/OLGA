
from h2py.generic.ugraphc cimport UGraph

cdef class Certificate:
    cdef:
        UGraph graph
