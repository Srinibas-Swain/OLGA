
import multiprocessing
import queue

from certificate.cert1 import Cert1


class GeneratorViaQueue:
    @classmethod
    def do(cls, all_graphs):
        certificates = multiprocessing.Manager().Queue()
        cls.distribute(all_graphs, certificates)
        return cls.eliminate_duplicates(certificates)

    @classmethod
    def distribute(cls, all_graphs, certificates):
        # https://stackoverflow.com/a/13652390
        graphs_new = multiprocessing.Manager().Queue()
        for ix, graph_new in enumerate(all_graphs):
            graphs_new.put((ix, graph_new))
        print('setting up proc')
        number_of_processes = 6
        processes = []
        # https://stackoverflow.com/a/13652390
        for w in range(number_of_processes):
            p = multiprocessing.Process(target=cls.compute_cert, args=(graphs_new, certificates))
            processes.append(p)
            p.start()
        print('join')
        for p in processes:
            p.join()

    @classmethod
    def eliminate_duplicates(cls, certificates):
        print('elim dups')
        certificate_cache = set()
        graphs_final = []
        while not certificates.empty():
            graph, cert = certificates.get()
            if cert not in certificate_cache:
                certificate_cache.add(cert)
                graphs_final.append(graph)
        return graphs_final

    @classmethod
    def compute_cert(cls, graphs_new, certificates):
        while True:
            try:
                # ix, graph = graphs_new.get_nowait()
                ix, graph = graphs_new.get(False, timeout=2)
            except queue.Empty:
                break
            except:
                break
            else:
                # print('{} / {}'.format(ix, graphs_new.qsize()), end=', ')
                try:
                    # print('in it: {}'.format(graph.generate_graph_id()))
                    certificates.put((graph, Cert1.do(graph)))
                    # print('done :: {}'.format(graph.generate_graph_id()))
                    # print('.', end=', ')
                except:
                    # print('yo')
                    break
