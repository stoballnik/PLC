from ModifiedList import ModifiedList


def fibonacci_generator():
    f_n_2, f_n_1 = 1, 0
    while True:
        f_n_0 = f_n_2 + f_n_1
        f_n_2, f_n_1 = f_n_1, f_n_0
        #print(str(f_n_0))
        yield f_n_0


def lines_generator(filename):
    with open(filename, "r") as f:
        for line in f:
            yield line.rstrip("\n")


def main():
    print(
        ModifiedList(fibonacci_generator())
            .Where(lambda x: x % 3 == 0)
            .Select(lambda x: x ** 2 if x % 2 == 0 else x)
            .Take(5)
            .ToList()
    )

    print(
        ModifiedList(lines_generator("input.txt"))
            .Select(lambda x: x.split(" "))
            .Flatten().GroupBy(lambda x: x)
            .Select(lambda x: (x[0], len(x[1])))
            .OrderBy(lambda x: -x[1]).ToList()
    )


if __name__ == "__main__":
    main()