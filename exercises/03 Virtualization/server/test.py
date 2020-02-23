# Sample code file to test file-running capabilities
def fibonacci(n):
    (i, j) = (0, 1)
    for a in range(1, n):
        (i, j) = (i + j, i)
        yield i

for i in fibonacci(20):
    print(i)