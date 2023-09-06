#include "grafo.h"
#include <windows.h>

void imprimirGrafo(Grafo grafo)
{
    Vertice* vetor = grafo.getVetVertices();
    bool** matriz = grafo.getMatAdjacencia();
    int grau = grafo.getGrau();

    for(int i = 0; i < grau; i++)
    {
        cout << vetor[i].getNome() << endl;
    }

    cout << endl;

    cout << "Grau: " << grau << endl;
    cout << "Direcionado: " << (grafo.getDirecionado() ? "Sim" : "Nao") << endl << endl << endl;

    for(int i = 0; i < grau; i++)
    {
        for(int j = 0; j < grau; j++)
        {
            cout << matriz[i][j] << " ";
        }
        cout << "\n";
    }
}

void novoGrafo(Grafo &grafo) {
    int n_vertices, n_arcos;
    bool direcionado;
    string aux, entrada = "";
    string* nomes;

    cout << endl << "Indique o numero de vertices do grafo: ";
    cin >> n_vertices;
    cout << endl;

    nomes = new string[n_vertices];

    entrada += to_string(n_vertices) + ";";

    for(int i = 0; i < n_vertices; i++) {
        cout << "Digite o nome do vertice " << i + 1 << ": ";
        cin >> aux;

        nomes[i] = aux;

        entrada += aux;
        entrada += (i < n_vertices - 1 ? "," : ";");
    }

    cout << "O grafo e direcionado? (1 - sim; 0 - nao)" << endl;
    cin >> aux;

    direcionado = (aux == "1");

    entrada += (direcionado ? "1;" : "0;");

    for(int i = 0; i < n_vertices; i++) {
        cout << endl << (direcionado ? "Indique o numero de arcos originados do vertice " : "Indique o numero de arestas do vertice ") << i + 1 << ": ";
        cin >> n_arcos;
        cout << endl;

        for (int j = 0; j < n_arcos; j++) {
            cout << "Destino " << j + 1 << ": ";
            cin >> aux;

            entrada += nomes[i] + "," + aux + ";";
        }
    }

    entrada += "}";

    delete[] nomes;

    cout << entrada;

    grafo = Grafo::criarGrafo(entrada);
}

void adicionarVertice(Grafo &grafo)
{
    string nome;
    cout << "Digite o nome do vertice: ";
    cin >> nome;
    grafo.adicionarVertice(nome);
}

void adicionarArco(Grafo &grafo)
{
    string entrada = "", origem, destino;
    cout << "Digite o vertice de origem: ";
    cin >> origem;
    cout << endl << "Digite o vertice de destino: ";
    cin >> destino;
    entrada = origem + "," + destino + ";";

    if (!grafo.adicionarArco(entrada)) {
        cout << "Houve um erro ao adicionar!" << endl;
        Sleep(3000);
    }
}

void removerVertice(Grafo &grafo)
{
    string nome;
    cout << "Digite o nome do vertice: ";
    cin >> nome;

    if (!grafo.removerVertice(nome)) {
        cout << "Houve um erro ao remover!" << endl;
        Sleep(3000);
    }
}

void removerArco(Grafo &grafo)
{
    string entrada = "", origem, destino;
    cout << "Digite o vertice de origem: ";
    cin >> origem;
    cout << endl << "Digite o vertice de destino: ";
    cin >> destino;
    entrada = origem + "," + destino + ";";

    if (!grafo.removerArco(entrada)) {
        cout << "Houve um erro ao remover!" << endl;
        Sleep(3000);
    }
}

void dfs(Grafo grafo)
{
    pair <bool, queue<string> > retorno;
    int parada;
    string inicio, verticeParada;
    do
    {
        do {
            cout << "Escolha o vertice inicial: ";
            cin >> inicio;

            if(!grafo.existeVertice(inicio))
                cout << "Vertice invalido." << endl << endl;
        } while(!grafo.existeVertice(inicio));

        cout << "Deseja buscar um vertice especifico? (1 - sim; 0 - nao)" << endl;
        cin >> parada;
        cout << endl;

        if (parada == 1)
        {
            cout << "Digite o nome do vertice de parada: ";
            cin >> verticeParada;

            retorno = grafo.DFS(inicio, verticeParada);

            if (!retorno.first)
                cout << endl << "O vertice nao foi encontrado!" << endl << endl;
            else
                cout << endl << "O vertice foi encontrado!" << endl << endl;

        }
        else
        {
            retorno = grafo.DFS(inicio);
        }

        cout << "Ordem percorrida: ";
        while(!retorno.second.empty())
        {
            cout << retorno.second.front() << "   ";
            retorno.second.pop();
        }

        cout << endl << endl << "Deseja fazer outra busca? (1 - sim; 0 - nao)";
        cin >> parada;
    }
    while (parada == 1);
}

void bfs(Grafo grafo)
{
    pair <bool, queue<string> > retorno;
    int parada;
    string inicio, verticeParada;
    do
    {
        do {
            cout << "Escolha o vertice inicial: ";
            cin >> inicio;

            if(!grafo.existeVertice(inicio))
                cout << "Vertice invalido." << endl << endl;
        } while(!grafo.existeVertice(inicio));

        cout << "Deseja buscar um vertice especifico? (1 - sim; 0 - nao)" << endl;
        cin >> parada;
        cout << endl;

        if (parada == 1)
        {
            cout << "Digite o nome do vertice de parada: ";
            cin >> verticeParada;

            retorno = grafo.BFS(inicio, verticeParada);

            if (!retorno.first)
                cout << endl << "O vertice nao foi encontrado!" << endl << endl;
            else
                cout << endl << "O vertice foi encontrado!" << endl << endl;

        }
        else
        {
            retorno = grafo.BFS(inicio);
        }

        cout << "Ordem percorrida: ";
        while(!retorno.second.empty())
        {
            cout << retorno.second.front() << "   ";
            retorno.second.pop();
        }

        cout << endl << endl << "Deseja fazer outra busca? (1 - sim; 0 - nao)";
        cin >> parada;
    }
    while (parada == 1);
}

void conectividade(Grafo grafo) {
    pair<bool, queue<queue<string> > > res;
    queue<string> aux;
    
    res = grafo.isConexo();

    cout << (res.first ? "O grafo e conexo.\n" : "O grafo nao e conexo.\n\n");

    if(!res.first) {
        cout << "Subgrafos conexos: \n";

        while(!res.second.empty()) {
            cout << " - ";

            aux = res.second.front();
            res.second.pop();

            while(!aux.empty()) {
                cout << aux.front();
                aux.pop();

                cout << (!aux.empty() ? ", " : "\n");
            }
        }
    }
    
    system("pause");
}

int main()
{
    Grafo grafo = Grafo::criarGrafo();
    bool saida = false;
    int sel;

    while (!saida)
    {
        imprimirGrafo(grafo);
        cout << endl << endl;
        cout << "1 - Novo Grafo " << endl;
        cout << "2 - Adicionar vertice " << endl;
        cout << "3 - Adicionar aresta/arco " << endl;
        cout << "4 - Remover vertice " << endl;
        cout << "5 - Remover aresta/arco " << endl;
        cout << "6 - Executar DFS " << endl;
        cout << "7 - Executar BFS " << endl;
        cout << "8 - Avaliar Conectividade " << endl;
        cout << "9 - Sair ";

        cout << endl << endl;
        cin >> sel;

        cout << endl << endl;
        switch (sel)
        {
        case 1:
            novoGrafo(grafo);
            break;
        case 2:
            adicionarVertice(grafo);
            break;
        case 3:
            adicionarArco(grafo);
            break;
        case 4:
            removerVertice(grafo);
            break;
        case 5:
            removerArco(grafo);
            break;
        case 6:
            dfs(grafo);
            break;
        case 7:
            bfs(grafo);
            break;
        case 8:
            conectividade(grafo);
            break;
        case 9:
            saida = true;
            break;
        }

        system("cls");
    }

    return 0;
}


