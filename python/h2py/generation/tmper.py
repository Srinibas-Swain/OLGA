

import itertools
import numpy as np
from pathos.pools import ParallelPool as Pool

from h2py.generic.ugraph import UGraph

def add(x, y):
    return x+y

pool = Pool()

inX = range(10)
inY = range(10)

print(pool.map(add, inX, inY))
# print(pool.map(lambda x,y: x+y, inX, inY))

class UUGraph:
    def __init__(self, n):
        import numpy as np
        self.num_vertices = n
        self.yoyoma = 'a'
        self.adj_mat = np.zeros((self.num_vertices, self.num_vertices), dtype=bool)

class Tmper:
    @classmethod
    def generate_graphs_with_k_new_edges(cls):
        def form_new_graph_dude(chosen_vertices):
            return chosen_vertices

        pool = Pool()
        tmp = pool.map(form_new_graph_dude, itertools.combinations(range(5), 3))
        print(tmp)
        # return list(tmp)

class Tmper2:
    def __init__(self):
        pool = Pool()
        tmp = pool.map(self.form_new_graph_dude, itertools.combinations(range(5), 3))
        print(tmp)
        # return list(tmp)

    def form_new_graph_dude(self, chosen_vertices):
        # return chosen_vertices
        graog = UGraph(5)
        return graog

    def add_edges_to_chosen_vertices(self, graph, chosen_vertices, vertex_queen_bee) -> None:
        for chosen_vertex in chosen_vertices:
            graph.set_edge(vertex_queen_bee, chosen_vertex)

if __name__ == '__main__':
    Tmper2()