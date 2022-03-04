
import graphviz as gv
import numpy as np
import seaborn as sns

from .graph_types import GraphTypes
from .ugraph import UGraph


class GraphUtils:
    @classmethod
    def to_line_graph(cls, graph: GraphTypes.UGraph) -> GraphTypes.UGraph:
        """
        For each vertex,
            consider the edges incident on it
            make all these edges pairwise adjacent in the line graph
        :param graph: input graph
        :return: a line graph
        """
        num_vertices: int = graph.num_vertices
        num_edges = graph.num_edges()
        line_graph: GraphTypes.UGraph = UGraph(num_edges)

        numbered_edge_matrix = cls.get_numbered_edge_matrix(graph)
        for vertex in range(num_vertices):
            nbrs: GraphTypes.Vector = list(graph.neighbors(vertex))
            for ix_nbr1, nbr1 in enumerate(nbrs):
                ix_edge1 = numbered_edge_matrix[vertex, nbr1]
                for nbr2 in nbrs[ix_nbr1+1:]:  # consider only higher ordered vertices. saves time. sure.
                    ix_edge2 = numbered_edge_matrix[vertex, nbr2]
                    line_graph.set_edge(ix_edge1, ix_edge2)
        return line_graph

    @classmethod
    def get_numbered_edge_matrix(cls, graph: UGraph) -> np.array:
        """
        Return an adjacency matrix which is integer valued with non-zero entries corresponding to the edge numbering
        based on column major order traversal of the upper triangular part of the adjacency matrix.
        :param graph: a graph instance
        :return: numbered edge matrix as a np.array
        """
        edge_matrix = np.zeros((graph.num_vertices, graph.num_vertices), dtype=int)
        ix_edge = 0
        for row, col in graph.edges():
            edge_matrix[col, row] = ix_edge
            edge_matrix[row, col] = ix_edge
            ix_edge += 1
        return edge_matrix

    @classmethod
    def color_vertices(cls, graph, node_colors):
        """
        Color the nodes of the graph with the given node colors.
        :param graph: graph instance whose edges need to be colored
        :param node_colors: colors of the edges
        :return: an instance of colored graphviz object corresponding to the graph instance.
        """
        labels = list(map(str, range(graph.num_vertices)))
        graph_viz_object = graph.graph_viz(labels)
        colors_xkcd = sns.xkcd_palette(np.random.choice(list(sns.xkcd_rgb.keys()), graph.num_vertices))
        for label, ix_color in zip(labels, node_colors):
            graph_viz_object.node(label, style='filled', fillcolor=cls.sns_color_to_hex(colors_xkcd[ix_color]))
        return graph_viz_object

    @classmethod
    def color_edges(cls, graph: GraphTypes.UGraph, edge_colors: GraphTypes.Colors, engine='neato'):
        """
        Color the edges of the graph with the given edge colors and an optional rendering engine.
        :param graph: graph instance whose edges need to be colored
        :param edge_colors: colors of the edges
        :param engine: any valid graphviz renderer. default: neato
        :return: an instance of colored graphviz object corresponding to the graph instance.
        """
        graph_viz_object = gv.Graph(engine=engine)
        colors_xkcd = sns.xkcd_palette(np.random.choice(list(sns.xkcd_rgb.keys()), graph.num_edges()))
        ix_edge = 0
        for row, col in graph.edges():
            ix_color = edge_colors[ix_edge]
            hex_color = cls.sns_color_to_hex(colors_xkcd[ix_color])
            graph_viz_object.edge(str(row), str(col), label='', fontsize=str(10), color=hex_color,
                                  penwidth=str(4))
            ix_edge += 1
        return graph_viz_object

    @classmethod
    def color_dominating_set(cls, graph, dom_set: GraphTypes.Vertices, color = None):
        """
        Color the nodes of the graph with the given node colors.
        :param graph: graph instance whose edges need to be colored
        :param node_colors: colors of the edges
        :return: an instance of colored graphviz object corresponding to the graph instance.
        """
        labels = list(map(str, range(graph.num_vertices)))
        graph_viz_object = graph.graph_viz(labels)

        if color is None:
            color = sns.xkcd_palette(np.random.choice(list(sns.xkcd_rgb.keys()), 1))[0]
        for dom_vertex in dom_set:
            graph_viz_object.node(str(dom_vertex), style='filled', fillcolor=cls.sns_color_to_hex(color))
        return graph_viz_object

    @classmethod
    def color_edge_subset(cls, graph: GraphTypes.UGraph, edge_subset: GraphTypes.EdgeSet, engine='neato'):
        """
        Color the edges of the graph with the given edge colors and an optional rendering engine.
        :param graph: graph instance whose edges need to be colored
        :param edge_subset: subset of edges which needs to be colored
        :param engine: any valid graphviz renderer. default: neato
        :return: an instance of colored graphviz object corresponding to the graph instance.
        """
        graph_viz_object = gv.Graph(engine=engine)
        color_xkcd = sns.xkcd_palette(np.random.choice(list(sns.xkcd_rgb.keys()), 1))[0]
        hex_color = cls.sns_color_to_hex(color_xkcd)
        ix_edge = 0
        for row, col in graph.edges():
            if ((col, row) in edge_subset) or ((row, col) in edge_subset):
                graph_viz_object.edge(str(row), str(col), label='*', fontsize=str(10), penwidth=str(4),
                                      color=hex_color)
            else:
                graph_viz_object.edge(str(row), str(col), fontsize=str(10), penwidth=str(1))
            ix_edge += 1
        return graph_viz_object

    @classmethod
    def label_path(cls, graph: GraphTypes.UGraph, path: GraphTypes.Vertices, engine='neato'):
        graph_viz_object = gv.Graph(engine=engine)
        # color used to label the path
        color_xkcd = sns.xkcd_palette(np.random.choice(list(sns.xkcd_rgb.keys()), 1))[0]
        hex_color = cls.sns_color_to_hex(color_xkcd)

        # edge_set = set(GraphUtils.make_edge_tuple(*edge) for edge in graph.edges())
        # for ix_edge, (node1, node2) in enumerate(zip(path[:-1], path[1:])):
        #     graph_viz_object.edge(str(node1), str(node2), label=str(ix_edge), color=hex_color, penwidth=str(4))
        #     edge_set.difference_update({GraphUtils.make_edge_tuple(node1, node2)})
        #
        # for col, row in edge_set:
        #     graph_viz_object.edge(str(col), str(row), fontsize=str(10), penwidth=str(2))

        edges_in_path = list(zip(path[:-1], path[1:]))
        position_by_edges = dict((GraphUtils.make_edge_tuple(*edge), ix) for ix, edge in enumerate(edges_in_path))
        print(path, position_by_edges)
        for row, col in graph.edges():
            edge = (row, col)
            print(edge)
            if edge in position_by_edges:
                position = position_by_edges[edge]
                graph_viz_object.edge(str(row), str(col), label=str(position), color=hex_color, penwidth=str(4))
            else:
                graph_viz_object.edge(str(col), str(row), fontsize=str(10), penwidth=str(1))
        return graph_viz_object

    @classmethod
    def sns_color_to_hex(cls, rgb_tuple):
        """
        Convert a unit-scaled rgb tuple (triplet) to a hex color representation.
        :param rgb_tuple: RGB tuple. (R, G, B) with unit scale (range: [0, 1])
        :return: a hex color representation corresponding to the RGB tuple as a string
        """
        rgb_ints = [int(x * 255) for x in rgb_tuple]
        return '#%02x%02x%02x' % tuple(rgb_ints)

    @classmethod
    def make_edge_tuple(cls, node1: int, node2: int) -> GraphTypes.Edge:
        """
        :param node1: one endpoint
        :param node2: other endpoint
        :return: a edge tuple where the first endpoint of the (edge) tuple is less than the second
        """
        edge: GraphTypes.Edge = (node1, node2)
        if node1 > node2:
            edge = (node2, node1)
        return edge
