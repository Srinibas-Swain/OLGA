
from enum import Enum

from generic.graph_types import GraphTypes

UGraph = GraphTypes.UGraph
Vertices = GraphTypes.Vertices


class Compare:
    class Result(Enum):
        WORSE = -1
        EQUAL = 0
        BETTER = 1

    @classmethod
    def do(cls, graph: UGraph, partition1: Vertices, partition2: Vertices) -> Result:
        """
        compare the two partitions wrt the given graph
        :param graph:
        :param partition1:
        :param partition2:
        :return:
        WORSE if partition1 <  partition2
        EQUAL if partition1 == partition2
        BETTER if partition1 >  partition2
        """
        n: int = min(len(partition1), len(partition2))
        for j in range(1, n):
            for i in range(j):
                x = graph.is_edge(partition1[i], partition1[j])
                y = graph.is_edge(partition2[i], partition2[j])
                if x < y:
                    return cls.Result.WORSE
                elif x > y:
                    return cls.Result.BETTER
        return cls.Result.EQUAL