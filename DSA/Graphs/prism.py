
I=float('inf')

matrix=[
            [I, I, I, I, I, I, I, I],
            [I, I, 25, I, I, I, 5, I],
            [I, 25, I, 12, I, I, I, 10],
            [I, I, 12, I, 8, I, I, I],
            [I, I, I, 8, I, 16, I, 14],
            [I, I, I, I, 16, I, 20, 18],
            [I, 5, I, I, I, 20, I, I],
            [I, I, 10, I, 14, 18, I, I],
        ]


graph=[   #0  1  2  3  4  5 
            [I, I, I, I, I, I], #0
            [I, I, 7, 6, 10, I], #1
            [I, 7, I, I, 2, 3], #2
            [I, 6, I, I, 8, I],  #3
            [I,10, 2, 8, I, 5], #4
            [I, I, 3, I, 5, I], #5
        ]

def primsMST(graph):
     u=0
     v=0
     minNum=I
     V=len(graph)
     n=len(graph)-1
     
     near=[float('inf')]*V

     #Create T[2][V-2]
     T1=[float('inf')]*(V-2)
     T2=[float('inf')]*(V-2)

     #Initialize: Find Min Cost of edge
     for i in range(1,V):
        for j in range(i,V):
             if graph[i][j] <minNum:
                 minNum = graph[i][j]
                 u=i
                 v=j
     
     
     T1[0]=u
     T2[0]=v
    
     print(T2)
     near[u]=near[v]=0

     for i in range(1,V):
         if near[i] !=0:
            if graph[i][u] < graph[i][v]:
                near[i]=u
            else:
                near[i]=v   

     #Repeat
     for i in range(1,n-1):
        k=1
        minNum=I
        for j in range(1,V):
             if near[j]!=0 and graph[j][near[j]] < minNum:
                 k=j
                 minNum=graph[j][near[j]]
        
        T1[i]=k
        T2[i]=near[k]
        print(near)
        print(k)
        
        near[k]=0
       
        for j in range(1,V):
             if near[j] !=0 and graph[j][k] < graph[j][near[j]]:
                 near[j]=k
       
     T3=[T1,T2]
     printMST(graph,T3)
   
  
def printMST(graph,T):
    print("Minimum spanning tree pring")
    sum=0
    V=len(graph)
    for i in range(0,V-2):
        c=graph[T[0][i]][T[1][i]]
        print("[", T[0][i],"]---[",T[1][i],"] cost", c) 
        sum=sum+c
    print("Total cost of MST ",sum)     




primsMST(matrix1)
