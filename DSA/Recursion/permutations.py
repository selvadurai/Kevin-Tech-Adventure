def permute(a, s, r):
    if s == r:
        print(a)
    else:
        for i in range(s, r):
            #swap
            a[s], a[i] = a[s], a[i] 
            permute(a, s+1, r)
            a[s], a[i] = a[i], a[s]  # backtrack


#Main Function
arr=['A','B','C']
permute(arr,0,3)

for i in range(0,3):
    print(i)
