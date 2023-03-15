def combo(arr,data,start,end,index,r):
     if(index==r):
        print(data)
        return 


     i=start
     while( i<=end and  end - i + 1 >=r-index):
         data[index]=arr[i]
         combo(arr,data,i+1,end,index+1,r)
         i+=1   

def combo2(arr, data,start,end, index, r):
     
     if(index==r):
        print(data)
        return 
    
     if(start>=end):
        return
     
     data[index]=arr[start]
     combo2(arr,data,start+1,end, index+1,r) 
     combo2(arr,data,start+1,end, index,r) 


#arr=[1,5,6,8,9,23,74,89,100,99.87,23,88,21,9,-100,-43,-12,30,2,122,43,3,32,9,3,43,20,99,16,21]

#Main  Function
#arr=['A','B','C']
#data=[0,0]
#combo(arr,data,0,len(arr)-1,0,2)
#combo2(arr,data,0,len(arr)-1,0,4)
#print(len(arr)-1)



