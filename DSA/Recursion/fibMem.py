
def fibM(n,memo):
   print(memo)
   if n<=1:
     return memo[n]
   if n not in memo:
      memo[n]=fibM(n-1,memo)+fibM(n-2,memo)
      return memo[n]
   else:
     return memo[n]


#Main Funcion
memo={0:0,1:1}
print(fibM(6,memo))