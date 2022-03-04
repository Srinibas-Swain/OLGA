
import functools as ft
import math
import multiprocess
import multiprocessing as mp
import operator

from certificate.cert1 import Cert1


class GeneratorViaProcess:
    @classmethod
    def do(cls, all_graphs, num_processes=None):
        if num_processes is None:
            num_processes = mp.cpu_count() - 2

        batches = cls.get_batches(all_graphs, num_processes)
        processes, outs = [], []
        manager = mp.Manager()
        return_dict = manager.dict()

        print('start')
        for ix_proc in range(num_processes):
            out_current = list()
            outs.append(out_current)
            p = mp.Process(target=cls.compute_certificate,
                                        args=(batches[ix_proc], ix_proc, return_dict))
            processes.append(p)
            p.start()
        print('join')
        for p in processes:
            p.join()
        print('joined')

        graphs_certs = cls.get_graph_certs(return_dict, batches)
        return cls.eliminate_duplicates(graphs_certs)

    @classmethod
    def get_graph_certs(cls, return_dict, batches):
        graphs_certs = []
        for ix_proc in return_dict.keys():
            batch = batches[ix_proc]
            certs = return_dict[ix_proc]
            assert len(batch) == len(certs)
            graphs_certs.extend(list(zip(batch, certs)))
        return graphs_certs


    @classmethod
    def get_batches(cls, all_graphs, num_processes):
        batch_size = int(math.ceil(len(all_graphs) / num_processes))
        return [all_graphs[ix_proc*batch_size:(ix_proc+1)*batch_size] for ix_proc in range(num_processes)]

    @classmethod
    def compute_certificate(cls, graphs, ix_proc, return_dict):
        # return_dict[ix_proc] = [(graph, Cert1.do(graph)) for graph in graphs]
        return_dict[ix_proc] = [Cert1.do(graph) for graph in graphs]

    @classmethod
    def eliminate_duplicates(cls, items):
        graphs_final = []
        certificates = set()
        for graph, cert in items:
            if cert not in certificates:
                certificates.add(cert)
                graphs_final.append(graph)
        return graphs_final