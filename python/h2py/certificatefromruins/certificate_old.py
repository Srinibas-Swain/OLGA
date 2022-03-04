# run it as python3 certificate_old.py <verbosity-level>
from h2py.certificatefromruins.blocks import Blocks
from h2py.certificatefromruins.graph_u import *
from h2py.certificatefromruins.permutation import *
from h2py.certificatefromruins.schreiersims import *
from h2py.certificatefromruins.block import *
from h2py.certificatefromruins.partition import *
import sys


class CertificateOld:
    def __init__(self):
        self.base = None
        self.U = []
        self.verbosity = 0

    def initialize2(self, graph):
        self.graph = graph
        self.n = graph.num_vertices
        self.mu = None
        self.sch = Schreiersims(self.n, Permutation.identity(self.n))
        self.str_verb_indent = ''
        blk1 = Block(set(range(self.n)))
        p = Partition(self.n)
        p.append(blk1)
        return p

    @classmethod
    def generate2(cls, graph, verbosity=0):
        cert = CertificateOld()
        cert.verbosity = verbosity
        p = cert.initialize2(graph)
        cert.canon2(p)
        (verbosity >= 1) and print('best', cert.mu)
        return cert.compute_cert()

    def canon2(self, p):
        (self.verbosity >= 2) and print(self.str_verb_indent + 'canon2', p)
        self.str_verb_indent += '   '

        q = self.refine(p)
        l = self.index_of_non_empty(q)

        (self.verbosity >= 3) and print(self.str_verb_indent + 'p ', p, ' refined to', q)
        (self.verbosity >= 3) and print(self.str_verb_indent + 'l', l)

        result = 'better'
        pi1 = None
        if self.mu is not None:
            pi1 = q.to_permutation(l)
            result = self.compare(pi1, l)

        (self.verbosity >= 3) and print(self.str_verb_indent + 'is q discrete?', q.is_discrete())

        if q.is_discrete():
            self.handle_discrete_q2(q, pi1, result)
        else:
            self.handle_non_discrete_q2(q, l, result)
        self.str_verb_indent = self.str_verb_indent[:-3]

    def handle_discrete_q2(self, q, pi1, result):
        (self.verbosity >= 4) and print(self.str_verb_indent + 'handle_discrete_q2 Q', q)

        if self.mu is None:
            self.mu = q.to_permutation()

            (self.verbosity >= 2) and print(self.str_verb_indent + 'first:', self.mu)
        else:
            if result == 'better':
                (self.verbosity >= 2) and print(self.str_verb_indent + 'better:', q)
                (self.verbosity >= 2) and print(self.str_verb_indent + 'better:', pi1)
                self.mu = pi1

            elif result == 'equal':
                (self.verbosity >= 2) and print(self.str_verb_indent + 'equal:', q)
                (self.verbosity >= 2) and print(self.str_verb_indent + 'equal:', pi1)

                pi2 = Permutation(self.n)
                for ix in range(self.n):
                    pi2[pi1[ix]] = self.mu[ix]

                (self.verbosity >= 2) and print(self.str_verb_indent + 'enter:', pi2)

                Schreiersims.enter2(self.n, pi2, self.sch)
            elif result == 'worse':
                (self.verbosity >= 2) and print(self.str_verb_indent + 'worse:', q)
                (self.verbosity >= 2) and print(self.str_verb_indent + 'worse:', pi1)

    def handle_non_discrete_q2(self, Q, l, result):
        if result == 'worse':
            (self.verbosity >= 2) and print(self.str_verb_indent + 'worse:', Q)
            return

        C, D = Q[l].clone(), Q[l].clone()
        R = self.init_R(Q, l)

        (self.verbosity >= 4) and print(self.str_verb_indent + 'handle_non_discrete_q2 R', R, ' C', C)

        while C.nonempty():
            u = C.pop()
            (self.verbosity >= 5) and print(self.str_verb_indent + 'trie u', u)

            self.update_R(R, l, u, D)
            (self.verbosity >= 5) and print(self.str_verb_indent + 'trie R', R)

            self.canon2(R)

            beta_prime = self.init_beta_prime(R, l)
            sch_new = Schreiersims.change_base(self.n,
                                               self.sch,
                                               beta_prime, self.verbosity - 5)
            for g in sch_new[l]:
                (self.verbosity >= 5) and print(self.str_verb_indent + 'trie g', g)

                C = C - {g[u]}

            (self.verbosity >= 5) and print(self.str_verb_indent + 'trie C', C)

    @classmethod
    def generate1(cls, graph, verbosity=0):
        cert = CertificateOld()
        cert.verbosity = verbosity
        p = cert.initialize1(graph)
        cert.canon1(p)
        (verbosity >= 1) and print('best', cert.mu)
        return cert.compute_cert()

    def initialize1(self, graph):
        self.graph = graph
        self.n = graph.num_vertices
        self.mu = None
        self.str_verb_indent = ''
        blk1 = Block(set(range(self.n)))
        p = Partition(self.n)
        p.append(blk1)
        return p

    def canon1(self, p):
        (self.verbosity >= 2) and print(self.str_verb_indent + 'canon1', p)
        self.str_verb_indent += '   '

        q = self.refine(p)
        l = self.index_of_non_empty(q)

        (self.verbosity >= 3) and print(self.str_verb_indent + 'p ', p, ' refined to', q)
        (self.verbosity >= 3) and print(self.str_verb_indent + 'l', l)

        result = 'better'
        pi1 = None
        if self.mu is not None:
            pi1 = q.to_permutation(l)
            result = self.compare(pi1, l)

        (self.verbosity >= 3) and print(self.str_verb_indent + 'canon1: result', result)
        (self.verbosity >= 3) and print(self.str_verb_indent + 'is q discrete?', q.is_discrete())

        if q.is_discrete():
            self.handle_discrete_q1(q, pi1, result)
        else:
            self.handle_non_discrete_q1(q, l, result)
        self.str_verb_indent = self.str_verb_indent[:-3]

    def handle_discrete_q1(self, q, pi1, result):
        (self.verbosity >= 4) and print(self.str_verb_indent + 'handle_discrete_q1 Q', q)

        if self.mu is None:
            self.mu = q.to_permutation()

            (self.verbosity >= 2) and print(self.str_verb_indent + 'first:', self.mu)
        else:
            if result == 'better':
                (self.verbosity >= 2) and print(self.str_verb_indent + 'better:', q)
                (self.verbosity >= 2) and print(self.str_verb_indent + 'better:', pi1)
                self.mu = pi1
            elif result == 'equal':
                (self.verbosity >= 2) and print(self.str_verb_indent + 'equal:', q)
                (self.verbosity >= 2) and print(self.str_verb_indent + 'equal:', pi1)
            elif result == 'worse':
                (self.verbosity >= 2) and print(self.str_verb_indent + 'worse:', q)
                (self.verbosity >= 2) and print(self.str_verb_indent + 'worse:', pi1)

    def handle_non_discrete_q1(self, Q, l, result):
        if result == 'worse':
            (self.verbosity >= 2) and print(self.str_verb_indent + 'handle_non_discrete_q1: worse:', Q)
        # return

        C, D = Q[l].clone(), Q[l].clone()
        R = self.init_R(Q, l)

        (self.verbosity >= 4) and print(self.str_verb_indent + 'handle_non_discrete_q1 R', R, ' C', C)

        while C.nonempty():
            u = C.pop()
            (self.verbosity >= 5) and print(self.str_verb_indent + 'trie u', u)

            self.update_R(R, l, u, D)
            (self.verbosity >= 5) and print(self.str_verb_indent + 'trie R', R)

            self.str_verb_indent += ' ' * 3
            self.canon1(R)
            self.str_verb_indent = self.str_verb_indent[:-3]
            (self.verbosity >= 5) and print(self.str_verb_indent + 'trie C', C)

    def init_R(self, Q, l):
        R = Partition(self.n)
        [R.append(Q[j]) for j in range(l)]
        [R.append(None) for _ in range(2)]
        [R.append(Q[j]) for j in range(l + 1, Q.size())]
        return R

    def update_R(self, R, l, u, D):
        R[l] = Block.from_set({u})
        R[l + 1] = D - {u}

    def init_beta_prime(self, R, l):
        (self.verbosity >= 4) and print(self.str_verb_indent + 'init beta prime', 'R', R, ' l', l)

        beta_prime = Permutation(self.n)
        set_tmp = set(range(self.n))
        for j in range(l + 1):
            r = R[j].get_list()[0]
            beta_prime[j] = r
            set_tmp.difference_update({r})
        j = l
        for y in set_tmp:
            j += 1
            beta_prime[j] = y
        return beta_prime

    def compare(self, part_pi, l):
        for j in range(1, l):
            for i in range(j):
                x = self.graph.is_edge(self.mu[i], self.mu[j])
                y = self.graph.is_edge(part_pi[i], part_pi[j])
                if x < y:
                    return 'worse'
                elif x > y:
                    return 'better'
        return 'equal'

    def refine(self, A):
        (self.verbosity >= 5) and print(self.str_verb_indent + 'refine ', A)
        B, S = A.clone(), A.clone()
        S.reverse()

        n = self.n
        U = Block.from_list(range(n))
        N = B.size()

        while N != 0:
            N -= 1
            T = S.pop()
            (self.verbosity >= 6) and print(self.str_verb_indent + 'N:', N, " T:", T)
            if T.issubset(U):
                # U = U \ T
                U.difference_update(T)
                j = 0

                while j < B.size() and B.size() < n:
                    if B.size() != 1:
                        B, S, U, j, N = self.split_update(B, S, T, U, j, N)
                    j += 1

                if B.size() == n:
                    break

            (self.verbosity >= 6) and print(self.str_verb_indent + "B:", B, "\nS:", S, "\n")
        return B

    def split_update(self, B, S, T, U, j, N):
        L = Blocks(self.n)
        for u in B[j].iterate():
            h = len(T.intersection(self.graph.neighbors(u)))
            L[h].append(u)

        m = L.num_non_empty()
        if m <= 1:
            return (B, S, U, j, N)

        # print("m:",m, "L:",L)
        size_B_old = B.size()
        B.increase_by(m - 1)
        S.increase_by(m)
        for h in range(size_B_old - 1, j, -1):
            B[m - 1 + h] = B[h]
        k = 0
        for h in range(self.n):
            if L[h].size() == 0:
                continue
            B[j + k] = L[h]
            S[N + m - 1 - k] = L[h]
            U.union_update(L[h])
            k += 1
        j = j + m - 1
        N = N + m
        return (B, S, U, j, N)

    def index_of_non_empty(self, q):
        for ix in range(q.size()):
            if q[ix].size() > 1:
                return ix
        return q.size()

    def compute_cert(self):
        (self.verbosity >= 2) and print(self.str_verb_indent, 'mu:', self.mu)
        pow_2, cert_val = 1, 0
        for j in range(self.n - 1, 0, -1):
            for i in range(j - 1, -1, -1):
                if self.graph.is_edge(self.mu[i], self.mu[j]):
                    (self.verbosity >= 2) and print(self.mu[i], '->', self.mu[j], ' => ', i, '->', j)
                    cert_val += pow_2
                pow_2 *= 2
        return cert_val


if __name__ == '__main__':
    # A = Partition.from_string("{0}{1,2,3,4,5,6,7}")
    # print(A)

    sg = "1,3,7 : 0,2,4 : 1,3,6 : 0,2,4 : 1,3,5 : 4,6,7 : 2,5,7 : 0,5,6"
    graph = UGraph.from_string(8, sg)

    cert = CertificateOld()
    # cert.initialize(graph)
    # B = cert.refine(A)
    # print("B ", B)

    cert_val1 = CertificateOld.generate1(graph, verbosity=int(sys.argv[1]))
    cert_val2 = CertificateOld.generate2(graph, verbosity=int(sys.argv[1]))
    graph.generate_graph_id()

    print('#edges:', graph.num_edges(), ' graph-id:', graph.graph_id,
          ' certificate1:', cert_val2, ' certificate1:', cert_val2)

# graph = UGraph.k_n(3)
# cert = Certificate().generate(graph)
# print('cert of graph', graph.graph_id, ' is', cert)
