
import logging

from generic import GraphTypes
from generic import UGraph
from generic.known_graphs import KnownGraphs
from certificate.canon1 import Canon1

Blocks = GraphTypes.Blocks
Vertices = GraphTypes.Vertices


class Cert1:
    """
    Implementation of Algorithm 7.8 @ p. 263 of Kreher, Stinson.
    """
    @classmethod
    def do(cls, graph: UGraph, logger: logging.Logger=None) -> int:
        """
        Compute the certificate of the given graph by finding the best partition using canon1.
        :param graph: an instance of UGraph
        :param logger: an optional logger.
        :return: the certificate of the graph
        """
        if logger is None:
            logger: logging.Logger = logging.getLogger(__name__)

        p: Blocks = [set(range(graph.num_vertices))]
        best: Vertices = Canon1.do(graph, p, logger=logger)
        logger.info("best: %s", best)
        return cls.compute_certificate(graph, best)
        # return -1

    @classmethod
    def compute_certificate(cls, graph: UGraph, best: Vertices) ->  int:
        """
        Given a graph and the best discrete partition, compute the certificate.
        :param graph: an instance of UGraph
        :param best: best discrete partition
        :return: certificate
        """
        k, cert = 0, 0
        for j in reversed(range(1, graph.num_vertices)):
            for i in reversed(range(j)):
                if graph.is_edge(best[i], best[j]):
                    cert += 2**k
                k += 1
        return cert


if __name__ == '__main__':
    import hashlib
    import random

    logging.basicConfig(level=logging.INFO)

    graph: UGraph = KnownGraphs.Kreher.example_7_7()
    cert = Cert1.do(graph)
    print("certificate: {}".format(cert))
    print("expected   : {}".format(5192304))

    graph: UGraph = KnownGraphs.Kreher.example_7_8()
    cert = Cert1.do(graph)
    print("certificate: {}".format(cert))
    print("expected   : {}".format(-1))

    md5_hash = hashlib.md5(str(random.randint(1, 1000)).encode()).hexdigest()
    print(md5_hash)
