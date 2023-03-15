def fib(n):
    if n<=1:
        return n
    else:
      return fib(n-1)+fib(n-2)


#print(fib(6))


fib_memo = {}
def fibM(n):
    if n <=1: return n
    
    print(fib_memo)
    if n not in fib_memo:
        fib_memo[n] = fibM(n-1) +fibM(n-2)
    return fib_memo[n]



def fibonacci_non_recursive(n):
    a, b = 0, 1
    for i in range(n):
        a, b = b, a + b
        print(a)


print(fibonacci_non_recursive(6))
#print(fibM(6))
