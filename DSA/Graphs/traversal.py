matrix=[
        #0,1,2,3,4,5,6,7
        [0,0,0,0,0,0,0,0],   #0
        [0,0,1,1,1,0,0,0],   #1
        [0,1,0,1,0,0,0,0],   #2
        [0,1,1,0,1,1,0,0],   #3 
        [0,1,0,1,0,1,0,0],   #4
        [0,0,0,1,1,0,1,1],   #5
        [0,0,0,0,0,1,0,0],   #6 
        [0,0,0,0,0,1,0,0]    #7
        ]


def BFS(graph,start):
     queue = []

     #number of node 
     nodeNum=len(graph[0])
     
     #initialzing visted array visted[nodeNum] 
     visted=[0]*nodeNum

     #Print first node
     print(start,end=" ")

     #Add first node to queue
     queue.append(start)
     #Mark it down in the visited array
     visted[start]=1

     #Keep looping until the queue is empty 
     while(len(queue)!=0):
          u=queue.pop(0)
          for v in range(0,nodeNum):
             if graph[u][v] == 1 and visted[v]==0:
                  print(v, end =" ")
                  visted[v]=1
                  queue.append(v)   
     print("")               


nodeCount=len(matrix[0]) 
visted=[0]*nodeCount 

def DFS(graph,u):
    if visted[u]==0:
       print(u,end=" ")
       visted[u]=1
       for v in range(0,nodeCount):
         if graph[u][v]==1:
            DFS(graph,v)




#BFS(matrix,1)
