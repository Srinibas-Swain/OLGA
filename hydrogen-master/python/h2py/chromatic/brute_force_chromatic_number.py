
import os
import tqdm

from .sequence_generator import SequenceGenerator
from ..generic.graph_types import GraphTypes
from ..generic.ugraph import UGraph

Colors = GraphTypes.Colors
Graph = GraphTypes.UGraph


class BruteForceChromaticNumber:
    """
    Compute the chromatic number of a graph like a caveman
    Chromatic number: mininum number of colors to color the vertices such that adjacent vertices don't have the
    same color
    """

    @classmethod
    def compute(cls, graph: Graph) -> Colors:
        """
        Given a graph, starting from 2, check if it is possible to color the graph.
        If it is possible, then return that coloring
        else increase the number of colors and try again.
        Any graph is vertex-colorable with graph.num_vertices colors.
        :param graph: graph which needs to be vertex colored.
        :return: a coloration, a list of colors indexed by vertices 0 .. num_vertices-1.
        """
        num_vertices = graph.num_vertices
        coloration_best: Colors = None
        for num_colors in range(2, num_vertices+1):
            coloration: Colors = cls.colorize(graph, num_colors)
            if coloration is not None:
                coloration_best: Colors = coloration
                break

        if coloration_best is None:
            raise Exception("Coloration is None for graph")
        return coloration_best

    @classmethod
    def colorize(cls, graph: Graph, num_colors: int) -> Colors:
        """
        Try to color the vertices of the graph with num_colors and return the coloration if coloration is possible.
        Tries 2**num_colors combinations, like, as i said, a caveman. But thats stupid, aint it, since num_colors-1
        isn't possible, we should try all permutations of num_colors???
        :param graph: graph which needs to be colored
        :param num_colors: number of colors with which the graph needs to be colored
        :return: coloration as a list of colors. None if coloration isn't possible
        """
        coloration_best: Colors = None
        for coloration in SequenceGenerator(num_colors, graph.num_vertices):
            # todo: can remove combination if number of unique colors < num_colors
            if cls.is_valid_coloration(graph, coloration):
                coloration_best = coloration
                break

        return coloration_best

    @classmethod
    def is_valid_coloration(cls, graph, coloration) -> bool:
        """
        Check if all the adj vertices are of different color
        :param graph: graph, duh.
        :param coloration: coloration of the vertices
        :return: true if coloration is valid. false otherwise.
        """
        for node, nbr in graph.edges():
            if coloration[node] == coloration[nbr]:
                return False
        return True


if __name__ == '__main__':
    fi_certificates = '../../../data/db_unique_certificates.csv'
    if os.path.isfile(fi_certificates):
        with open(fi_certificates, 'r') as f:
            lines = f.readlines()
        for line in tqdm.tqdm(lines):
            if len(line) < 2:
                continue
            num_vertices, graph_id = line.split(",")
            num_vertices = int(num_vertices)
            graph_id = int(graph_id)
            coloration = BruteForceChromaticNumber.compute(UGraph.graph_from_graph_id(num_vertices, graph_id))
            print(num_vertices, graph_id, coloration)
