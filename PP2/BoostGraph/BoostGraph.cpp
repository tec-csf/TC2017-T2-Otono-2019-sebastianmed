/*

  Código con grafo ya establecido (se encuentra en el main)
  Se realizan y se calcula el tiempo de insertar un vertice y una arista, eliminar un vertice y una arista, DFS, BFS, Prim, Kruskal, Dijkstra y Floyd Warshall
  Equipo: Sebastián Medina  y Carlos de la Garza

*/


#include <iostream>
#include <deque>
#include <iterator>
#include <vector>
#include <time.h>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/graph/exterior_property.hpp>

using namespace std;
using namespace boost;

struct node{
    float id;
};

typedef property<edge_weight_t, float> EdgeWeightProperty;
typedef adjacency_list<listS,vecS,directedS,node,EdgeWeightProperty> DirectedGraph;
typedef property_map<DirectedGraph, edge_weight_t>::type EdgeWeightMap;
typedef graph_traits<DirectedGraph>::vertex_descriptor vertex_descriptor;
typedef graph_traits < DirectedGraph >::edge_descriptor edge_descriptor;
typedef exterior_vertex_property<DirectedGraph, float> DistanceProperty;
typedef DistanceProperty::matrix_type DistanceMatrix;
typedef DistanceProperty::matrix_map_type DistanceMatrixMap;
typedef pair<int, int> Edge;


vertex_descriptor addVertex(DirectedGraph & g, int n)
{
    vertex_descriptor v0 = add_vertex(g);
    g[v0].id = n;
    return v0;
}

void addEdge(int vertex1, int vertex2, float weight, DirectedGraph & g)
{
    add_edge(vertex1-1, vertex2-1, weight, g);
}

bool vertexExists(DirectedGraph & g, int n, int & t)
{
   int pos = 0;
   DirectedGraph::vertex_iterator start,end;
   for(tie(start,end) = vertices(g); start!=end; start++)
   {
       if(g[*start].id == n)
       {
           t = pos;
           return true;
       }
       pos++;
   }
   return false;
}

bool edgeExists(DirectedGraph & g, int v1, int v2, float w)
{
   DirectedGraph::edge_iterator edgeIt, edgeEnd;
   for (tie(edgeIt, edgeEnd) = edges(g); edgeIt != edgeEnd; ++edgeIt)
   {
       if(source(*edgeIt, g) == v1 && target(*edgeIt, g) == v2 && get(edge_weight, g, *edgeIt) == w)
           return true;
   }
   return false;
}


void insertarVertice(DirectedGraph & g){
  clock_t t;
  int vertice;
  cout << "Ingrese el nodo que quiera agregar: ";
  while(!(cin >> vertice)){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Intente otra vez: ";
    }
  int posicion;
  if(vertexExists(g, vertice, posicion)) {
     cout << "El vértice: " << vertice << " ya existe" << endl;
  } else {
    t = clock();
    vertex_descriptor v0 = addVertex(g,vertice);
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Tiempo en insertar vértice: %f ms \n", time_taken*1000);
  }
  cout << "" << endl;
}

void insertarArista(DirectedGraph & g){
  clock_t t;
  bool negativeEdge = false;
  bool unDirected = false;
  float v1, v2;
  int pos1, pos2;
  cout << "Vértice 1: ";
  while(!(cin >> v1)){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Entrada inválida, intenta otra vez: ";
  }
  if(!vertexExists(g, v1, pos1)) {
    cout << "El vértice con id " << v1 << " no existe, debe crearse antes" << endl;
  } else {
      cin.ignore();
      cout << "Vértice 2: ";
        while(!(cin >> v2)){
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
          cout << "Entrada inválida, intenta otra vez: ";
        }
        if(!vertexExists(g, v2, pos2)) {
          cout << "El vértice con id " << v2 << " no existe, debe crearse antes " << endl;
        } else {
            float weight;
            cin.ignore();
            cout << "Peso de la arista a insertar: ";
            while(!(cin >> weight)){
              cin.clear();
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              cout << "Entrada inválida, intenta otra vez: ";
            }
      EdgeWeightProperty w = weight;
      if(edgeExists(g,pos1,pos2,weight)) {
        cout << "La arista de " << pos1 << " a " << pos2 << " con peso " << weight << " ya existe" << endl;
      } else {
        if(weight < 0) {
          negativeEdge = true;
          cout << "No se pueden agregar aristas negativas" << endl;
        }
        t = clock();
        add_edge(pos1,pos2,w,g);
        if(unDirected){
          add_edge(pos2,pos1,w,g);
        }
        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC;
        printf("Tiempo en insertar arista: %f ms \n", time_taken*1000);
      }
    }
  }
  cout << "" << endl;
}

void eliminarVertice(DirectedGraph & g){
  clock_t t;
  float vertice;
  int pos;
  cout << "Ingrese el nodo que quiera borrar: ";
  while(!(cin >> vertice)){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Entrada inválida, intenta otra vez: ";
  }
  if(vertexExists(g, vertice, pos)){
    graph_traits<DirectedGraph>::vertex_iterator start,end,next;
    tie(start, end) = vertices(g);
    for (next = start; start != end; start = next) {
      ++next;
      if(g[*start].id == vertice){
        t = clock();
        clear_vertex(*start, g);
        remove_vertex(*start,g);
        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC;
        printf("Tiempo en borrar vértice: %f ms \n", time_taken*1000);
      }
    }
  } else {
      cout << "El vértice con id " << vertice << " no existe " << endl;
  }
  cout << "" << endl;
}

void eliminarArista(DirectedGraph & g){
  clock_t t;
  float v1,v2;
  int pos1,pos2;
  cout << "Id del vértice 1 conectado por la arista: ";
  while(!(cin >> v1)){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Entrada inválida, intenta otra vez: ";
  }
  if(vertexExists(g, v1,pos1)) {
    cin.ignore();
    cout << "Id del vértice 2 conectado por la arista: ";
    while(!(cin >> v2)){
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Entrada inválida, intenta otra vez: ";
    }
    if(vertexExists(g, v2,pos2)){
      if(edge(pos1,pos2,g).second){
        pair<DirectedGraph::edge_descriptor, bool> retrievedEdge = edge(pos1, pos2, g);
        t = clock();
        remove_edge(retrievedEdge.first,g);
        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC;
        printf("Tiempo en borrar arista: %f ms \n", time_taken*1000);
      } else {
          cout << "La arista de " << v1 << " a " << v2 << " no existe" << endl;
      }
    } else {
        cout << "El vértice con id " << v2 << " no existe " << endl;
      }
  } else {
      cout << "El vértice con id " << v1 << " no existe " << endl;
  }
  cout << "" << endl;
}

void DFS(DirectedGraph & g){

  clock_t t;
  default_dfs_visitor vis;
  t = clock();
  depth_first_search(g, visitor(vis));
  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC;
  printf("Tiempo de DFS: %f ms \n", time_taken*1000);
  cout << "" << endl;

}

void BFS(DirectedGraph & g){

  clock_t t;
  default_bfs_visitor vis;
  t = clock();
  breadth_first_search(g, vertex(0, g), visitor(vis));
  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC;
  printf("Tiempo de BFS: %f ms \n", time_taken*1000);
  cout << "" << endl;


}

void Prim(DirectedGraph & g){

  clock_t t;
  vector <vertex_descriptor> p(num_vertices(g));
  t = clock();
  prim_minimum_spanning_tree(g, &p[0]);
  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC;
  printf("Tiempo de Prim: %f ms \n", time_taken*1000);

  cout << "" << endl;

  for (size_t i = 0; i != p.size(); ++i)
  {
      if (p[i] != i)
          cout << g[p[i]].id << " --- " << g[i].id << endl;
      else
          cout << "start -> " << g[i].id << endl;
  }

  cout << "" << endl;

}

void Kruskal(DirectedGraph & g){

  clock_t t;
  property_map<DirectedGraph, edge_weight_t>::type weight = get(edge_weight, g);
  vector <edge_descriptor> spanning_tree;
  t = clock();
  kruskal_minimum_spanning_tree(g, back_inserter(spanning_tree));
  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC;
  printf("Tiempo de Kruskal: %f ms \n", time_taken*1000);
  cout << "" << endl;

}

void Dijkstra(DirectedGraph & g){

  clock_t t;
  vector<vertex_descriptor> p(num_vertices(g));
  vector<int> d(num_vertices(g));
  vertex_descriptor s = vertex(0, g);
  t = clock();
  dijkstra_shortest_paths(g, s, predecessor_map(&p[0]).distance_map(&d[0]));
  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC;
  printf("Tiempo de Dijkstra: %f ms \n", time_taken*1000);
  cout << "" << endl;

}

void FloydWarshall(DirectedGraph & g){

  clock_t t;
  map<vertex_descriptor, map<vertex_descriptor, float> >matrix;
  EdgeWeightMap weight_pmap = get(edge_weight, g);
  DistanceMatrix dist(num_vertices(g));
  DistanceMatrixMap dm(dist, g);
  t = clock();
  bool noNegCycles = floyd_warshall_all_pairs_shortest_paths (g, dm, weight_map(weight_pmap));
  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC;
  printf("Tiempo de Floyd Warshall: %f ms \n", time_taken*1000);
  cout << "" << endl;

}


int main(int argc, const char * argv[]){

  DirectedGraph g;
  vector<vertex_descriptor> verts;

  for(int i=1; i<15; i++)
  {
      verts.push_back(addVertex(g, i));
  }

  addEdge(1,4,8,g);
  addEdge(1,3,8,g);
  addEdge(2,5,7,g);
  addEdge(3,10,4,g);
  addEdge(3,2,7,g);
  addEdge(3,5,8,g);
  addEdge(4,7,3,g);
  addEdge(4,5,1,g);
  addEdge(4,8,2,g);
  addEdge(5,6,9,g);
  addEdge(6,13,4,g);
  addEdge(7,4,6,g);
  addEdge(8,9,3,g);
  addEdge(8,7,3,g);
  addEdge(9,10,2,g);
  addEdge(9,12,4,g);
  addEdge(10,3,10,g);
  addEdge(10,6,6,g);
  addEdge(11,12,6,g);
  addEdge(12,11,8,g);
  addEdge(12,9,2,g);
  addEdge(12,14,9,g);
  addEdge(13,14,6,g);
  addEdge(14,13,2,g);

  cout << "" << endl;

  insertarVertice(g);
  insertarArista(g);
  eliminarVertice(g);
  eliminarArista(g);
  DFS(g);
  BFS(g);
  Prim(g);
  Kruskal(g);
  Dijkstra(g);
  FloydWarshall(g);



}
