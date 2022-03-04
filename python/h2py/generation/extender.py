
import itertools
from pathos.pools import ParallelPool as Pool

from h2py.generic.ugraph import UGraph

class Extender:
    def __init__(self):
        pass

    def do(self, graph, num_edges):
        pool = Pool()
        combinations = list(itertools.combinations(range(graph.num_vertices), num_edges))
        tmp = pool.map(self.form_new_graphe, [graph]*len(combinations), combinations)
        return tmp

    def form_new_graphe(self, graph, chosen_vertices):
        graph_new = self.create_graph(graph)
        self.add_edges_to_chosen_vertices(graph_new, chosen_vertices, graph_new.num_vertices - 1)
        return graph_new.adj_mat
        # return chosen_vertices

    def create_graph(self, graph):
        graph_new = UGraph(graph.num_vertices + 1)
        for row, col in graph.edges():
            graph_new.set_edge(row, col)
        return graph_new

    def add_edges_to_chosen_vertices(self, graph, chosen_vertices, vertex_queen_bee) -> None:
        for chosen_vertex in chosen_vertices:
            graph.set_edge(vertex_queen_bee, chosen_vertex)


if __name__ == '__main__':
    graph = UGraph(2)
    print(Extender().do(graph, 1))

