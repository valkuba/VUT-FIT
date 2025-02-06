#!/usr/bin/env python3

import collections

my_counter = collections.Counter()

def log_and_count(counts, key=None):
    """Decorator function which is performed before the called function"""
    def outer(func):
        """Outer function is used to access name of the called function"""
        def inner(*args, **kwargs):
            """Inner is for accessing passed arguments"""
            print(f"called {func.__name__} with {args} and {kwargs}")
            if not key:
                counts[func.__name__] += 1              # adding to collection by function name
            else:
                counts[key] += 1                        # for "basic functions" counter
            """Calling the actual function as an return which is not really needed for tests but its nice to perform the called func"""
            return func(*args, **kwargs)
        return inner
    return outer
    
@log_and_count(key = 'basic functions', counts = my_counter)
def f1(a, b=2):
    return a ** b

@log_and_count(key = 'basic functions', counts = my_counter)
def f2(a, b=3):
    return a ** 2 + b

@log_and_count(counts = my_counter)
def f3(a, b=5):
    return a ** 3 - b


f1(2)
f2(2, b=4)
f1(a=2, b=4)
f2(4)
f2(5)
f3(5)
f3(5,4)

print(my_counter)
