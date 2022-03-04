
import functools as ft

from generic.graph_types import UGraph, Generator, GraphTypes
from generic.list_utils import ListUtils


class G6:
    """
    Bit vectors:

      A bit vector x of length k can be represented as follows.
          Example:  1000101100011100

      (1) Pad on the right with 0 to make the length a multiple of 6.
          Example:  100010110001110000

      (2) Split into groups of 6 bits each.
          Example:  100010 110001 110000

      (3) Add 63 to each group, considering them as bigendian binary numbers.
          Example:  97 112 111

      These values are then stored one per byte.
      So, the number of bytes is ceiling(k/6).

      Let R(x) denote this representation of x as a string of bytes.

    Small nonnegative integers:

      Let n be an integer in the range 0-68719476735 (2^36-1).

      If 0 <= n <= 62, define N(n) to be the single byte n+63.
      If 63 <= n <= 258047, define N(n) to be the four bytes
          126 R(x), where x is the bigendian 18-bit binary form of n.
      If 258048 <= n <= 68719476735, define N(n) to be the eight bytes
          126 126 R(x), where x is the bigendian 36-bit binary form of n.

      Examples:  N(30) = 93
                 N(12345) = N(000011 000000 111001) = 126 66 63 120
                 N(460175067) = N(000000 011011 011011 011011 011011 011011)
                              = 126 126 63 90 90 90 90 90
    Description of graph6 format.
    ----------------------------
    Data type:
       simple undirected graphs of order 0 to 68719476735.
    Optional Header:
       >>graph6<<     (without end of line!)
    File name extension:
       .g6
    One graph:
       Suppose G has n vertices.  Write the upper triangle of the adjacency
       matrix of G as a bit vector x of length n(n-1)/2, using the ordering
       (0,1),(0,2),(1,2),(0,3),(1,3),(2,3),...,(n-1,n).

       Then the graph is represented as  N(n) R(x).
    Example:
       Suppose n=5 and G has edges 0-2, 0-4, 1-3 and 3-4.

       x = 0 10 010 1001

       Then N(n) = 68 and R(x) = R(010010 100100) = 81 99.
       So, the graph is  68 81 99.
    :param graph:
    :return:
    """
    GROUP_SIZE = 6
    @classmethod
    def from_ugraph(cls, graph: UGraph) -> bytes:
        integers = cls.compute_N(graph) + cls.compute_R(cls.str_edges(graph))
        # print('integers before converting to bytes: {}'.format(integers))
        byte_stream: bytes = bytes(integers)
        return byte_stream

    @classmethod
    def group(cls, items, fill_value=None):
        """
        Group the items in groups of 6. last group is zero padded to size 6.
        """
        return ListUtils.in_groups_of(items, cls.GROUP_SIZE, fill_value=fill_value)

    @classmethod
    def str_edges(cls, graph):
        """
        serialize upper triangular adjacency matrix to a series of chars, '0' and '1',
        """
        edge_vector = [graph.is_edge(row, col) for col in range(1, graph.num_vertices) for row in range(col)]
        return [str(int(edge)) for edge in edge_vector]

    @classmethod
    def compute_R(cls, chars, num_groups=None):
        """
        form binary numbers in groups of 6.
        A bit vector x of length k can be represented as follows.
          Example:  1000101100011100

      (1) Pad on the right with 0 to make the length a multiple of 6.
          Example:  100010110001110000

      (2) Split into groups of 6 bits each.
          Example:  100010 110001 110000

      (3) Add 63 to each group, considering them as bigendian binary numbers.
          Example:  97 112 111
        """
        groups = [int(''.join(edge_group), 2) + 63 for edge_group in cls.group(chars, fill_value='0')]
        if num_groups is None:
            return groups
        return groups + [0] * (num_groups - len(groups))

    @classmethod
    def R_inverse(cls, num_vertices, items):
        print(ft.reduce(lambda x, y: x + y, [bin(item-63)[2:] for item in items]))


    @classmethod
    def compute_N(cls, graph) -> [int]:
        """
        Let n be an integer in the range 0-68719476735 (2^36-1).

          If 0 <= n <= 62, define N(n) to be the single byte n+63.
          If 63 <= n <= 258047, define N(n) to be the four bytes
              126 R(x), where x is the bigendian 18-bit binary form of n.
          If 258048 <= n <= 68719476735, define N(n) to be the eight bytes
              126 126 R(x), where x is the bigendian 36-bit binary form of n.
        """
        n = graph.num_vertices
        n_binary = list(bin(n)[2:])
        if n <= 62:
            return [n+63]
        elif n <= 258047:
            return [126] + cls.compute_R(n_binary, 3)
        elif n <= 68719476735:
            return [126, 126] + cls.compute_R(n_binary, 6)
        else:
            raise Exception("n outta bounds")

    @classmethod
    def N_inverse(cls, byte_stream: bytes):
        def n_inverse_helper(items):
            return 1

        ints = [x for x in byte_stream]
        first, second = ints[0], ints[1]
        if (first == 126) and (second == 126):
            return n_inverse_helper(ints[2:8]), ints[8:]
        elif first == 126:
            return n_inverse_helper(ints[2:4]), ints[4:]
        else:
            return first - 63, ints[1:]

    @classmethod
    def to_ugraph(cls, byte_stream: bytes) -> UGraph:
        num_vertices, rest_of_stream = cls.N_inverse(byte_stream)
        graph: UGraph = UGraph(num_vertices)

        # left zero padded, 6-width binary representations
        binary_string = list(ft.reduce(lambda x, y: x + y, [format(item - 63, '06b') for item in rest_of_stream]))
        num_entries = (num_vertices-1)*num_vertices//2
        entry_iterator: map = map(int, binary_string[:num_entries])
        array_iterator: Generator = cls.upper_triangular_matrix_iterator(num_vertices)

        for (row, col), entry in zip(array_iterator, entry_iterator):
            if entry == 1:
                graph.set_edge(row, col)
            else:
                graph.reset_edge(row, col)
        return graph

    @classmethod
    def upper_triangular_matrix_iterator(cls, num_vertices: int) -> Generator[GraphTypes.RowCol, None, None]:
        for col in range(1, num_vertices):
            for row in range(col):
                yield (row, col)


if __name__ == '__main__':
    from h2py.generic.known_graphs import KnownGraphs

    graph: UGraph = KnownGraphs.g6_sample_graph()
    print("A")
    print(graph)
    print("g6: ", G6.from_ugraph(graph))

    # print('int from bytes of g6 format', int.from_bytes(G6.from_ugraph(graph), byteorder='little'))
    # print([x for x in G6.from_ugraph(graph)])
    graph_decoded: UGraph = G6.to_ugraph(G6.from_ugraph(graph))
    print(graph_decoded)

    print(G6.to_ugraph(b'DDW'))
    # graph: UGraph = KnownGraphs.Kreher.example_7_7()
    # print(graph)
    # print(Converter.to_g6(graph))