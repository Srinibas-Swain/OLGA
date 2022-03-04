import unittest

from chromatic.sequence_generator import SequenceGenerator


class CombinationGeneratorTest(unittest.TestCase):
    def test_number_of_combinations(self):
        order, n = 5, 2
        count = 0
        for _ in SequenceGenerator(order, n):
            count += 1
        self.assertEqual(5**2, count)

    def test_combinations_themselves(self):
        order, n = 5, 2
        expected = [[x, y] for x in range(order) for y in range(order)]
        for ix, combination in enumerate(SequenceGenerator(order, n)):
            self.assertListEqual(expected[ix], combination)


if __name__ == '__main__':
    unittest.main()
