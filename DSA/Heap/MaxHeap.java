package com.heap;

public class MaxHeap {
	
	public int [] heap;
	public int curSize;
	public int maxSize;
	
	//Initializer 
	public MaxHeap(int arraySize) {
		this.heap=new int[arraySize];
		this.curSize=0;
	}
	
	public void insert(int value) {
		heap[curSize]=value;
		int curIndex=curSize;
		
		while( heap[curIndex]>heap[(curIndex-1) / 2 ]){
			swapIndex(curIndex,(curIndex-1) /2 );
			curIndex=(curIndex -1 ) / 2;
		}
		
		curSize++;
	}
	
	
	public void delete() {
		int deleteValue=-9999;
		
		//1. Swap the last element with the root element
		swapIndex(curSize-1,0);
		
		//2. Delete the last node
		 heap[curSize-1]=deleteValue;
		 
		 //3. Shrink Heap
		 curSize--;
		 
		
		 int pos=0;
		 while(pos<curSize) {
			 int leftPos =(pos*2)+1;
			 int rightPos=(pos*2)+2;
			 
			 if(leftPos>curSize || rightPos>curSize || heap[leftPos]==deleteValue  || heap[rightPos]==deleteValue) {
				 return;
			 }
			 
			 if(heap[leftPos]>heap[rightPos]) {
				 swapIndex(pos,leftPos);
				 pos=leftPos;
			 }
			 else {
				  swapIndex(pos,rightPos);
				  pos=rightPos;
			 }
			 
		 }
		
	}
	
	public void printHeap() {
		for(int i=0;i<curSize;i++) {
			System.out.println(this.heap[i]+" ");
		}
	}
	
	
	public void swapIndex(int indx_1,int indx_2) {
		int temp=this.heap[indx_1];
		this.heap[indx_1]=this.heap[indx_2];
		this.heap[indx_2]=temp;
		
	}

	public static void main(String[] args) {
        MaxHeap heap = new MaxHeap(7);
        heap.insert(100);
        heap.insert(25);
        heap.insert(70);
        heap.insert(35);
        heap.insert(375);
		//heap.printHeap();
	
		heap.delete();
		heap.printHeap();

        

	}

}
