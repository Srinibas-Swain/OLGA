
import hashlib
import logging
import os
import random
import subprocess as sp

from certificate.cert1 import Cert1
from generic.known_graphs import KnownGraphs
from converter.g6 import G6
from generic.graph_types import GraphTypes

class NautyVerify:
    @classmethod
    def do(cls, graph: GraphTypes.UGraph) -> bool:
        cert_actual: int = Cert1.do(graph)
        cert_expected: int = Cert1.do(cls.get_nauty_cert(graph))
        if cert_expected != cert_actual:
            print('certs: actual: {}, expected: {}'.format(cert_actual, cert_expected))
            raise Exception("nauty verification failed")
        return cert_actual == cert_expected

    @classmethod
    def get_nauty_cert(cls, graph) -> GraphTypes.UGraph:
        md5 = cls.get_md5()
        try:
            infile = '/tmp/nauty-verify-{}-infile'.format(md5)
            outfile = '/tmp/nauty-verify-{}-outfile'.format(md5)
            cls.write_g6(graph, infile)
            cls.call_nauty(infile, outfile)
            graph_cert: GraphTypes.UGraph = cls.read_g6(outfile)
            return graph_cert
        finally:
            os.remove(infile)
            os.remove(outfile)

    @classmethod
    def read_g6(cls, outfile) -> GraphTypes.UGraph:
        with open(outfile, 'rb') as f:
            bytes = f.read()
            graph_cert: GraphTypes.UGraph = G6.to_ugraph(bytes)
        return graph_cert

    @classmethod
    def call_nauty(cls, infile, outfile):
        args = ['nauty-labelg', '-g', infile, outfile]
        sp.call(args, stdout=open(os.devnull, 'w'), stderr=open(os.devnull, 'w'))

    @classmethod
    def write_g6(cls, graph: GraphTypes.UGraph, infile):
        with open(infile, 'wb') as f:
            # print(G6.from_ugraph(graph))
            f.write(G6.from_ugraph(graph))
            f.write("\n".encode())

    @classmethod
    def get_md5(cls):
        return hashlib.md5(str(random.randint(1, 1000)).encode()).hexdigest()


if __name__ == '__main__':

    logging.basicConfig(level=logging.WARN)
    NautyVerify.do(KnownGraphs.Kreher.example_7_7())
    NautyVerify.do(KnownGraphs.Kreher.example_7_8())

    # graph: GraphTypes.UGraph = KnownGraphs.Kreher.example_7_7()
    # cert = Cert1.do(graph)
    # print("certificate: {}".format(cert))
    # print("expected   : {}".format(5192304))
    # byte_stream = G6.from_ugraph(graph)
    #
    # # graph: GraphTypes.UGraph = KnownGraphs.Kreher.example_7_8()
    # # cert = Cert1.do(graph)
    # # print("certificate: {}".format(cert))
    # # print("expected   : {}".format(-1))
    #
    # md5_hash = hashlib.md5(str(random.randint(1, 1000)).encode()).hexdigest()
    # print(md5_hash)
