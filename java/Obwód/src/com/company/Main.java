package com.company;
import static org.junit.jupiter.api.Assertions.assertTrue;
import java.util.Scanner;

public class Main {

    public static void main(String[] args) {
        test();
        test2();
        deMorgan();
    }
    public static void test(){
        Obwód o=new Obwód(12);
        o.createNode(13, "AND", 4);
        o.createNode(14, "OR", 3);
        o.createNode(15, "PARITY", 2);
        o.createNode(16, "NOT");
        o.createNode(17, "MAJORITY", 2);
        o.createNode(18, "OR", 2);
        o.createNode(19, "AND", 3);
        o.createNode(20, "PARITY", 3);
        o.makeNodeOutput(20);
        o.addEdge(1,13);
        o.addEdge(2,13);
        o.addEdge(3,13);
        o.addEdge(4,13);
        o.addEdge(5,14);
        o.addEdge(6,14);
        o.addEdge(7,14);
        o.addEdge(8,15);
        o.addEdge(9,15);
        o.addEdge(10,16);
        o.addEdge(11,17);
        o.addEdge(12,17);
        o.addEdge(13,18);
        o.addEdge(14,18);
        o.addEdge(15,19);
        o.addEdge(16,19);
        o.addEdge(17,19);
        o.addEdge(18,20);
        o.addEdge(19,20);
        o.addEdge(1,20);
        boolean[] in={true,false,false,true,false,false,false,false,true,false,true,true};
        assertTrue(!o.evaluate(in));
        assertTrue(o.depth()==3);
    }
    public static void test2(){
        Obwód o=new Obwód(12);
        o.createNode(13, "AND", 4);
        o.createNode(14, "OR", 3);
        o.createNode(15, "PARITY", 2);
        o.createNode(16, "NOT");
        o.createNode(17, "MAJORITY", 2);
        o.createNode(18, "OR", 2);
        o.createNode(19, "AND", 3);
        o.createNode(20, "PARITY", 3);
        o.makeNodeOutput(20);
        o.addEdge(1,13);
        o.addEdge(2,13);
        o.addEdge(3,13);
        o.addEdge(4,13);
        o.addEdge(5,14);
        o.addEdge(6,14);
        o.addEdge(7,14);
        o.addEdge(8,15);
        o.addEdge(9,15);
        o.addEdge(10,16);
        o.addEdge(11,17);
        o.addEdge(12,17);
        o.addEdge(13,18);
        o.addEdge(14,18);
        o.addEdge(15,19);
        o.addEdge(16,19);
        o.addEdge(17,19);
        o.addEdge(18,20);
        o.addEdge(19,20);
        o.addEdge(1,20);
        boolean[] in={true,false,false,true,true,false,false,false,true,false,true,true};
        assertTrue(o.evaluate(in));
        assertTrue(o.size()==20);
    }
    public static void deMorgan(){
        Obwód o=new Obwód(2);
        o.createNode(3, "AND", 2);
        o.createNode(4, "NOT", 1);
        o.addEdge(1, 3);
        o.addEdge(2, 3);
        o.addEdge(3, 4);
        o.makeNodeOutput(4);
        Obwód o2=new Obwód(2);
        o2.createNode(3, "NOT", 1);
        o2.createNode(4, "NOT", 1);
        o2.createNode(5, "OR", 2);
        o2.addEdge(1,3);
        o2.addEdge(2,4);
        o2.addEdge(3,5);
        o2.addEdge(4,5);
        o2.makeNodeOutput(5);
        boolean[] in=new boolean[2];
        for(int i=0;i<=1;i++){
            for(int j=0;j<=1;j++){
                in[0]= i!=0;
                in[1]= j!=0;
                assertTrue(o.evaluate(in)==o2.evaluate(in));
                o.clear();
                o2.clear();
            }
        }
        assertTrue(o.depth()==2);
        assertTrue(o2.depth()==2);
        assertTrue(o.size()==4);
        assertTrue(o2.size()==5);
    }
}