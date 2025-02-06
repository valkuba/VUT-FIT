#!/usr/bin/env python3

class Polynomial:
    """Class to store polynomial with single attribute coefficients which is a list"""
    def __init__ (self, *args, **kwargs):
        """Initiating attributes of class Polynomial
            returns Polynomial object"""
        if kwargs:                                          # method is called with named arguments
            self.coefficients = []
            # arguments are sorted by x value and the x is removed from key
            sorted_kwargs = sorted(kwargs.items(), key=lambda x: int(x[0][1:]))
            sorted_kwargs = [(int(key[1:]), value) for key, value in sorted_kwargs]
            last = sorted_kwargs[0][0]
            """Appending coefficients to attribute"""
            for i in range(0, last):                        # if there are missing some coefficients (eg. we start with x2)
                self.coefficients.append(0)
            for i in sorted_kwargs:
                while last < i[0] - 1:
                    self.coefficients.append(0)
                    last += 1
                self.coefficients.append(i[1])
                last = i[0]
        elif isinstance(args[0], list):                     # argument is a list
            self.coefficients = args[0]
        else:
            self.coefficients = list(args)                  # multiple arguments

    def __str__(self):
        """Method to print polynomial in a readable format
            returns string representing polynomial"""
        degree = len(self.coefficients) - 1                 # degree of polynomial (biggest exponent)
        result = ""
        first = True
        for i in reversed(self.coefficients):
            if first:                                      # first element doesn't have + sign
                operator = ""
                first = False
                if i < 0:
                    operator = "-"
            elif i < 0:
                operator = " - "
            else:
                operator = " + "
            if i != 0:
                result += operator
                if (i not in (1, -1)) or degree == 0:      # dont need to write the 1 coefficient before x
                    result += str(abs(i))
                if degree > 0:
                    result += "x"
                    if degree != 1:
                        result += "^" + str(degree)
            elif degree == len(self.coefficients) - 1:
                first = True
            degree -= 1
        if result == "":                                # if the polynomial is empty 0 is printed
            result = "0"
        return result

    def __add__(self, other):
        """Method which allows adding polynomial
            returns added polynomial (the self polynomial stays unchanged)"""
        # need to make both polynomial same length
        while len(self.coefficients) > len(other.coefficients):
            other.coefficients.append(0)
        while len(self.coefficients) < len(other.coefficients):
            self.coefficients.append(0)
        return Polynomial([sum(pair) for pair in zip(self.coefficients, other.coefficients)])

    def __eq__(self, other):
        """Method which allows comparing polynomials
            returns True if polynomials are same otherwise False"""
        while len(self.coefficients) > len(other.coefficients):
            other.coefficients.append(0)
        while len(self.coefficients) < len(other.coefficients):
            self.coefficients.append(0)
        for num, num2 in zip(self.coefficients, other.coefficients):
            if num != num2:
                return False
        return True

    def __pow__(self, power):
        """Method which allows power polynomials
            returns polynomial powered on <power>"""
        if power == 0:
            return Polynomial(1)
        if power == 1:
            return Polynomial(self.coefficients)
        multiply = self.coefficients
        while power > 1:
            result = {}
            for i in range (0, len(self.coefficients)):                 # iterates through both polynomial and multiplies them
                for j in range (0, len(multiply)):
                    try:
                        result[i+j] += self.coefficients[i] * multiply[j]   # the result is saved into dictionary
                    except KeyError:                                        # where the key is degree of polynomial
                        result[i+j] = self.coefficients[i] * multiply[j]
            multiply = list(result.values())
            power -= 1
        return Polynomial(multiply)

    def derivative(self):
        """Method to derivate Polynomial
            returns derivated polynomial"""
        degree = 0
        result = []
        for num in self.coefficients:
            if degree == 0:
                degree += 1
                continue
            new = num*degree
            result.append(new)
            degree += 1
        return Polynomial(result)

    def at_value(self, *value):
        """Method calculated result of polynomial at value
            returns result of polynomial"""
        result = 0
        result = 0
        degree = 0
        if len(value) > 1:                                      # if 2 polynomial are sent need to subtract the results of them
            for num in self.coefficients:
                result += num * (pow(value[1], degree))
                degree += 1
            degree = 0
        for num in self.coefficients:
            if len(value) > 1:
                result -= num * (pow(value[0], degree))
            else:
                result += num * (pow(value[0], degree))
            degree += 1
        return result
