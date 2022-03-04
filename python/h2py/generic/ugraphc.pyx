# from libcpp cimport bool
from libc.stdlib cimport malloc, free
from numpy import bool

cdef class UGraph:
    def __cinit__(self, num_vertices):
        self.num_vertices = num_vertices
        self.adj_mat = <int*>malloc(self.num_vertices * self.num_vertices * sizeof(int))

    def __dealloc__(self):
        if self.adj_mat != NULL:
            print("dealloc")
            free(self.adj_mat)

    cpdef int num_edges(self):
        cdef:
            int edges = 0
            unsigned int ix
        for ix in range(self.num_vertices * self.num_vertices):
            if self.adj_mat[ix] == 1:
                edges += 1
        return edges

    cpdef unsigned long generate_graph_id(self):
        cdef:
            unsigned long graph_id
            unsigned int ix
