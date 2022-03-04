
import multiprocessing
import tqdm

from certificate.cert1 import Cert1


class GeneratorViaPool:
    @classmethod
    def do(cls, all_graphs, num_processes=None):
        if num_processes is None:
            num_processes = multiprocessing.cpu_count() - 2

        pool = multiprocessing.Pool(processes=num_processes)
        items = pool.map(cls.compute_certificate, all_graphs)
        # return list()
        return cls.eliminate_duplicates(zip(all_graphs, items))

    @classmethod
    def eliminate_duplicates(cls, items):
        graphs_final = []
        certificates = set()
        for graph, cert in items:
            if cert not in certificates:
                certificates.add(cert)
                graphs_final.append(graph)
        return graphs_final

    @classmethod
    def compute_certificate(cls, graph):
        return Cert1.do(graph)