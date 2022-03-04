
from ..generic.graph_types import GraphTypes

Vector = GraphTypes.Vector


class SequenceGenerator:
    """
    Generator for all combinations of a given order
    """
    def __init__(self, order: int, n: int):
        """
        Init
        :param order: base aka number of possibilities per entry.
        :param n: length of the sequence
        """
        self.order: int = order
        self.n: int = n
        self.entries: Vector = [0 for _ in range(self.n)]

    def __iter__(self):
        yield self.entries
        while self.increment() is False:
            yield self.entries

    def __next__(self):
        print("__next__")
        while self.increment() is False:
            yield self.entries

    def increment(self) -> bool:
        ix: int = self.n - 1
        while ix >= 0:
            entry = (self.entries[ix] + 1) % self.order
            self.entries[ix] = entry
            if entry != 0:
                return False
            ix -= 1
        return True

