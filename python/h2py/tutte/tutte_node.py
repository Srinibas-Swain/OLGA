
from ..generic.graph_types import GraphTypes
from ..generic.tarjen_bridges import TarjenBridges
from ..generic.ugraph_multi_edge import UGraphMultiEdge


class TutteNode:
    def __init__(self, value: UGraphMultiEdge, left=None, right=None):
        self.value: UGraphMultiEdge = value
        self.left: TutteNode = left
        self.right: TutteNode = right

    def remove_edge(self, edge: GraphTypes.Edge):
        """
        Remove the edge from graph.
        :return: a Node instance with value as the cloned graph with edge removed.
        """
        cloned: UGraphMultiEdge = self.value.clone()
        cloned.remove_edge(edge[0], edge[1])
        node: TutteNode = TutteNode(cloned)
        return node

    def contract_edge(self, edge: GraphTypes.Edge):
        """
        Contracts the edge and returns a new node. The new_graph will have one less vertex.
        Let the end points of edge that is to be contracted be (v1, v2). The other vertices are \hat{V} = V \ {v1, v2}
        These vertices are mapped to 0 .. |\hat{V}|-1
        {v1, v2}, which need to be merged are mapped to |\hat{V}|
        """
        if not self.value.is_edge(edge[0], edge[1]):
            raise Exception("not edge {}".format(edge))
        edge = (min(edge), max(edge))
        n_old = self.value.num_vertices
        new_graph = UGraphMultiEdge(n_old - 1)
        new_by_old = self.get_new_by_old(edge, n_old)

        # graph.edges() need not ensure row < col
        for col in range(n_old):
            for row in range(col):
                if not self.value.is_edge(col, row):
                    continue

                # retain the multi-edge count in the new_graph
                for _ in range(self.value.edge_count(col, row)):
                    new_graph.set_edge(new_by_old[col], new_by_old[row])

                if (row == edge[0]) and (col == edge[1]):  # this is the edge that is to be removed
                    new_graph.remove_edge(new_by_old[col], new_by_old[row])

        return TutteNode(new_graph)

    @classmethod
    def get_new_by_old(cls, edge, n_old):
        new_vertices = list(sorted(set(range(n_old)).difference(set(edge))))
        new_by_old = [-1 for _ in range(n_old)]
        for ix, old in enumerate(new_vertices):
            new_by_old[old] = ix
        new_by_old[edge[0]] = new_by_old[edge[1]] = len(new_vertices)
        return new_by_old

    def get_powers(self) -> (int, int):
        """
        :return: the number of bridges and number of self-loops in the graph pertaining to this node.
        """
        is_edge = self.value.is_edge
        num_bridges, num_self_loops = 0, 0
        for col in range(self.value.num_vertices):
            for row in range(col+1):
                if row == col:
                    num_self_loops += self.value.edge_count(row, col)
                else:
                    if is_edge(row, col):
                        num_bridges += 1
        return num_bridges, num_self_loops

    def find_valid_edge(self) -> GraphTypes.Edge:
        """
        Find a edge in the graph which is not a bridge or a self loop.
        :param graph: UGraph instance
        :return: Edge if a valid edge is found. else None
        """
        bridges: GraphTypes.EdgeSet = set(self.get_edge(node1, node2)
                                          for node1, node2 in TarjenBridges(self.value).compute())
        # hack for tarjen bridges for multi-edge graphs
        edge_count = self.value.edge_count
        bridges = set(bridge for bridge in bridges if (edge_count(*bridge) == 1))

        is_edge = self.value.is_edge
        non_bridge: GraphTypes.Edge = None
        for col in range(self.value.num_vertices):
            for row in range(col):
                if not is_edge(col, row):
                    continue
                edge = self.get_edge(row, col)
                if edge not in bridges:
                    return edge
        return non_bridge

    @classmethod
    def get_edge(cls, node1, node2) -> GraphTypes.Edge:
        """
        Construct an edge with the first value <= second value
        :param node1: first endpoint
        :param node2: second endpoint
        :return: Edge with [0] <= [1]
        """
        edge: GraphTypes.Edge = (min(node1, node2), max(node1, node2))
        return edge


if __name__ == '__main__':
    graph = UGraphMultiEdge(3)
    graph.set_edge(0, 1)
    graph.set_edge(1, 2)
    graph.set_edge(1, 2)
    print(graph)
    node = TutteNode(graph)
    print(node.contract_edge((1, 2)).value)

    # graph = UGraphMultiEdge(3)
    # graph.set_edge(0, 1)
    # graph.set_edge(1, 2)
    # graph.set_edge(2, 0)
    # print(graph)
    # node = TutteNode(graph)
    # print(node.contract_edge((0, 1)).value)
    #
    # graph = UGraphMultiEdge(4)
    # graph.set_edge(0, 1)
    # graph.set_edge(1, 2)
    # graph.set_edge(2, 3)
    # graph.set_edge(3, 0)
    # graph.set_edge(1, 3)
    # print(graph)
    # node = TutteNode(graph)
    # print(node.contract_edge((1, 3)).value)


