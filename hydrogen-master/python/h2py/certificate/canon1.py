
import logging

from generic.known_graphs import KnownGraphs
from generic.graph_types import GraphTypes
from certificate.compare import Compare
from certificate.refine import Refine

Blocks = GraphTypes.Blocks
Result = Compare.Result
UGraph = GraphTypes.UGraph
Vertex = GraphTypes.Vertex
VertexSet = GraphTypes.VertexSet
Vertices = GraphTypes.Vertices


class Canon1:
    """
    Implementation of algorithm 7.7 of Kreher, Stinson.
    """
    @classmethod
    def do(cls, graph: UGraph, blocks: Blocks, best: Vertices = None, logger: logging.Logger = None, depth: int=0) -> Vertices:
        """
        Start by refining the given partition.
            if the refined partition is discrete,
                if it is better than the best found so far, update best.
                return the best
            else, if the refined partition is worse than the best, noop and return the best found so far.
            if the non discrete partition is not worse (ie, equal or better), proceed to process it.
        :param graph: an instance of UGraph
        :param blocks: an unrefined partition (list of vertex-sets)
        :param best: a discrete partition which has been found to be the best so far.
        :param logger: an instance of logger. optional
        :param depth: for logging purposes. optional
        :return: the best discrete partition found
        """
        q: Blocks = Refine.do(graph, blocks)
        logger.debug('{} q: {}'.format('  '*depth, q))
        result: Result = Result.BETTER
        if best is not None:
            pi: Vertices = cls.get_partition(blocks)
            result = Compare.do(graph, best, pi)
            logger.debug('{} result: {}'.format('  '*depth, result))

        if len(q) == graph.num_vertices:
            logger.debug('{} discrete q: {}'.format('  '*depth, q))
            if (best is None) or (result == Result.BETTER):
                best = cls.get_partition(q)
                logger.debug('{} best: {}'.format('  '*depth, best))
            return best

        if result == Result.WORSE:
            return best

        return cls.handle_non_discrete(graph, q, best, logger, depth)

    @classmethod
    def handle_non_discrete(cls, graph: UGraph, q: Blocks, best: Vertices, logger: logging.Logger, depth: int) -> Vertices:
        """
        If refine doesn't yield a discrete partition, proceed to blindly discretize it, block by block, by backtracking.
        for the first non-discrete block found, split it into two blocks, a singleton and the rest.
        we do this m times, where m is the size of the non-discrete block.
        This updated partition is passed to canon1, resulting in the call tree as shown in figure 7.2.
        Refer to pages 258-262 for more details and the worked out example 7.8.

        :param graph: an instance of UGraph
        :param q: a non-discrete partition
        :param best: best discrete partition found so far
        :param logger: an instance of logger
        :param depth: depth of recursion, for logging purposes.
        :return: the best discrete partition found
        """
        ix_non_discrete = cls.get_ix_non_discrete(q)
        block_non_discrete: VertexSet = q[ix_non_discrete].copy()
        logger.debug('{} non-discrete: {}'.format('  '*depth, block_non_discrete))
        r: Blocks = [block.copy() for block in q[:ix_non_discrete]]
        r.extend([{}, {}])
        r.extend([block.copy() for block in q[ix_non_discrete+1:]])
        for u in block_non_discrete:
            r[ix_non_discrete] = {u}
            r[ix_non_discrete+1] = block_non_discrete.difference({u})
            logger.debug('{} r: {}'.format('  '*depth, r))
            best = cls.do(graph, r, best, logger, depth+1)
        return best

    @classmethod
    def get_partition(cls, blocks: Blocks) -> Vertices:
        """
        Construct a half or fully complete from a list of blocks (vertex-sets). If the blocks correspond to a discrete
        partition (all blocks of size 1), then we return a discrete partition. Else, the part of the blocks upto and not
        including the non-discrete block will be converted to a (incomplete) discrete partition.
        :param blocks: list of vertex-sets (aka blocks)
        :return: a complete or partial discrete partition
        """
        partition: Vertices = []
        for block in blocks:
            if len(block) > 1:
                break
            partition.append(list(block)[0])
        return partition

    @classmethod
    def get_ix_non_discrete(cls, blocks: Blocks) -> int:
        """
        Find the index of the first non-discrete block in the given partition / list of blocks (vertex-sets)
        :param blocks: a partition (ie, a list of blocks, ie. a list of vertex-sets)
        :return: index of the first non-discrete block or len(blocks) if the partition is discrete.
        """
        for ix_block, block in enumerate(blocks):
            if len(block) > 1:
                return ix_block
        return len(blocks)


if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)
    logger: logging.Logger = logging.getLogger(__name__)

    graph: UGraph = KnownGraphs.Kreher.example_7_7()
    blocks: Blocks = [set(range(graph.num_vertices))]
    best = Canon1.do(graph, blocks, logger=logger)
    print('best    : {}'.format(best))
    print('expected: {}'.format([1, 3, 5, 6, 7, 0, 2, 4]))

    graph: UGraph = KnownGraphs.Kreher.example_7_8()
    blocks: Blocks = [set(range(graph.num_vertices))]
    best = Canon1.do(graph, blocks, logger=logger)
    print('best    : {}'.format(best))
