# DSATUR
#
# **Saturation Degree**
#	  Number of different colors to which it is adjacent (colored vertices)
# 1. Arrange vertices by decreasing order of degrees
# 1. Color a vertex of maximal degree with color 1
# 1. Choose a vertex with a maximal saturation degree.
#	* If there is an equality, choose any vertex of maximal degree in the uncolored **subgraph**
# 1. Color the chosen vertex with the least possible (lowest numbered) color
# 1. If all the vertices are colored, stop.
#	* Otherwise, return to 3

import numpy as np
import operator
import seaborn as sns

from ..generic.graph_types import GraphTypes

Colors = GraphTypes.Colors
Vertices = GraphTypes.Vertices

class DSatur:
    def __init__(self, graph, verbose=False):
        self.graph, self.verbose = graph, verbose
        self.degrees, self.max_degree_vertex = self.compute_degrees()
        self.possible_colors = [set(range(graph.num_vertices)) for _ in
                                range(graph.num_vertices)]  # colors possible for a vertex
        self.num_colored = 0  # num of colored vertices so far; ==self.graph.num_vertices
        self.colors = [None for _ in range(self.graph.num_vertices)] # colors assigned to the vertices
        self.vertices_coloring_order: Vertices = []  # this is to find the apprx maximal clique that starts the coloring

    def log(self, print_string, *args):
        print_string = "dsatur: " + print_string
        if self.verbose:
            if len(args) == 0:
                print(print_string)
            else:
                print(print_string.format(*args))

    def compute(self) -> (GraphTypes.Colors, GraphTypes.Vertices):
        self.set_color(self.max_degree_vertex, 0)  # 0 instead of 1 to ease things
        while not self.completely_colored():
            vertex = self.vertex_with_max_dsat()
            self.set_color(vertex, self.min_color(vertex))
        return self.colors, self.find_maximal_clique()

    def find_maximal_clique(self) -> Vertices:
        """Incrementally find the maximal clique based on the coloration order"""
        ix = 0
        self.log('vertices-coloring-order: {}', self.vertices_coloring_order)

        for ix in range(1, self.graph.num_vertices):
            clique_so_far, vx_current = self.vertices_coloring_order[:ix], self.vertices_coloring_order[ix]
            if not self.completely_connected_to_subgraph(clique_so_far, vx_current):
                return clique_so_far
        return self.vertices_coloring_order

    def completely_connected_to_subgraph(self, vxs_subgraph, vx_current):
        """Check if vx_current is connected to all of vxs_subgraph"""
        for vx_subgraph in vxs_subgraph:
            if not self.graph.is_edge(vx_current, vx_subgraph):
                return False
        return True

    def completely_colored(self):
        return self.num_colored == self.graph.num_vertices

    def vertex_with_max_dsat(self):
        """ Find a vertex with max satd (break ties by choosing max degree vertex in uncolored subgraph) """
        vertices = self.max_sat_degree_vertices()
        if len(vertices) == 1:
            return vertices[0]

        # multiple vertices with same max sat deg
        # find the vertex with max degree in the uncolored subgraph
        return self.max_degree_vertex_in_uncolored_graph()

    def max_degree_vertex_in_uncolored_graph(self):
        ix_max, _ = max(enumerate(self.degrees), key=operator.itemgetter(1))
        return ix_max

    def max_sat_degree_vertices(self):
        """ Returns tuples of (vertex, its_saturation_degree) for each vertex with max sat degree"""
        # (vertex, saturation-degree) for uncolored vertices
        # saturation degree = num_vertices - possible colors
        vx_sat_degrees = [(vx, self.graph.num_vertices - len(self.possible_colors[vx])) for vx, color in
                          enumerate(self.colors) if color is None]
        sat_degree_max = max(sat_degree for _, sat_degree in vx_sat_degrees)

        self.log('vx_sat_degrees: {} \n sat-degree-max: {}', vx_sat_degrees, sat_degree_max)
        return [vx for vx, sat_degree in vx_sat_degrees if sat_degree == sat_degree_max]

    def min_color(self, vertex):
        """get the min color that could color vertex"""
        return min(self.possible_colors[vertex])

    def set_color(self, vertex, color):
        self.log("colors[{}] = {}", vertex, color)

        self.colors[vertex] = color
        self.vertices_coloring_order.append(vertex)
        self.num_colored += 1

        # remove vertex from graph, update possible colors of nbrs of vertex
        for nbr in self.graph.neighbors(vertex):
            if self.colors[nbr] is not None:
                continue

            # remove the vertex -- nbr edge
            self.degrees[vertex] -= 1
            self.degrees[nbr] -= 1

            # update possible_colors of the nbr
            if color in self.possible_colors[nbr]:
                self.log("possible_colors[{}] \ {}", nbr, color)
                self.possible_colors[nbr].remove(color)

        self.log('self.degrees: {}', self.degrees)
        assert self.degrees[vertex] == 0, "degrees[vertex] (={}) != 0".format(self.degrees[vertex])

    def compute_degrees(self):
        max_degree_vertex = 0
        degrees = [0 for _ in range(self.graph.num_vertices)]
        for row, col in self.graph.edges():
            degrees[row] += 1
            degrees[col] += 1
            max_degree_vertex = col if degrees[col] > degrees[max_degree_vertex] else (
                row if degrees[row] > degrees[max_degree_vertex] else max_degree_vertex)
        return degrees, max_degree_vertex

    @classmethod
    def color_graph(kls, graph, colors):
        """ prerequisite: self.compute() """
        labels = list(map(str, range(graph.num_vertices)))
        graph_viz_object = graph.graph_viz(labels)
        colors_xkcd = sns.xkcd_palette(np.random.choice(list(sns.xkcd_rgb.keys()), graph.num_vertices))
        for label, ix_color in zip(labels, colors):
            graph_viz_object.node(label, style='filled', fillcolor=kls.sns_color_to_hex(colors_xkcd[ix_color]))
        return graph_viz_object

    @classmethod
    def sns_color_to_hex(kls, rgb_tuple):
        """ input: sns rgb tuple, normalized by 255"""
        rgb_ints = [int(x * 255) for x in rgb_tuple]
        return '#%02x%02x%02x' % tuple(rgb_ints)

if __name__ == '__main__':
    from graph import UGraph

    graphs = [UGraph.graph_from_edges({'num_vertices': 2, 'edges': [(0, 1)]}),
              UGraph.graph_from_edges({'num_vertices': 3, 'edges': [(0, 1), (1, 2)]}),
              UGraph.graph_from_edges({'num_vertices': 3, 'edges': [(0, 1), (1, 2), (2, 0)]}),
              UGraph.graph_from_edges({'num_vertices': 4, 'edges': [(0, 1), (1, 2), (2, 3), (3, 0)]})]

    for graph_obj in graphs:
        print(graph_obj.graph_id, DSatur(graph_obj).colors)

    # run this if using jupyter
    #
    # from IPython.display import SVG
    # from IPython.display import display as i_display
    # from ipywidgets import interact, interactive, fixed
    #
    # def compute_dsat(graph_obj):
    #     dsatur = DSatur(graph_obj)
    #     gv = dsatur.color_graph()
    #     i_display(SVG(gv._repr_svg_()))
    #
    #
    # graphs = [UGraph.graph_from_edges({'num_vertices': 2, 'edges': [(0, 1)]}),
    #           UGraph.graph_from_edges({'num_vertices': 3, 'edges': [(0, 1), (1, 2)]}),
    #           UGraph.graph_from_edges({'num_vertices': 3, 'edges': [(0, 1), (1, 2), (2, 0)]}),
    #           UGraph.graph_from_edges({'num_vertices': 4, 'edges': [(0, 1), (1, 2), (2, 3), (3, 0)]})]
    #
    # interactive(compute_dsat, graph_obj=enumerate(graphs))