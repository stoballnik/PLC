from collections import defaultdict

class ModifiedList:

    def __init__(self, sequence):
        self.sequence = sequence


    def Select(self, function):
        def select_generator(sequence):
            for x in sequence:
                yield function(x)
        return ModifiedList(select_generator(self.sequence))


    def Flatten(self):
        def flatten_generator(sequence):
            for array in sequence:
                yield from array
        return ModifiedList(flatten_generator(self.sequence))


    def Where(self, function):
        def where_generator(sequence):
            for i in sequence:
                if function(i):
                    yield i

        return ModifiedList(where_generator(self.sequence))


    def Take(self, k):
        def take_generator(sequence, k):
            for i in range(k):
                yield next(sequence)
        return ModifiedList(take_generator(self.sequence, k))


    def GroupBy(self, key):
        sequence = defaultdict(lambda: [])
        for i in self.sequence:
            sequence[key(i)].append(i)
        return ModifiedList(sequence.items())


    def OrderBy(self, key):
        return ModifiedList(sorted(self.sequence, key=key))


    def ToList(self):
        return [i for i in self.sequence]