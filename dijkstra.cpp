/*****************************************************************************
Title:          Dijkstra.cpp
Author:         Nishad Sharker
******************************************************************************/

#include <iostream>
#include <fstream>

#include <string>

#include <cstdlib>//exit function

#include <sstream>
#include <vector>

#include <queue> //Priority Queue
#include <float.h> //DBL_MAX

#include <algorithm> //Reverse string to show path from starting Vertex to user specified Vertex

using namespace std;

/*******************
Vertex object

This will represent a vertex in our graph

currVertex is the number of this vertex

prevVertex is initialized to -1, and will be assigned a proper value when put into the dijkstra function

distance is initialized to the large double value and will be assigned the min distance when dijkstra is called on the graph

known is intialized to false and should be come true after the dijkstra function

*********************/

struct Vertex
{

    int currVertex;
    int prevVertex;
    double distance;
    bool known;

    Vertex(): currVertex{-1}, prevVertex{-1}, distance{DBL_MAX}, known{false} {}
    Vertex(int v): currVertex{v}, prevVertex{-1}, distance{DBL_MAX}, known{false} {}

    Vertex & operator=(const Vertex & rhs)
    {
        currVertex = rhs.currVertex;
        prevVertex = rhs.prevVertex;
        distance = rhs.distance;
        known = rhs.known;
        return *this;

    };


};

/******************
The compare function used to make a priority queue of Vertex objects

******************/
class CompareGreater{
public:
    bool const operator()(Vertex &v1, Vertex &v2){
        return (v1.distance > v2.distance);
    }



};

/********************
The graph is represented as a vector of Vertex.
This vector will only contain a lot of all the Vertices in our graph.
The edges of our graph will have to be held in an adjaceny list

********************/

struct Graph{

    vector<Vertex> g;

    Graph(int VertexNumbers){
        g.resize(VertexNumbers);
        for(int i = 0; i < VertexNumbers; i++)
        {
            Vertex v(i);
            g[i] = v;
        }
    }

};

/*****************
AdjaencyPair will be the object stored in our adjaceny list.

Adjaceny list will be a vector of vector of AdjacencyPairs.

******************/
struct AdjacencyPair
{
    double weight;
    int neighVertex;

    AdjacencyPair(double w, int pv): weight{w}, neighVertex{pv} {}
};


/*********************
dijkstra function will take a graph, an adacency list and a starting vertex and will
    fill all the vertices distance and prevVertex which will display the shortest path to
    each vertice in the graph starting from the starting vertex
*********************/

void dijkstra (Graph& a, vector<vector<AdjacencyPair> >& adjL, const int& startV)
{
    priority_queue<Vertex,vector<Vertex>,CompareGreater> q;

    //make v's distance = 0
    a.g[startV].distance = 0;

    //add v to priority q
    q.push(a.g[startV]);
    Vertex v;

    for( ; ; ){
        bool isUnknown = false;


        //Get the next vertex that will be marked as known
        //Making sure we are using an unknown Vertex
        while(!q.empty() && !isUnknown){
            v = q.top();
            q.pop();
            if(!v.known)
            {
                isUnknown=true;
            }

        }
        if(!isUnknown)
        {
            break;
        }

        //currIndex will tell us what Vertex we are currently looking at
        int currIndex = v.currVertex;

        //mark v known
        //updating v and our actual graph
        v.known = true;
        a.g[currIndex].known = true;


        //add all v's neighbors (w) to the list with teh weights
        int neighborSize = adjL[currIndex].size();

        for(int i = 0; i < neighborSize; i++)
        {
            //update the neighbor vertex with new weights
            int neighIndex = adjL[currIndex][i].neighVertex;

            if(a.g[neighIndex].distance > a.g[currIndex].distance + adjL[currIndex][i].weight)
            {
                a.g[neighIndex].distance = a.g[currIndex].distance + adjL[currIndex][i].weight;
                a.g[neighIndex].prevVertex = currIndex;
                q.push(a.g[neighIndex]);

            }
            //check if this vertex weight plus edge weight in adj list is  more than the current nieghbor's current weight
            //put them in que
            //

        }//end for


    }//end for

}//end dijkstra



int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        cerr<<"The number of arguments provided is incorrect! Closing Program. \n";
        exit(1);
    }

    ifstream graphFile;
    graphFile.open(argv[1]);

    string vertexNumString;


    getline(graphFile,vertexNumString);
    istringstream vertexNum(vertexNumString);

    int numOfVertices;

    vertexNum >> numOfVertices;

    Graph userGraph(numOfVertices);
    for(int i = 0; i < numOfVertices; i++)
    {
        Vertex v(i);
        userGraph.g[i] = v;
    }

    //Creating our Adjacency list
    vector<vector<AdjacencyPair>> AdjList;

    string fullLine;
    while(getline(graphFile,fullLine))
    {
        istringstream lineStream(fullLine);

        int vertex;
        lineStream >> vertex;


        int neighbor;
        double weight;
        lineStream >> neighbor;

        //Make vector to add into our Adjacency List
        vector<AdjacencyPair> neighborList;
        while(neighbor != -1)
        {

            lineStream >> weight;
            AdjacencyPair n(weight, neighbor);
            neighborList.push_back(n);

            lineStream >> neighbor;
        }
        //add the vector into the adj list
        AdjList.push_back(neighborList);

    }


    istringstream startString(argv[2]);

    int startVertex;

    //Grab the starting Vertex from the command line argument
    startString >> startVertex;


    if(startVertex >= numOfVertices || startVertex < 0)
    {
        cerr << "Error! Starting vertex is invalid. Exiting the program." << endl;
        cerr << "Please choose a proper starting vertex, between 0 and " << numOfVertices - 1 << endl;
        exit(1);
    }


    dijkstra(userGraph,AdjList,startVertex);

    string continueSearch = "y";
    while(continueSearch == "y")
    {
        cout << "Which vertex would you like to find the shortest path from vertex " << argv[2] << "?  ";
        int userVertex;
        cin >> userVertex;
        while (userVertex >= numOfVertices || userVertex < 0)
        {
            cout << "Sorry choose a valid vertex from 0 to "  << numOfVertices - 1 << "." << endl;
            cin >> userVertex;
        }
        string path;
        cout << "Total cost of the path: " << userGraph.g[userVertex].distance << endl;
        do{


            //UserVertex is an integer
            //Converting it into a string to add into our path
            ostringstream ss;
            ss << userVertex;

            string vertexString = ss.str();


            path += vertexString;




            userVertex = userGraph.g[userVertex].prevVertex;


        //We have hit the end of our path if userVertex is equal to -1
        if(userVertex != -1)
        {
           path+= " - ";

        }

        }while(userVertex != -1);

        //Reverse the path so that we can read the path
        //starting from the starting vertex and ending at the selected vertex
        reverse(path.begin(),path.end());
        cout << "Start: " << path << "  End" << endl;




        cout << endl;
        cout << "Enter y if you would like to search for another vertex:";
        cin >> continueSearch;
        cout << endl;
    }




    graphFile.close();
    return 0;
}
