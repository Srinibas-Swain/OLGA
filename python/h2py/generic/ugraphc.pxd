
from libcpp cimport bool

cdef class UGraph:
    cdef:
        readonly unsigned int num_vertices
        int* adj_mat

    cpdef unsigned int num_edges(self)
    cpdef unsigned long graph_id(self)