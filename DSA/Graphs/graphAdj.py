adjList=[[0],
         [3,2,4],
         [1,3],
         [4,1,2],
         [1,5,3],
         [4,3,6,7],
         [5],
         [5] ]


#print(adjList)

def BFS(graph,start):
    visted=[False]*len(graph)
    queue=[]

    queue.append(start)
    visted[start]=True
    print(start,end = " ")


    while(len(queue)!=0):
        u=queue.pop(0)

        for v in range(0,len(graph[u])):
            if(visted[graph[u][v]]==False):
                queue.append(graph[u][v])
                visted[graph[u][v]]=True
                print(graph[u][v],end=" ")   
    
    print(" ")




def DFS(graph,index):
     visted=[False]*len(graph)
     DFSR(graph,index,visted)
     print("")


def DFSR(graph,index,visted):
    if(visted[index]==False):
        visted[index]=True
        print(index,end=" ")
        for v in range(0,len(graph[index])):
            DFSR(graph,graph[index][v],visted)




BFS(adjList,1)
DFS(adjList,1)