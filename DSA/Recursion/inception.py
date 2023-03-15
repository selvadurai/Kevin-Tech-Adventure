



def dream (n):
    #Base Case 
    #Law 2. A recursive algorithm must have a base case.
    if n==0:
         print("You Woke Up..Dream is Over!")
         return 0;
    
    print("Your in Dream ",n)
    #Law 1. A recursive algorithm must call itself, recursively.
    dream(n-1)
    #3.A recursive algorithm must change its state and move toward the base case.



#dream(6)









# Counts up to 10 
def count(n):
    #Base Case 
    #2. A recursive algorithm must have a base case.
    if n==10:
       return
    else:   
      print("Count: ", n)
      #1. A recursive algorithm must call itself, recursively.
      count(n+1)
      #3.A recursive algorithm must change its state and move toward the base case.


#count(1)







import sys
sys.setrecursionlimit(10000)

#tail(1)

def tail(n):
    if n==7:
       return
    else:  
      print("Count: ", n)
      tail(n+1)   

      


#tail(1)

def head(n):
    if n==7:
       return
    else:  
      head(n+1)   
      print("Count: ", n)


#head(1)     


def funcA(n):
    if n>0:
      print(round(n))
      funcB(n-1)

def funcB(n):
    if n>1:
       print(round(n))
       funcA(n/2)


#funcA(20)


def fun(n):
   if n>100:
      return n-10
   else:
      return fun(fun(n+11))


#print(fun(95))         


def funcTree(n):
     if n>0:
      print(n)
      funcTree(n-1)
      funcTree(n-1)


funcTree(3)
