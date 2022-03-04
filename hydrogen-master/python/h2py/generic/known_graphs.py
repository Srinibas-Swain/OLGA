

from .ugraph import UGraph


class KnownGraphs:
    class Kreher:
        @classmethod
        def example_7_6(cls) -> UGraph:
            return UGraph.graph_from_edges(8, [
                (0, 1), (1, 2), (2, 3), (3, 4), (4, 5), (5, 6), (6, 7), (7, 0),
                (0, 3), (1, 4), (2, 6), (5, 7)
            ])

        @classmethod
        def example_7_7(cls) -> UGraph:
            return cls.example_7_6()

        @classmethod
        def example_7_8(cls) -> UGraph:
            return UGraph.graph_from_edges(7, [
                (0, 6),
                (1, 4), (1, 5),
                (2, 3), (2, 5),
                (3, 4), (3, 5), (3, 6),
                (4, 5), (4, 6),
                (5, 6)
            ])

    @classmethod
    def g6_sample_graph(cls) -> UGraph:
        """
        Suppose n=5 and G has edges 0-2, 0-4, 1-3 and 3-4.
        """
        return UGraph.graph_from_edges(5, [
            (0, 2), (0, 4), (1, 3), (3, 4)
        ])
