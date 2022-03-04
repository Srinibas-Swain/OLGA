
from typing import List, Set, Dict, NewType, Tuple, Generator

from .ugraph import UGraph


class GraphTypes:
    Index = NewType('Index', int)
    Dict = Dict

    Bag = Set[int]
    Vector = List[int]

    Vertex = NewType('Vertex', int)
    Vertices = NewType('Vertices', List[Vertex])
    VertexSet = NewType('VertexSet', Set[Vertex])

    NbrhdSet = Dict[int, Set[int]]
    Nbrhd = List[Vertices]

    Edge = NewType('Edge', Tuple[Vertex, Vertex])
    Edges = NewType('Edges', List[Edge])
    EdgeSet = NewType('EdgeSet', Set[Edge])

    Color = NewType('Color', int)
    Colors = NewType('Colors', List[Color])
    ColorSet = NewType('ColorsSet', Set[Color])

    UGraph = NewType('UGraph', UGraph)

    Bools = List[bool]

    List = List

    Blocks = NewType('Blocks', List[VertexSet])
    Generator = Generator
    RowCol = NewType('RowCol', Tuple[Vertex, Vertex])
    CertificateCache = Set[Tuple[Vertex, int]]