
from .brute_force_chromatic_number import BruteForceChromaticNumber
from ..generic.graph_types import GraphTypes
from ..generic.ugraph import UGraph
from ..generic.graph_utils import GraphUtils


class BruteForceChromaticIndex:
    """
        Compute the edge chromatic number aka chromatic index of a graph like a caveman
        Chromatic index: mininum number of colors to color the edges such that adjacent edges don't have the
        same color. Two edges are adjacent if they have one common vertex as their endpoint.
    """

    @classmethod
    def compute(cls, graph: GraphTypes.UGraph) -> GraphTypes.Colors:
        """
        Compute the edge coloring of a given graph. Convert the graph to a line graph and calls
        :func:`~BruteForceChromaticNumber.compute`
        :param graph: graph which needs to be edge colored
        :return: a coloration, a list of edge colors indexed by edges 0 .. num_edges-1.
                 The edges are numbered as they appear in the column major order of the upper triangular part
                 of the adjancency matrix. This is the same order returned by :func:`~UGraph.edges`
        """
        line_graph: GraphTypes.UGraph = GraphUtils.to_line_graph(graph)
        return BruteForceChromaticNumber.compute(line_graph)


if __name__ == '__main__':
    import os
    import tqdm

    fi_certificates = '../../../data/d_db_unique_certificates.csv'
    if os.path.isfile(fi_certificates):
        with open(fi_certificates, 'r') as f:
            lines = f.readlines()
        for line in tqdm.tqdm(lines):
            if len(line) < 2:
                continue
            num_vertices, graph_id = line.split(",")
            num_vertices = int(num_vertices)
            graph_id = int(graph_id)
            coloration = BruteForceChromaticIndex.compute(UGraph.graph_from_graph_id(num_vertices, graph_id))
            print(num_vertices, graph_id, coloration)
