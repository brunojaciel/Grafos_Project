#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include <sstream>
using namespace std;

#include <stack>
#include <queue>
#include <utility>

class Vertice; // prototipo da classe auxiliar para os vertices

class Grafo {
private:
    Vertice* vetVertices; // vertices do grafo
    int grau; // numero de vertices
    bool** matAdjacencia; // matriz de adjacencia
    bool direcionado; // indica se grafo direcionado ou nao


    int nomeParaChave(string nome); // retorna indice de um vertice pelo nome
    string chaveParaNome(int chave); // retorna nome de um vertice pela chave

public:
    Grafo(); // construtor


    static Grafo criarGrafo(string entrada = ""); // retorna grafo a partir de uma string geradora (funcao estatica)

    int* codificaArco(string origem, string destino); // retorna um vetor com as duas chaves equivalentes aos vertices indicados
    string* codificaArco(int origem, int destino); // retorna um vetor com as dois nomes equivalentes aos vertices indicados
    bool existeVertice(string nome); // retorna verdadeiro caso vertice exista no grafo

    void adicionarVertice(string nome);
    bool adicionarArco(string entrada);
    bool removerVertice(string nome);
    bool removerArco(string entrada);

    pair<bool, queue<string> > DFS(string inicio = "", string fim = "");
    pair<bool, queue<string> > BFS(string inicio = "", string fim = "");

    int* fechoTransitivoDireto(string vertice = "");
    int* fechoTransitivoInverso(string vertice = "");

    pair<bool, queue<queue<string> > > isConexo();



    // gets e sets dos atributos
    Vertice* getVetVertices() const { return this->vetVertices; };
    void setVetVertices(Vertice* data) { this->vetVertices = data; };

    int getGrau() const { return this->grau; };
    void setGrau(int data) { this->grau = data; };

    bool** getMatAdjacencia() const { return this->matAdjacencia; };
    void setMatAdjacencia(bool** data) { this->matAdjacencia = data; };

    bool getDirecionado() const { return this->direcionado; };
    void setDirecionado(bool data) { this->direcionado = data; };
};

class Vertice {
private:
    int chave; // chave identificadora do vertice
    string nome; // nome do vertice

public:
    Vertice() {}; // construtor
    Vertice(int chave, string nome) : chave(chave), nome(nome) {}; // construtor inicializado

    // gets e sets dos atributos
    int getChave() { return this->chave; };
    void setChave(int data) { this->chave = data; };
    string getNome() { return this->nome; };
    void setNome(string data) { this->nome = data; };
};

#endif // GRAFO_H
