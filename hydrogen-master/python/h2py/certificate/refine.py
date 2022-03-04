
import functools as ft
import line_profiler as lp

from generic.graph_types import GraphTypes

Blocks = GraphTypes.Blocks
UGraph = GraphTypes.UGraph
VertexSet = GraphTypes.VertexSet
Neighbors = GraphTypes.List[GraphTypes.Vertices]
NeighborsSet = GraphTypes.List[GraphTypes.VertexSet]


class Refine:
    """
    Given a list of blocks, the goal is to split the blocks as much as possible, with the ultimate goal being a discrete
    partition.
    TODO: not very clear as to why compute_L might lead to a good parititioning.
    TODO: not very clear on the role of U.
    """
    @classmethod
    def do(cls, graph: UGraph, A: Blocks) -> Blocks:
        """
        Based on algorithm 7.5 Refine#main of Kreher
        Clone A into B and S.
        B will hold the refinement, S will hold the list of paritioning vertex-sets.
        U holds the whitelist of vertices that needs partitioning.
        The algorithm proceeds by considering each vertex-set of S and tries to refine B.
        After each step, the whitelist of vertices is updated with the list of vertices which got refined.
        If a particular vertex-set leads to no-refinement, we try to ignore it.
        When none of the vertices in the whitelist lead to any refinement, we stop.
        :param A: the coarse parition which needs refining.
        :param graph: corresponding instance of UGraph
        :return: a refined partition.
        """
        B: Blocks = cls.clone_blocks(A)
        S: Blocks = cls.clone_blocks(A)
        S.reverse()  # just to ensure we follow example 7.7
        U: VertexSet = set(range(graph.num_vertices))
        nbrs: NeighborsSet = cls.cache_neighbors(graph)
        while len(S) != 0:
            T: VertexSet = S.pop()
            if not T.issubset(U):
                continue
            U.difference_update(T)
            # print('B: {}, U: {}, S: {}, T: {}'.format(B, U, S, T))
            B, S, U = cls.purify(graph, B, S, U, T, nbrs)
            # print('')
        return B

    @classmethod
    def cache_neighbors(cls, graph: UGraph) -> NeighborsSet:
        return [set(list(graph.neighbors(node))) for node in range(graph.num_vertices)]

    @classmethod
    def purify(cls, graph: UGraph, B: Blocks, S: Blocks, U: VertexSet, T: VertexSet, nbrs: NeighborsSet) -> (Blocks, Blocks, VertexSet):
        """
        Try to discretize each block of B.
        For each block in B:
            if there is no refinement:
                noop
            else:
                replace the block in B with the refinement
                add the refinement to S, to be considered immediates (like DFS)
                update U

        :param graph: an instance of UGraph
        :param B: that which is to be purified
        :param S: List of partitioning vertex-sets
        :param U: god knows why this is there.
        :param T: partitioning vertex set
        :return: a refined partition
        """
        B_new: Blocks = []
        for block in B:
            # print('refining {}'.format(block))
            if len(B) == 1:  # todo: why is there a check if |B| != 1 in the algo?
                # print('what {}'.format(B))
                pass
            blocks: Blocks = cls.purify_block(graph, T, block, nbrs)
            # print('refined to {}'.format(blocks))
            if len(blocks) <= 1:  # no refinement of the block
                B_new.append(block)
                continue

            B_new.extend(blocks)
            S.extend(reversed(blocks))
            U = U.union(ft.reduce(lambda x,y: x.union(y), blocks, set()))
            if len(B) == graph.num_vertices:
                break
        return B_new, S, U

    @classmethod
    def purify_block(cls, graph: UGraph, T: VertexSet, block: VertexSet, nbrs: NeighborsSet) -> Blocks:
        """
        Compute L as mentioned in Algorithm 7.5 Refine#SplitAndUpdate.
        We are given a vertex-set T, which is used to split the given vertex-set, block, as follows:
        For each vertex v in block:
            find the number of neighbors of the vertex v which are present in T.
            let this number be k
            augment L[k] with the vertex v.

        This procedure can be visualized as follows:
        We have the induced subgraph with vertices from T and block, with T on one side and block on the other.
        We group the vertices in block by the number of edges from a vertex in block to T.
        :param graph: an instance of UGraph
        :param T: the set of vertices used for splitting
        :param block: list of vertices to be split
        :return: a split of the vertices present in block if a split is possible. else, return the block
        """
        L: Blocks = [set() for _ in range(graph.num_vertices)]
        for vertex in block:
            # h = len(T.intersection(set(graph.neighbors(vertex))))
            h = len(T.intersection(nbrs[vertex]))
            L[h] = L[h].union({vertex})
        # print('L: {}'.format(cls.trim_L(L)))
        return cls.trim_L(L)

    @classmethod
    def trim_L(cls, L: Blocks) -> Blocks:
        """
        Remove the empty sets from the list of vertex-sets
        :param L: list of vertex-sets
        :return: trimmed list of vertex-sets
        """
        trimmed: Blocks = [block for block in L if len(block) > 0]
        return trimmed

    @classmethod
    def clone_blocks(cls, blocks: Blocks) -> Blocks:
        cloned: Blocks = [block.copy() for block in blocks]
        return cloned


if __name__ == '__main__':
    from h2py.generic.known_graphs import KnownGraphs
    graph: GraphTypes.UGraph = KnownGraphs.Kreher.figure_7_6()
    A: GraphTypes.Blocks = [{0}, {1, 2, 3, 4, 5, 6, 7}]
    refined = Refine.do(graph, A)
    print('\n' * 3 + 'A: {}\nrefined : {}'.format(A, refined))
    print('expected: {}'.format([{0}, {2, 4}, {5, 6}, {7}, {1, 3}]))

    graph: GraphTypes.UGraph = KnownGraphs.Kreher.example_7_8()
    A: GraphTypes.Blocks = [{0, 1, 2, 3, 4, 5, 6}]
    refined = Refine.do(graph, A)
    print('\n' * 3 + 'A: {}\nrefined : {}'.format(A, refined))
    print('expected: {}'.format([{0}, {1, 2}, {3, 4}, {6}, {5}]))
