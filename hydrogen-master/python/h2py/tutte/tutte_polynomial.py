
import numpy as np

from ..generic.ugraph import UGraph


class TuttePolynomial:
    def __init__(self, graph: UGraph):
        self.size = max(graph.num_edges(), graph.num_vertices)
        self.values: np.array = np.zeros((self.size, self.size), dtype=np.int)

    def increment(self, x_power, y_power):
        self.values[x_power, y_power] += 1

    def __str__(self):
        str_rep = ''
        for x_power in range(self.size):
            for y_power in range(self.size):
                coefficient = self.values[x_power, y_power]
                if coefficient > 0:
                    str_coefficient = '' if coefficient == 1 else coefficient
                    x_term, y_term = '', ''
                    if x_power > 1:
                        x_term = 'x^{}'.format(x_power)
                    elif x_power == 1:
                        x_term = 'x'
                    if y_power > 1:
                        y_term = 'y^{}'.format(y_power)
                    elif y_power == 1:
                        y_term = 'y'
                    str_rep += '{}{}{} + '.format(str_coefficient, x_term, y_term)
        return str_rep[:-2]
