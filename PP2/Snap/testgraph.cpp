#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <queue>
#include <time.h>
#include "Snap.h"

using namespace std;
using namespace TSnap;

void prim(TPt<TNodeEDatNet<TInt, TFlt> > G, int num);
void kruskal(TPt<TNodeEDatNet<TInt, TFlt> > g, int vertices);
void dijkstra(TPt<TNodeEDatNet<TInt, TFlt> >  G, const int &SrcNId);
void floydWarshall(TPt<TNodeEDatNet<TInt, TFlt> >  G, int &vertices);
bool sortFunction(int a, int b,vector<float> distancias);
int findSet(int x, int padre[]);
void setUnion(int u, int v, int padre[]);

typedef TNodeEDatNet<TInt,TFlt>::TNodeI SnapNode;
typedef TNodeEDatNet<TInt,TFlt>::TEdgeI SnapEdge;

class default_visitor {
public:
    default_visitor() { }
    default_visitor(const int& Nodes){ }
    void DiscoverNode(int NId) {}
    void FinishNode(const int& NId) { }
    void ExamineEdge(const int& NId1, const int& NId2) { }
    void TreeEdge(const int& NId1, const int& NId2) { }
    void BackEdge(const int& NId1, const int& NId2) { }
    void FwdEdge(const int& NId1, const int& NId2) { }
};

class distanciaId
{
private:
public:
  double distancia;
  int id;
  int padre;
  distanciaId() { distancia = -1; id = 0; }
  void setDistancia(double _distancia) { distancia = _distancia; }
  void setId(int _id) { id = _id; }
  void setPadre(int _padre) { padre = _padre; }
  double getDistancia() { return distancia; }
  int getId() { return id; }
  int getPadre() { return padre; }
};

class sorter {
  vector<float> type_;
public:
  sorter(vector<float> type) : type_(type) {}
  bool operator()(int o1, int o2) const
  {
    return sortFunction(o1, o2, type_ );
  }
};

struct {
  bool operator()(distanciaId* a, distanciaId* b)
  {
    return a->getDistancia() < b->getDistancia();
  }
} comparaDistancia;

bool vertexExists(TPt<TNodeEDatNet<TInt, TFlt> >  & G, int id)
{
    for (SnapNode NI = G->BegNI(); NI < G->EndNI(); NI++)
    {
        if(NI.GetDat() == id)
        {
            return true;
        }
    }
    return false;
}

bool edgeExists(TPt<TNodeEDatNet<TInt, TFlt> >  & G, int id1, int id2)
{
    for (SnapEdge EI = G->BegEI(); EI < G->EndEI(); EI++)
    {
        if(EI.GetDstNDat() == id2 && EI.GetSrcNDat() == id1)
            return true;
    }
    return false;
}

bool edgeExists(TPt<TNodeEDatNet<TInt, TFlt> >  & G, int id1, int id2, TFlt weight)
{
    for (SnapEdge EI = G->BegEI(); EI < G->EndEI(); EI++)
    {
        if(EI.GetDstNDat() == id2 && EI.GetSrcNDat() == id1 && EI.GetDat() == weight)
            return true;
    }
    return false;
}

int findSet(int x, int padre[]) {
  if (padre[x-1] != x)
    padre[x-1] = findSet(padre[x-1], padre);
  return padre[x-1];
}

void setUnion(int u, int v, int padre[]) {
  u = findSet(u, padre);
  v = findSet(v, padre);
  padre[u-1] = v;
}

bool sortFunction(int a, int b, vector<float> distancias){
  return distancias[a-1] < distancias[b-1];
}

//DFS
void DFS(TPt<TNodeEDatNet<TInt, TFlt> >  & G){
    clock_t t;
    t = clock();
    default_visitor vis(G->GetNodes());
    TCnCom::GetDfsVisitor(G, vis);
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Tiempo en DFS: %f ms \n", time_taken*1000);
}

//BFS
void BFS(TPt<TNodeEDatNet<TInt, TFlt> >  & G){
    clock_t t;
    t = clock();
    PNGraph GBFS = TSnap::GetBfsTree(G, 1, true, true);
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Tiempo en BFS: %f ms \n", time_taken*1000);
}

//PRIM
void prim(TPt<TNodeEDatNet<TInt, TFlt> >  & G)
{
  clock_t t;
  t = clock();
  double inf = 1000000;
  int num = 1;
  distanciaId* u;
  vector<distanciaId*> delta(G->GetNodes());
  vector<int> Q(G->GetNodes());
  vector<distanciaId*> S;

  for (TNodeEDatNet< TInt, TFlt >::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++)
  {
    distanciaId* temp = new distanciaId();
    temp->setDistancia(inf);
    temp->setId(NI.GetId());
    delta[NI.GetId() - 1] = temp;
  }
  delta[num-1]->setDistancia(0);
  while (!delta.empty())
  {
    sort(delta.begin(), delta.end(), comparaDistancia);
    u = delta[0];
    for (auto i : delta)
    {
      if (G->IsEdge(u->getId(), i->getId()))
      {
        if (i->getDistancia() > G->GetEDat(u->getId(), i->getId()))
        {
          i->setPadre(u->getId());
          i->setDistancia(G->GetEDat(u->getId(), i->getId()));
        }
      }
    }
    S.push_back(delta[0]);
    delta.erase(delta.begin());
  }

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC;
  printf("Tiempo en Prim: %f ms \n", time_taken*1000);

  // for (auto i : S) {
  //    cout << i->getPadre() << "--->" << i->getId() << endl;
  //  }

}

// KRUSKAL
void kruskal(TPt<TNodeEDatNet<TInt, TFlt> >  & G) {

  clock_t t;
  t = clock();
  int vertices = G->GetNodes();
  int padre[vertices];
  vector<pair<int, TPt<TNodeEDatNet<TInt, TFlt> >::TObj::TEdgeI> > aristas;
  vector<TPt<TNodeEDatNet<TInt, TFlt> >::TObj::TEdgeI> tree;

  for (TPt<TNodeEDatNet<TInt, TFlt> >::TObj::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
    int id = NI.GetId();
    padre[id-1] = id;
  }

  for (TPt<TNodeEDatNet<TInt, TFlt> >::TObj::TEdgeI ei = G->BegEI(); ei < G->EndEI(); ei++)
    aristas.push_back(make_pair(ei.GetDat(), ei));

  sort(aristas.begin(), aristas.end());

  for (unsigned int i = 0; i < aristas.size(); ++i) {
    int u = aristas[i].second.GetSrcNId();
    int v = aristas[i].second.GetDstNId();
    if (findSet(u, padre) != findSet(v, padre)) {
      tree.push_back(aristas[i].second);
      setUnion(u, v, padre);
    }
  }

  while (!tree.empty()) {
   // cout << tree.front().GetSrcNId() << " --> " << tree.front().GetDstNId();
   // cout << " peso: " << tree.front().GetDat() << endl;
    tree.erase(tree.begin());
  }

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC;
  printf("Tiempo en Kruskal: %f ms \n", time_taken*1000);

}

//DIJKSTRA
void dijkstra(TPt<TNodeEDatNet<TInt, TFlt> >  & G){
  clock_t t;
  t = clock();
  int size = G->GetNodes();
  int v = 1;
  deque<int> noVisitados;
  vector<int> visitados;

  vector<float> distances (size,INFINITY);
  vector<int> parents (size,-1);

  for(int i=0; i<size; i++){
    noVisitados.push_back(i+1);
  }

  distances[v-1] = 0;

  while(!noVisitados.empty()){

    sort(noVisitados.begin(), noVisitados.end(), sorter(distances));
    int u = noVisitados[0];
    int sourceNode = u;

    noVisitados.pop_front();
    visitados.push_back(u);

    TNodeEDatNet<TInt, TFlt>::TNodeI NI = G->GetNI(sourceNode);

    for (int e = 0; e < NI.GetOutDeg(); e++){

      int destNode = NI.GetOutNId(e);
      TNodeEDatNet<TInt, TFlt>::TEdgeI EI = G->GetEI(sourceNode,destNode);
      float edgeWeight = (float)EI.GetDat();

      if(distances[destNode-1] > distances[sourceNode-1] + edgeWeight){
        distances[destNode-1] = distances[sourceNode-1] + edgeWeight;
        parents[destNode-1] = sourceNode;
      }
    }
  }

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC;
  printf("Tiempo en Dijkstra: %f ms \n", time_taken*1000);

   // int node = 1;
   // for (int i = 0 ; i< parents.size(); i++){
   //   cout << parents[node-1] << " -> " << node << " distance:  " << distances[node-1] << endl;
   //   node++;
   //}

}

//FLOYD WARSHALL
void floydWarshall(TPt<TNodeEDatNet<TInt, TFlt> >  & G) {
  clock_t t;
  t = clock();
  int vertices = G->GetNodes();
  int infinito = 1000000;
  vector<int> fila;
  vector< vector<int> > mat;

  for (int i = 0; i < vertices; i++) {

    for (int j = 0; j < vertices; j++) {
      fila.push_back(-2);
    }

    mat.push_back(fila);
  }

  for (int i = 0; i < vertices; i++) {

    for (int j = 0; j < vertices; j++) {

      if (i == j) {
        mat[i][j] = 0;
      }
      else if (G->IsEdge(i+1, j+1)) {
        mat[i][j] = G->GetEDat(i+1,j+1);
      }
      else {
        mat[i][j] = infinito;
      }
    }
  }

  for(int k = 0; k < vertices; ++k){

    for(int i = 0; i < vertices; ++i){

      for(int j = 0; j < vertices; ++j) {

        int temp = mat[i][k] + mat[k][j];
        if ( temp < mat[i][j] ){
          mat[i][j] = temp;
        }

      }
    }
  }

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC;
  printf("Tiempo en Floyd Warshall: %f ms \n", time_taken*1000);

   // for (int i = 0; i < vertices; i++) {
   //   for (int j = 0; j < vertices; j++) {
   //     if (mat[i][j] == infinito) {
   //       cout << "Del nodo " << i+1 << " al nodo " << j+1 << " : inf" << endl;
   //     }//cierre del if
   //     else {
   //       cout << "Del nodo " << i+1 << " al nodo " << j+1 << " : " << mat[i][j] << endl;
   //     }
   //   }
   //   cout << endl;
   // }


}

//INSERTAR VERTICE
void insertarVertice(TPt<TNodeEDatNet<TInt, TFlt> >  & G){
  clock_t t;
  int v;
  cout << "Id del nodo a agregar: ";
  while(!(cin >> v)){
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  cout << "Entrada inválida, intenta otra vez: ";
  }
  t = clock();
  if(vertexExists(G, v)){
  cout << "El vértice con id: " << v << " ya existe" << endl;
  }
  else{
  G->AddNode(v, v);
  cout << "Vértice con id: " << v << " insertado correctamente" << endl;
  }
  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC;
  printf("Tiempo en insertar vértice: %f ms \n", time_taken*1000);
}

//INSERTAR ARISTA
void insertarArista(TPt<TNodeEDatNet<TInt, TFlt> >  & G){
  clock_t t;
  int id1, id2;
  bool negativeEdge = false;
  cout << "Vértice 1: ";
  while(!(cin >> id1)){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada inválida, intenta otra vez: ";
      }

      t = clock();
      if(!vertexExists(G, id1))
        cout << "El vértice con id: " << id1 << " no existe" << endl;
        else
        {
          cout << "Vértice 2: ";
          while(!(cin >> id2)){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida, intenta otra vez: ";
          }

          if(!vertexExists(G, id2))
            cout << "El vértice con id: " << id2 << " no existe" << endl;
            else
            {
             float weight;
             cout << "Peso de la arista: ";
             while(!(cin >> weight)){
               cin.clear();
               cin.ignore(numeric_limits<streamsize>::max(), '\n');
               cout << "Entrada inválida, intenta otra vez: ";
             }
             if(edgeExists(G,id1,id2,weight))
             {
               cout << "La arista de " << id1 << " a " << id2 << " con peso " << weight << " ya existe" << endl;
             }
             else
             {
               if(weight < 0)
               {
                 negativeEdge = true;
                 cout << "No se pueden agregar aristas negativas" << endl;
               }
               G->AddEdge(id1, id2, weight);
               cout << "Se añadió la arista de " << id1 << " a " << id2 << " con peso " << weight << endl;
             }
            }
          }
   t = clock() - t;
   double time_taken = ((double)t)/CLOCKS_PER_SEC;
   printf("Tiempo en insertar arista: %f ms \n", time_taken*1000);
}

void eliminarVertice(TPt<TNodeEDatNet<TInt, TFlt> >  & G){
  clock_t t;
  int v;
  cout << "Id del nodo a borrar: ";
  while(!(cin >> v)){
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  cout << "Entrada inválida, intenta otra vez: ";
  }

  t = clock();
  SnapNode Vertex;
  if (vertexExists(G,v))
    G->DelNode(v);
  else
    cout << "El vértice con id " << v << " no existe" << endl;
  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC;
  printf("Tiempo en eliminar vertice: %f ms \n", time_taken*1000);
}

void eliminarArista(TPt<TNodeEDatNet<TInt, TFlt> >  & G){
  clock_t t;
  int id1, id2;
  cout << "Id del vértice 1 conectado por la arista: ";
    while(!(cin >> id1)){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada inválida, intenta otra vez: ";
      }
    t = clock();
      if(vertexExists(G,id1))
      {
        cout << "Id del vértice 2 conectado por la arista: ";
        while(!(cin >> id2)){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida, intenta otra vez: ";
          }

          if(vertexExists(G,id2))
          {
            if(edgeExists(G,id1,id2))
            {
              G->DelEdge(id1, id2);
              cout << "La arista de " << id1 << " a " << id2 << " fue borrada exitosamente" << endl;
            }
            else
            {
              cout << "La arista de " << id1 << " a " << id2 << " no existe" << endl;
            }
          }
          else
          {
            cout << "El vértice con id " << id2 << " no existe" << endl;
          }
        }
        else
        {
          cout << "El vértice con id " << id1 << " no existe" << endl;
        }
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Tiempo en eliminar arista: %f ms \n", time_taken*1000);
}


int main(int argc, char* argv[]) {
    TPt<TNodeEDatNet<TInt, TFlt> >  G = TNodeEDatNet<TInt, TFlt>::New();
    for(int i=1; i<15; i++)
    {
        G->AddNode(i,i);
    }

    G->AddEdge(1,4,8);
    G->AddEdge(1,3,8);
    G->AddEdge(2,5,7);
    G->AddEdge(3,10,4);
    G->AddEdge(3,2,7);
    G->AddEdge(3,5,8);
    G->AddEdge(4,7,3);
    G->AddEdge(4,5,1);
    G->AddEdge(4,8,2);
    G->AddEdge(5,6,9);
    G->AddEdge(6,13,4);
    G->AddEdge(7,4,6);
    G->AddEdge(8,9,3);
    G->AddEdge(8,7,3);
    G->AddEdge(9,10,2);
    G->AddEdge(9,12,4);
    G->AddEdge(10,3,10);
    G->AddEdge(10,6,6);
    G->AddEdge(11,12,6);
    G->AddEdge(12,11,8);
    G->AddEdge(12,9,2);
    G->AddEdge(12,14,9);
    G->AddEdge(13,14,6);
    G->AddEdge(14,13,2);

    cout << "DFS" << endl;
    DFS(G);
    cout << " " << endl;
    cout << "BFS" << endl;
    BFS(G);
    cout << " " << endl;
    cout << "PRIM" << endl;
    prim(G);
    cout << " " << endl;
    cout << "KRUSKAL" << endl;
    kruskal(G);
    cout << " " << endl;
    cout << "DIJKSTRA" << endl;
    dijkstra(G);
    cout << " " << endl;
    cout << "FLOYD WARSHALL" << endl;
    floydWarshall(G);
    cout << " " << endl;
    cout << "INSERTAR VERTICE" << endl;
    insertarVertice(G);
    cout << " " << endl;
    cout << "INSERTAR ARISTA" << endl;
    insertarArista(G);
    cout << " " << endl;
    cout << "ELIMINAR VERTICE" << endl;
    eliminarVertice(G);
    cout << " " << endl;
    cout << "ELIMINAR ARISTA" << endl;
    eliminarArista(G);
    cout << " " << endl;

}
