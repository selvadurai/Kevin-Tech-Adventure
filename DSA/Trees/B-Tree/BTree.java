package com.tree;

public class BTree {

public int T;

// Node creation
public class Node {
  public int n;
  public int key[] = new int[2 * T - 1];
  public  Node child[] = new Node[2 * T];
  public boolean leaf = true;

  public int Find(int k) {
    for (int i = 0; i < this.n; i++) {
      if (this.key[i] == k) {
        return i;
      }
    }
    return -1;
  };
}

public BTree(int t) {
  T = t;
  root = new Node();
  root.n = 0;
  root.leaf = true;
}

public Node root;

// Search key
private Node Search(Node x, int key) {
  int i = 0;
  if (x == null)
    return x;
  for (i = 0; i < x.n; i++) {
    if (key < x.key[i]) {
      break;
    }
    if (key == x.key[i]) {
      return x;
    }
  }
  if (x.leaf) {
    return null;
  } else {
    return Search(x.child[i], key);
  }
}

// Splitting the node
private void Split(Node x, int pos, Node y) {
  //System.out.println("Split is happening Node x: "+x.n+" Node y:"+y.n+" pos: "+pos);	
  Node z = new Node();
  
  z.leaf = y.leaf;
  z.n = T - 1;
  
  //Copy the last (t-1) keys of y to z
  for (int j = 0; j < T - 1; j++) {
    z.key[j] = y.key[j + T];
    System.out.println( "z.key[j] "+ j +" "+z.key[j]+ " y.key[j + T] "+y.key[j + T] );
  }
  
  // Copy the last t children of y to z
  if (y.leaf==false) {
    for (int j = 0; j < T; j++) {
      z.child[j] = y.child[j + T];
      System.out.println("z.child[j] "+z.child[j] +" y.child[j+T] "+y.child[j+T]);
    }
  }
  
  // Reduce the number of keys in y
  y.n = T - 1;
  
   //Since this node is going to have a new child,
  // create space of new child
  for (int j = x.n; j >= pos + 1; j--) {
    x.child[j + 1] = x.child[j];
    System.out.println("x.child[j + 1] "+ x.child[j + 1] +"x.child[j] "+x.child[j]);
  }
  
  // Link the new child to this node
  x.child[pos + 1] = z;
  System.out.println("position "+pos);

   //A key of y will move to this node. Find the location of
  // new key and move all greater keys one space ahead
  for (int j = x.n - 1; j >= pos; j--) {
    x.key[j + 1] = x.key[j];
    System.out.println("x.key[j+1] "+x.key[j+1]+" x.key[j] "+ x.key[j]);
  }
  
  // Copy the middle key of y to this node
  x.key[pos] = y.key[T - 1];
  
  
  // Increment count of keys in this node
  x.n = x.n + 1;
}

// Inserting a value
public void Insert(final int key) {
  Node r = root;
  if (r.n == 2 * T - 1) {
	System.out.println("Key Split :" +key);  
    Node s = new Node();
    root = s;
    s.leaf = false;
    s.n = 0;
    s.child[0] = r;
    Split(s, 0, r);
    insertValue(s, key);
  } else {
    insertValue(r, key);
  }
}

// Insert the node
final private void insertValue(Node x, int k) {

  if (x.leaf) {
	System.out.println("Leaf key is inserted "+ k);  
    int i = 0;
    //Does the sorting in the key
    for (i = x.n - 1; i >= 0 && k < x.key[i]; i--) {
      x.key[i + 1] = x.key[i];
    }
    //inserts value into key
    x.key[i + 1] = k;
    //add to +1 to n 
    x.n = x.n + 1;
  } else {
	  
	    System.out.println("Not Leaf "+k); 
        int i = 0;
        for (i = x.n - 1; i >= 0 && k < x.key[i]; i--) {
        	  System.out.println("i "+i);
        }
  	    System.out.println("i "+i);

        i++;
        
       Node tmp = x.child[i];
 	   System.out.println("i "+i);

       if (tmp.n == 2 * T - 1) {
           Split(x, i, tmp);
           if (k > x.key[i]) {
             i++;
           }
        }
       
       
       System.out.println("i before insert "+i);
       insertValue(x.child[i], k);
  }

}

public void Show() {
  Show(root);
}

// Display
private void Show(Node x) {
  assert (x == null);
  System.out.print("x.n="+x.n+" ");  
  for (int i = 0; i < x.n; i++) {
	//  System.out.print("x.n "+x.n+" ");  
    System.out.print(x.key[i] + " ");
  }
  if (!x.leaf) {
    for (int i = 0; i < x.n + 1; i++) {
      System.out.println("Elka");	
      Show(x.child[i]);
    }
  }
}

// Check if present
public boolean Contain(int k) {
  if (this.Search(root, k) != null) {
    return true;
  } else {
    return false;
  }
}

public static void main(String[] args) {
  BTree b = new BTree(2);
  b.Insert(8);
  b.Insert(9);
  b.Insert(10);
  b.Insert(11);
  
 // System.out.println(b.root.key[0]);
  //System.out.println(b.root.child[1].key[0]);
  
  b.Insert(7);
  b.Insert(20);
  b.Insert(17);

  b.Show();

  if (b.Contain(12)) {
    System.out.println("\nfound");
  } else {
    System.out.println("\nnot found");
  }
  ;
}
}
	
	
