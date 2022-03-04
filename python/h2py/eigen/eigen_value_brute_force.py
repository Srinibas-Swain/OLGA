
import numpy as np

from ..generic import GraphTypes
from ..generic import UGraph


class EigenValueBruteForce:
    """
    Compute the k-th eigen value of the given graph, like a philistine.
    """
    @classmethod
    def compute(cls, graph: UGraph, k: int = None) -> np.array:
        eigen_values, _ = np.linalg.eig(graph.adj_mat)

        eigen_values: np.array = cls.validate_and_clean(eigen_values)

        if k is None:
            return eigen_values
        else:
            return eigen_values[k:k+1]

    @classmethod
    def validate_and_clean(cls, eigen_values: np.array) -> np.array:
        if eigen_values.dtype != np.complex:
            return eigen_values

        num_imags_non_trivial = np.sum(np.absolute(eigen_values.imag) > 1e-10)
        if num_imags_non_trivial > 0:
            raise Exception("Eigen values are imaginary: \n{}".format(eigen_values))

        return eigen_values.real


if __name__ == '__main__':
    from ..generic.unique_certificates import UniqueCertificates

    for graph in UniqueCertificates():
        print("n: {} id: {} eigen-value: {:.3f}".format(graph.num_vertices, graph.generate_graph_id(),
                                                        EigenValueBruteForce.compute(graph)))
