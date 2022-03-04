
import itertools

class ListUtils:
    @classmethod
    def in_groups_of(cls, items, n, fill_value=None):
        """
        generate a list contains items in items in groups of n
        """
        return list(itertools.zip_longest(*(iter(items),) * n, fillvalue=fill_value))


if __name__ == '__main__':
    print(list(map(str, range(10))))
    print(ListUtils.in_groups_of(list(map(str, range(10))), 3, 0))
    print(ListUtils.in_groups_of(list(range(10)), 4, 0))
    print(ListUtils.in_groups_of(list(range(10)), 5, 0))