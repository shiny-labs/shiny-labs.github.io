#!/usr/bin/python

from sympy import isprime

## part 1
def xor_stack_with_palprimes(stack, palprimes):
    result = []

    if len(stack) != len(palprimes):
        raise Exception("Args must have the same length. Length of stack={}, length of palprimes={}.")

    for i in range(len(stack)):
        a = stack[i]
        b = palprimes[i]
        #print(a, "^", b)
        result.append(chr(a ^ b))

    return "".join(result)


def part_1():
    stack = [17488, 16758, 16599, 16285, 16094, 15505, 15417, 14832, 14450, 13893, 13926, 13437, 12833, 12741, 12533, 11504, 11342, 10503, 10550, 10319, 975, 1007, 892, 893, 660, 743, 267, 344, 264, 339, 208, 216, 242, 172, 74, 49, 119, 113, 119, 106]
    stack = stack[::-1]

    palprimes = [2, 3, 5, 7, 11, 101, 131, 151, 181, 191, 313, 353, 373, 383, 727, 757, 787, 797, 919, 929, 10301, 10501, 10601, 11311, 11411, 12421, 12721, 12821, 13331, 13831, 13931, 14341, 14741, 15451, 15551, 16061, 16361, 16561, 16661, 17471]

    return xor_stack_with_palprimes(stack, palprimes)



## part 2
def part_2():
    stack_2 = [98426, 97850, 97604, 97280, 96815, 96443, 96354, 95934, 94865, 94952, 94669, 94440, 93969, 93766]
    stack_2 = stack_2[::-1]

    palprimes_99 = [93739, 94049, 94349, 94649, 94849, 94949, 95959, 96269, 96469, 96769, 97379, 97579, 97879, 98389]

    return xor_stack_with_palprimes(stack_2, palprimes_99)



## part 3
def generate_odd_length_palindrome(start, end, ascending=True):
    """ generate palindrome strings of odd length with the given prefix range
        start = starting prefix bound
        end = ending prefix bound
        ascending = whether to increase or decrease when going from start to end.

        returns an iterator

        examples:
        >>> for i in generate_odd_length_palindrome(15, 20):
                print(i)
            151
            181
            191

        >>> for i in generate_odd_length_palindrome(20, 15, ascending=False):
                print(i)
            191
            181
            151
    """
    full_range = range(start, end)

    if not ascending:
        full_range = reversed(range(end, start + 1))

    for i in full_range:
        s = str(i)
        inv_s = s[::-1]

        result = int(s + inv_s[1:])
        if isprime(result):
            yield result

def find_next_palindromic_prime(n):
    """
    Find the next prime palindrome for the number n

    Example:
    >>> find_next_palindromic_prime(192)
    313
    >>> find_next_palindromic_prime(102)
    131
    """

    # if n < 11, no palindromes, pick a prime in the list
    if (n <= 101):
        for i in [2, 3, 5, 7, 11, 101]:
            if i > n:
                return i

    n_str = str(n)
    n_len = (len(n_str) // 2) + 1

    start = int(n_str[0:n_len])
    end = n * 2

    for i in generate_odd_length_palindrome(start, end):
        if i > n:
            return i

def find_prev_palindromic_prime(n):
    """
    Find the previous prime palindrome for the number n

    Example:
    >>> find_prev_palindromic_prime(919)
    797
    >>> find_prev_palindromic_prime(102)
    101
    """

    # if n < 11, no palindromes, pick a prime in the list
    if (n <= 101):
        for i in [101, 11, 7, 5, 3, 2]:
            if i < n:
                return i

    n_str = str(n)
    n_len = (len(n_str) // 2 ) + 1

    start = int(n_str[0:n_len])
    end = 0

    for i in generate_odd_length_palindrome(start, end, ascending=False):
        if i < n:
            return i

def generate_sequence(start, max_terms, ascending=True):
    seq = []
    current_n = start

    if ascending:
        while len(seq) < max_terms:
            res = find_next_palindromic_prime(current_n)
            seq.append(res)
            current_n = res

    else:
        while len(seq) < max_terms:
            res = find_prev_palindromic_prime(current_n)
            seq.append(res)
            current_n = res

    return seq

def compute_primes_for_stack_3():
    stack_3 = [101141058, 101060206, 101030055, 100998966, 100887990, 100767085, 100707036, 100656111, 100404094, 100160922, 100131019, 100111100, 100059926, 100049982, 100030045, 9989997, 9981858, 9980815, 9978842, 9965794, 9957564, 9938304, 9935427, 9932289, 9931494, 9927388, 9926376, 9923213, 9921394, 9919154, 9918082, 9916239]
    stack_3 = stack_3[::-1]

    palindromic_prime_765 = find_prev_palindromic_prime(stack_3[0])
    palindromic_primes_765 = generate_sequence(palindromic_prime_765, len(stack_3) - 1)
    palindromic_primes_765.insert(0, palindromic_prime_765)

    return palindromic_primes_765

def part_3():
    stack_3 = [101141058, 101060206, 101030055, 100998966, 100887990, 100767085, 100707036, 100656111, 100404094, 100160922, 100131019, 100111100, 100059926, 100049982, 100030045, 9989997, 9981858, 9980815, 9978842, 9965794, 9957564, 9938304, 9935427, 9932289, 9931494, 9927388, 9926376, 9923213, 9921394, 9919154, 9918082, 9916239]
    stack_3 = stack_3[::-1]
    palprimes_765 = compute_primes_for_stack_3()
    # palprimes_765 = [9916199, 9918199, 9919199, 9921299, 9923299, 9926299, 9927299, 9931399, 9932399, 9935399, 9938399, 9957599, 9965699, 9978799, 9980899, 9981899, 9989899, 100030001, 100050001, 100060001, 100111001, 100131001, 100161001, 100404001, 100656001, 100707001, 100767001, 100888001, 100999001, 101030101, 101060101, 101141101]

    return xor_stack_with_palprimes(stack_3, palprimes_765)




if __name__ == "__main__":
    print("Full URL: {}{}{}".format(part_1(), part_2(), part_3()))

