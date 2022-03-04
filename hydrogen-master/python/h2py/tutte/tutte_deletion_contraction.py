from ..generic.ugraph import UGraph
from ..generic.ugraph_multi_edge import UGraphMultiEdge
from ..generic.graph_types import GraphTypes
from .tutte_node import TutteNode
from .tutte_polynomial import TuttePolynomial


class TutteDeletionContraction:
    """
    Compute the tutte polynomial of a graph using deletion-contraction recurrence.
    T_{G}(x,y) = T_{G \setminus e} (x,y) + T_{G/e}(x,y), e not a loop nor a bridge

    Basically, consider a binary tree with G as the root.
    The left node and right node correspond to a randomly chosen edge: left node represents the graph with that edge
    removed and the right node represents the graph with the edge contracted.
    Any node (graph) which consists only of bridges and self loops aren't expanded further and for the leaves.

    If the leaf node consists of b bridges and s self loops, then we increment the coefficient of x^b y^s by 1.

    The tutte polynomial can be represented as an matrix. The (i, j)-th element of the array corresponds to the
    coefficient of x^i y^j. The array is of size (|E|, |E|)

    References:
        1. Welsh, D. J. A.; Merino, C. (2000), "The Potts model and the Tutte polynomial", Journal of Mathematical
        Physics, 41 (3), doi:10.1063/1.533181.
        2. https://www.wikiwand.com/en/Tutte_polynomial#Deletion%E2%80%93contraction
        3. https://en.wikipedia.org/wiki/File:Deletion-contraction.svg
    """
    def __init__(self, graph: UGraph):
        self.graph: UGraphMultiEdge = UGraphMultiEdge.from_ugraph(graph)
        self.polynomial: TuttePolynomial = TuttePolynomial(graph)

    def compute(self):
        self.process_node(TutteNode(self.graph))

    def process_node(self, node: TutteNode) -> None:
        edge: GraphTypes.Edge = node.find_valid_edge()
        if edge is not None:
            node.left = node.remove_edge(edge)
            node.right = node.contract_edge(edge)
            self.process_node(node.left)
            self.process_node(node.right)
        else:
            self.update_polynomial(node)

    def update_polynomial(self, node: TutteNode) -> None:
        x_power, y_power = node.get_powers()
        self.polynomial.increment(x_power, y_power)


if __name__ == '__main__':
    from generic.unique_certificates import UniqueCertificates
    for ix, graph in enumerate(UniqueCertificates()):
        tdc = TutteDeletionContraction(graph)
        tdc.compute()
        print("ix: {} n: {} id: {} poly: {}".format(ix, graph.num_vertices, graph.generate_graph_id(), tdc.polynomial))

    # graph = list(UniqueCertificates())[28]
    # tdc = TutteDeletionContraction(graph)
    # tdc.compute()
    # print(tdc.polynomial)

    graph1 = UGraph(4)
    graph1.set_edge(0, 1)
    graph1.set_edge(1, 2)
    graph1.set_edge(2, 3)
    graph1.set_edge(3, 0)
    graph1.set_edge(1, 3)
    tdc = TutteDeletionContraction(graph1)
    tdc.compute()
    print('actual  :', tdc.polynomial)
    print('expected: y + y^2 + x + 2xy + 2x^2 + x^3')

