#include "grafo.h"

Grafo::Grafo() :
    vetVertices(NULL),
    grau(0),
    matAdjacencia(NULL),
    direcionado(false)
{

}

int Grafo::nomeParaChave(string nome) {
    for(int i = 0; i < this->grau; i++)
        if(this->vetVertices[i].getNome() == nome)
            return this->vetVertices[i].getChave();

    return -1; // vertice nao existe no grafo
}

string Grafo::chaveParaNome(int chave) {
    if(chave < this->grau)
        return this->vetVertices[chave].getNome();

    return ""; // vertice nao existe no grafo
}

int* Grafo::codificaArco(string origem, string destino) {
    int* res = new int[2];

    res[0] = this->nomeParaChave(origem);
    res[1] = this->nomeParaChave(destino);

    if(res[0] == -1 or res[1] == -1) {
        delete[] res;
        return NULL; // arco nao valido, algum vertice nao existe no grafo
    }

    return res;
}

string* Grafo::codificaArco(int origem, int destino) {
    string* res = new string[2];

    res[0] = this->chaveParaNome(origem);
    res[1] = this->chaveParaNome(destino);

    if(res[0] == "" or res[1] == "") {
        delete[] res;
        return NULL; // arco nao valido, algum vertice nao existe no grafo
    }

    return res;
}

bool Grafo::existeVertice(string nome) {
    return (nomeParaChave(nome) != -1);
}

Grafo Grafo::criarGrafo(string entrada) { /// Formato da entrada: "n_vertices;nome_1,nome_2,...,nome_n;bool_direcionado;arco_1;arco_2;...;arco_n;}"
    Grafo res;
    int it_entrada = 0, it_vertices = 0, n_vertices;
    string aux;
    stringstream stream;

    Vertice* vertices;
    bool** matAdjacencia;
    bool direcionado;

    aux = "";

    if(entrada == "") {
        return Grafo();
    }

    while(entrada[it_entrada] != ';') {
        aux += entrada[it_entrada];
        it_entrada++;
    } it_entrada++;

    stream.clear();
    stream << aux;
    stream >> n_vertices;

    vertices = new Vertice[n_vertices];

    matAdjacencia = new bool*[n_vertices];
    matAdjacencia[0] = new bool [n_vertices*n_vertices];
    for(int i = 1; i < n_vertices; i++)
        matAdjacencia[i] = matAdjacencia[0] + i*n_vertices;

    for(int i = 0; i < n_vertices; i++)
        for(int j = 0; j < n_vertices; j++)
            matAdjacencia[i][j] = false;

    while(entrada[it_entrada] != ';') {
        string nome;

        if(entrada[it_entrada] == ',')
            it_entrada++;

        aux = "";

        while(entrada[it_entrada] != ',' and entrada[it_entrada] != ';') {
            aux += entrada[it_entrada];
            it_entrada++;
        }

        nome = aux;
        vertices[it_vertices].setChave(it_vertices);
        vertices[it_vertices].setNome(nome);

        it_vertices++;
    } it_entrada++;

    direcionado = entrada[it_entrada] == '1';

    it_entrada += 2;

    res.setVetVertices(vertices);
    res.setGrau(n_vertices);
    res.setDirecionado(direcionado);

    while(entrada[it_entrada] != '}') {
        string origem, destino;
        int* arco;

        aux = "";

        while(entrada[it_entrada] != ',') {
            aux += entrada[it_entrada];
            it_entrada++;
        } it_entrada++;

        origem = aux;

        aux = "";

        while(entrada[it_entrada] != ';') {
            aux += entrada[it_entrada];
            it_entrada++;
        } it_entrada++;

        destino = aux;

        arco = res.codificaArco(origem, destino);

        if(arco) {
            matAdjacencia[ arco[0] ][ arco[1] ] = true;
            if(!direcionado)
                matAdjacencia[ arco[1] ][ arco[0] ] = true;
        }
    }

    res.setMatAdjacencia(matAdjacencia);

    return res;
};

void Grafo::adicionarVertice(string nome) {
    Vertice* novoVetVertices;
    int novoGrau = this->grau + 1;
    bool** novaMatAdjacencia;

    { // prepara nova matriz de adjacencia
        novaMatAdjacencia = new bool*[novoGrau];
        novaMatAdjacencia[0] = new bool [novoGrau*novoGrau];
        for(int i = 1; i < novoGrau; i++)
            novaMatAdjacencia[i] = novaMatAdjacencia[0] + i*novoGrau;

        for(int i = 0; i < this->grau; i++) {
            for(int j = 0; j < this->grau; j++)
                novaMatAdjacencia[i][j] = this->matAdjacencia[i][j];

            novaMatAdjacencia[i][novoGrau - 1] = false;
            novaMatAdjacencia[novoGrau - 1][i] = false;
        }

        novaMatAdjacencia[novoGrau - 1][novoGrau - 1] = false;
    }

    { // prepara novo vetor de vertices
        novoVetVertices = new Vertice[novoGrau];

        for(int i = 0; i < this->grau; i++)
            novoVetVertices[i] = this->vetVertices[i];

        novoVetVertices[novoGrau - 1].setChave(novoGrau - 1);
        novoVetVertices[novoGrau - 1].setNome(nome);
    }

    if(this->vetVertices != NULL)
        delete [] this->vetVertices;
    if(this->matAdjacencia != NULL) {
        delete [] this->matAdjacencia[0];
        delete [] this->matAdjacencia;
    }

    this->vetVertices = novoVetVertices;
    this->matAdjacencia = novaMatAdjacencia;
    this->grau = novoGrau;
}

bool Grafo::adicionarArco(string entrada) { /// Formato da entrada: "nome_1,nome_2;"
    int i = 0, origem, destino;
    string aux = "";

    while(entrada[i] != ',') {
        aux += entrada[i];
        i++;
    }

    i++;

    origem = this->nomeParaChave(aux);

    aux = "";

    while(entrada[i] != ';') {
        aux += entrada[i];
        i++;
    }

    destino = this->nomeParaChave(aux);

    if(origem == -1 or destino == -1)
        return false;

    this->matAdjacencia[origem][destino] = true;
    if(!this->direcionado)
        this->matAdjacencia[destino][origem] = true;

    return true;
}

bool Grafo::removerVertice(string nome) {
    Vertice* novoVetVertices;
    int novoGrau = this->grau - 1;
    bool** novaMatAdjacencia;

    int vertice = this->nomeParaChave(nome);

    if(vertice == -1)
        return false;

    { // prepara nova matriz de adjacencia
        novaMatAdjacencia = new bool*[novoGrau];
        novaMatAdjacencia[0] = new bool [novoGrau*novoGrau];
        for(int i = 1; i < novoGrau; i++)
            novaMatAdjacencia[i] = novaMatAdjacencia[0] + i*novoGrau;

        for(int i = 0; i < novoGrau; i++) {
            for(int j = 0; j < novoGrau; j++) {
                int x, y;

                x = (i >= vertice ? i + 1 : i);
                y = (j >= vertice ? j + 1 : j);

                novaMatAdjacencia[i][j] = matAdjacencia[x][y];
            }
        }
    }

    { // prepara novo vetor de vertices
        novoVetVertices = new Vertice[novoGrau];

        for(int i = 0; i < novoGrau; i++) {
            int x = (i >= vertice ? i + 1 : i);

            novoVetVertices[i] = this->vetVertices[x];
        }

        for(int i = vertice; i < novoGrau; i++)
            novoVetVertices[i].setChave(i);
    }

    delete [] this->vetVertices;
    delete [] this->matAdjacencia[0];
    delete [] this->matAdjacencia;

    this->vetVertices = novoVetVertices;
    this->matAdjacencia = novaMatAdjacencia;
    this->grau = novoGrau;

    return true;
}

bool Grafo::removerArco(string entrada) { /// Formato da entrada: "nome_1,nome_2;"
    int i = 0;
    string origem = "", destino = "";
    int* arco;

    while(entrada[i] != ',') {
        origem += entrada[i];
        i++;
    }

    i++;

    while(entrada[i] != ';') {
        destino += entrada[i];
        i++;
    }

    arco = codificaArco(origem, destino);

    if(arco) {
        this->matAdjacencia[ arco[0] ][ arco[1] ] = false;
        if( !this->direcionado )
            this->matAdjacencia[ arco[1] ][ arco[0] ] = false;

        return true;
    }

    return false;
}

pair<bool, queue<string> > Grafo::DFS(string inicio, string fim) {
    int atual, alvo;
    bool busca = false, percorre = true, encontrado = false;
    bool* visitados;
    stack<int> nos;
    queue<string> ordem;
    pair<bool, queue<string> > resultado;

    visitados = new bool[this->grau];

    for(int i = 0; i < this->grau; i++)
        visitados[i] = false;

    if(inicio != "")
        atual = this->nomeParaChave(inicio);
    else
        atual = 0;

    if(fim != "") {
        busca = true;
        alvo = this->nomeParaChave(fim);
    }

    visitados[atual] = true;
    nos.push( atual );
    ordem.push( this->chaveParaNome(atual) );

    if(busca and atual == alvo) {
        encontrado = true;
        percorre = false;
    }

    while(percorre) {
        atual = nos.top();

        for(int i = 0; i < this->grau; i++) {
            if(matAdjacencia[atual][i] and !visitados[i]) {
                nos.push(i);
                ordem.push( this->chaveParaNome(i) );

                visitados[i] = true;

                atual = i;

                if(busca and atual == alvo) {
                    i = this->grau;
                    encontrado = true;
                    percorre = false;
                } else {
                    i = -1;
                }
            }
        }

        if(percorre) {
            nos.pop();

            if(nos.empty()) {
                for(int i = 0; i < this->grau; i++)
                    if(!visitados[i]) {
                        visitados[i] = true;
                        nos.push(i);
                        ordem.push( this->chaveParaNome(i) );

                        if(busca and i == alvo) {
                            encontrado = true;
                            percorre = false;
                        }

                        i = this->grau;
                    }

                if(nos.empty())
                    percorre = false;
            }
        }
    }

    resultado.first = encontrado;
    resultado.second = ordem;

    return resultado;
}

pair<bool, queue<string> > Grafo::BFS(string inicio, string fim) {
    int atual, alvo;
    bool busca = false, percorre = true, encontrado = false;
    bool* visitados;
    queue<int> nos;
    queue<string> ordem;
    pair<bool, queue<string> > resultado;

    visitados = new bool[this->grau];

    for(int i = 0; i < this->grau; i++)
        visitados[i] = false;

    if(inicio != "")
        atual = this->nomeParaChave(inicio);
    else
        atual = 0;

    if(fim != "") {
        busca = true;
        alvo = this->nomeParaChave(fim);
    }

    visitados[atual] = true;
    nos.push( atual );
    ordem.push( this->chaveParaNome(atual) );

    if(busca and atual == alvo) {
        encontrado = true;
        percorre = false;
    }

    while(percorre) {
        atual = nos.front();

        for(int i = 0; i < this->grau; i++) {
            if(matAdjacencia[atual][i] and !visitados[i]) {
                nos.push(i);
                ordem.push( this->chaveParaNome(i) );

                visitados[i] = true;

                if(busca and i == alvo) {
                    i = this->grau;
                    encontrado = true;
                    percorre = false;
                }
            }
        }

        if(percorre) {
            nos.pop();

            if(nos.empty()) {
                for(int i = 0; i < this->grau; i++)
                    if(!visitados[i]) {
                        visitados[i] = true;
                        nos.push(i);
                        ordem.push( this->chaveParaNome(i) );

                        if(busca and i == alvo) {
                            encontrado = true;
                            percorre = false;
                        }

                        i = this->grau;
                    }

                if(nos.empty())
                    percorre = false;
            }
        }
    }

    resultado.first = encontrado;
    resultado.second = ordem;

    return resultado;
}

int* Grafo::fechoTransitivoDireto(string vertice) {
    int atual, profundidade;
    bool percorre = true;
    int* visitados;
    queue<int> nos;
    pair<bool, queue<string> > resultado;

    visitados = new int[this->grau];

    for(int i = 0; i < this->grau; i++)
        visitados[i] = -1;

    if(vertice != "")
        atual = this->nomeParaChave(vertice);
    else
        atual = 0;

    profundidade = 0;

    visitados[atual] = profundidade;
    nos.push( atual );

    while(percorre) {
        atual = nos.front();

        profundidade = visitados[atual] + 1;

        for(int i = 0; i < this->grau; i++) {
            if(matAdjacencia[atual][i] and visitados[i] < 0) {
                nos.push(i);

                visitados[i] = profundidade;
            }
        }

        nos.pop();

        if(nos.empty())
            percorre = false;
    }

    return visitados;
}

int* Grafo::fechoTransitivoInverso(string vertice) {
    int atual, profundidade;
    bool percorre = true;
    int* visitados;
    queue<int> nos;
    pair<bool, queue<string> > resultado;

    visitados = new int[this->grau];

    for(int i = 0; i < this->grau; i++)
        visitados[i] = -1;

    if(vertice != "")
        atual = this->nomeParaChave(vertice);
    else
        atual = 0;

    profundidade = 0;

    visitados[atual] = profundidade;
    nos.push( atual );

    while(percorre) {
        atual = nos.front();

        profundidade = visitados[atual] + 1;

        for(int i = 0; i < this->grau; i++) {
            if(matAdjacencia[i][atual] and visitados[i] < 0) {
                nos.push(i);

                visitados[i] = profundidade;
            }
        }

        nos.pop();

        if(nos.empty())
            percorre = false;
    }

    return visitados;
}

pair<bool, queue<queue<string> > > Grafo::isConexo() {
    pair<bool, queue<queue<string> > > res;
    bool conexo = true, percorre = true, *interseccao, *visitados;
    int atual = 0, *bDireto, *bInverso;
    queue<string> grafo;
    queue<queue<string> > subgrafos;

    bDireto = this->fechoTransitivoDireto();
    bInverso = this->fechoTransitivoInverso();

    for(int i = 0; i < this->grau; i++)
        conexo = conexo and (bDireto[i] >= 0) and (bInverso[i] >= 0);

    res.first = conexo;

    if(!conexo) {
        interseccao = new bool[this->grau];
        visitados = new bool[this->grau];

        visitados[0] = true;

        for(int i = 1; i < this->grau; i++)
            visitados[i] = false;

        while(percorre) {
            while(!grafo.empty())
                grafo.pop();
            
            bDireto = this->fechoTransitivoDireto(this->chaveParaNome(atual));
            bInverso = this->fechoTransitivoInverso(this->chaveParaNome(atual));

            for(int i = 0; i < this->grau; i++)
                interseccao[i] = (bDireto[i] >= 0) and (bInverso[i] >= 0);

            for(int i = 0; i < this->grau; i++) {
                if(interseccao[i]) {
                    grafo.push(this->chaveParaNome(i) );

                    visitados[i] = true;
                }
            }

            subgrafos.push(grafo);

            percorre = false;

            for(int i = 0; i < this->grau and !percorre; i++) {
                if(!visitados[i]) {
                    atual = i;
                    percorre = true;
                }
            }
        }

        res.second = subgrafos;
    }

    return res;
}
