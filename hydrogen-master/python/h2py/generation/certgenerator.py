import argparse
import functools as ft
import itertools
import multiprocessing
import operator
import queue # imported for using queue.Empty exception
import sys

import tqdm

from generic import GraphTypes
from generic import UGraph
from certificate import Cert1
from generation.extender2 import Extender2
from generation.generator_via_pool import GeneratorViaPool
from generation.generator_via_process import GeneratorViaProcess

CertificateCache = GraphTypes.CertificateCache
List = GraphTypes.List
Vertex = GraphTypes.Vertex
Vertices = GraphTypes.Vertices


class CertGenerator:
    """
    Try to generate graphs based on the certificate graphs generated for n-1 vertices and add all possible combinations
    of edges only from the new node.
    """
    @classmethod
    def do(cls, n_stop, num_processes: int = None):
        graphs = cls.generate_till_2()
        return cls.generate(graphs, n_stop, num_processes)

    @classmethod
    def generate(cls, graphs, n_stop: int, num_processes: int = None):
        if graphs[0].num_vertices == n_stop:
            return []

        all_graphs = cls.get_all_graphs(graphs)
        # all_graphs = cls.get_all_graphs_iterator(graphs)
        # graphs_final = GeneratorViaPool.do(all_graphs)

        # graphs_final = GeneratorViaPool.do(all_graphs, num_processes=num_processes)
        # print('n: {} count: {}'.format(graphs_final[0].num_vertices, len(graphs_final)))
        # return graphs + cls.generate(graphs_final, n_stop)

        certs = [Cert1.do(graph) for graph in all_graphs]
        graphs_trimmed, certs_trimmed = cls.eliminate_duplicates(zip(all_graphs, certs))
        print('trimmed from {} to {}'.format(len(all_graphs), len(graphs_trimmed)))
        return [certs_trimmed] + cls.generate(graphs_trimmed, n_stop)

    @classmethod
    def eliminate_duplicates(cls, items):
        graphs_final = []
        certificates = set()
        for graph, cert in items:
            if cert not in certificates:
                certificates.add(cert)
                graphs_final.append(graph)
        return graphs_final, list(certificates)

    @classmethod
    def get_all_graphs_iterator(cls, graphs):
        for graph in graphs:
            for num_edges in range(1, graph.num_vertices + 1):
                for chosen_vertices in itertools.combinations(range(graph.num_vertices), num_edges):
                    yield cls.form_new_graph(graph, chosen_vertices)

    @classmethod
    def get_all_graphs(cls, graphs):
        graph_gens = [list(cls.extend(graph)) for graph in graphs]
        all_graphs = ft.reduce(operator.add, graph_gens)
        print('all graphs: {}'.format(len(all_graphs)))
        return all_graphs

    @classmethod
    def extend(cls, graph):
        for num_edges in range(1, graph.num_vertices+1):
            for chosen_vertices in itertools.combinations(range(graph.num_vertices), num_edges):
                yield cls.form_new_graph(graph, chosen_vertices)

    @classmethod
    def check_and_add(cls, cert_cache, graphs_new, graph_new):
        cert: int = Cert1.do(graph_new)
        key = (graph_new.num_vertices, cert)
        if key in cert_cache:
            return
        cert_cache.add(key)
        graphs_new.append(graph_new)

    @classmethod
    def form_new_graph(cls, graph, chosen_vertices):
        graph_new = cls.create_graph(graph)
        cls.add_edges_to_chosen_vertices(graph_new, chosen_vertices, graph_new.num_vertices-1)
        return graph_new

    @classmethod
    def add_edges_to_chosen_vertices(cls, graph, chosen_vertices, vertex_queen_bee) -> None:
        for chosen_vertex in chosen_vertices:
            graph.set_edge(vertex_queen_bee, chosen_vertex)

    @classmethod
    def create_graph(cls, graph):
        graph_new = UGraph(graph.num_vertices + 1)
        for row, col in graph.edges():
            graph_new.set_edge(row, col)
        return graph_new

    @classmethod
    def generate_till_2(cls):
        """
        Generate all graphs with 2 vertices. Basically a singleton list with edge.
        """
        graph = UGraph(2)
        graph.set_edge(0, 1)
        return [graph]

    @classmethod
    def parse_args(cls):
        arg_parser: argparse.ArgumentParser = argparse.ArgumentParser()
        arg_parser.add_argument("-p", "--num_processes", type=int, default=2)
        arg_parser.add_argument("-n", "--n_stop", type=int, default=6)
        args = arg_parser.parse_args()
        return args


if __name__ == '__main__':
    args = CertGenerator.parse_args()
    graphs = CertGenerator.do(n_stop=args.n_stop, num_processes=args.num_processes)
    print('num graphs generated: {}'.format(len(graphs)))
    # print(graphs)
    # num_graphs_by_num_vertices = dict()
    # for graph in graphs:
    #     if graph.num_vertices not in num_graphs_by_num_vertices:
    #         num_graphs_by_num_vertices[graph.num_vertices] = 0
    #     num_graphs_by_num_vertices[graph.num_vertices] += 1
    # print(num_graphs_by_num_vertices)

