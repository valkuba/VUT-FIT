# minitask 7

def deprecated(func):
    def inner(*args, **kwargs):
        print(f"Call to deprecated function: {func.__name__}")
        print(f"with args: {args}")
        print(f"with kwargs: {kwargs}")
        print(f"returning: {func(*args, **kwargs)}")
        return func(*args, **kwargs)
    return inner


@deprecated
def some_old_function(x, y):
    return x + y

some_old_function(1,y=2)

# should print:
# Call to deprecated function: some_old_function
# with args: (1,)
# with kwargs: {'y': 2}
# returning: 3 