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
     bfsList=[]
     queue = []
     nodeNum=len(graph[0])
     visted=[False]*nodeNum
  
     bfsList.append(start)

     queue.append(start)
     visted[start]=True

   
     while(len(queue)!=0):
          u=queue.pop(0)
          for v in range(0,nodeNum):
             if graph[u][v] == 1 and visted[v]==False:
                  bfsList.append(v)
                  visted[v]=True
                  queue.append(v)    

     print(bfsList)                         

 
def DFS(graph,start):
     nodeCount=len(graph[0])
     visted=[False]*nodeCount 
     result=[]
     DFSR(graph,start,visted,result,nodeCount)
     print(result)

def DFSR(graph,start,visted,result,nodeCount):
    if visted[start]==False:
       result.append(start)
       visted[start]=True
       for v in range(0,nodeCount):
         if graph[start][v]==1:
            DFSR(graph,v,visted,result,nodeCount)




BFS(matrix,7)
DFS(matrix,7)


