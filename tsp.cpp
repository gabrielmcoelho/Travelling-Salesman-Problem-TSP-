#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

#define n 20 // Define a população como tendo 10 indivíduos
#define p 9 // O número de cruzamentos por geração é igual a 4
#define c 3 // Cada disputa do torneio possui 3 concorrentes

struct Cromossomo
{
    int v[5];
    float fitness;
};

void inicializa_vetores(int v[]);
int calcula_fitness(int v[]);
int peso_entre(int a, int b);
int seleciona_pai_torneio(Cromossomo pop[]);
void cruzamento(Cromossomo c1, Cromossomo c2, Cromossomo f[]);
void validacao(int v[]);
void descobre_melhores(Cromossomo pop[], int melhores[]);
void mutacao(int v[]);
void imprime(Cromossomo pop[]);

int main ()
{
    clock_t begin = clock();
    srand(time(0));
    bool solucao = false;
    int pai1, pai2, aux=0, ger=1, indicefinal, melhorfitness;
    Cromossomo pop[n], pop2[n], filhos[2];
    /// GERANDO A POPULAÇÃO INICIAL
    for(int i=0; i<n; i++)
    {
        inicializa_vetores(pop[i].v);
        pop[i].fitness = calcula_fitness(pop[i].v);
    }
    cout << "=====================GERACAO " << ger << "=====================" << endl << endl;
    imprime(pop);
    ger++;
    while(solucao==false) // Critério de parada adotado: Tempo
    {
        /// APLICANDO MUTAÇÃO
        int mut = rand()%n;
        mutacao(pop[mut].v);
        pop[mut].fitness = calcula_fitness(pop[mut].v);
        /// COMEÇANDO AS ITERAÇÕES
        for(int i=0; i<p; i++)
        {
            /// SELEÇÃO
            pai1 = seleciona_pai_torneio(pop);
            pai2 = seleciona_pai_torneio(pop);
            /// CRUZAMENTO
            cruzamento(pop[pai1], pop[pai2], filhos);
            pop2[aux] = filhos[0];
            pop2[aux+1] = filhos[1];
            aux+=2;
        }
        /// ELITISMO
        int melhores[2];
        descobre_melhores(pop, melhores);
        pop2[aux] = pop[melhores[0]];
        pop2[aux+1] = pop[melhores[1]];
        /// SUBSTITUINDO AS GERACOES
        for(int i=0; i<n; i++)
        {
            pop[i] = pop2[i];
        }
        aux = 0;
        cout << "=====================GERACAO " << ger << "=====================" << endl << endl;
        imprime(pop);
        ger++;
        /// CHECANDO SOLUÇÃO
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        if(elapsed_secs>0.2)
        {
            melhorfitness = pop[0].fitness;
            indicefinal = 0;
            for(int i=1; i<n; i++){
                if(pop[i].fitness < melhorfitness){
                    melhorfitness = pop[i].fitness;
                    indicefinal = i;
                }
            }
            cout << "Melhor caminho: ";
            for(int i=0; i<5; i++){
                cout << pop[indicefinal].v[i] << " ";
            }
            cout << endl << "Fitness do melhor caminho: " << pop[indicefinal].fitness;
            solucao = true;
        }
    }
}

void inicializa_vetores(int v[])
{
    v[0] = rand()%5 + 1;
    do
    {
        v[1] = rand()%5 + 1;
    }
    while(v[1]==v[0]);
    do
    {
        v[2] = rand()%5 + 1;
    }
    while(v[2]==v[0] || v[2]==v[1]);
    do
    {
        v[3] = rand()%5 + 1;
    }
    while(v[3]==v[0] || v[3]==v[1] || v[3]==v[2]);
    do
    {
        v[4] = rand()%5 + 1;
    }
    while(v[4]==v[0] || v[4]==v[1] || v[4]==v[2] || v[4]==v[3]);
}

int calcula_fitness(int v[])
{
    int resultado=0;
    for(int i=0; i<4; i++)
    {
        resultado = resultado + peso_entre(v[i],v[i+1]);
    }
    return resultado;
}

int peso_entre(int a, int b)
{
    int aux = 10*a + b;
    if(aux==12 || aux==21)
    {
        return 2;
    }
    else if(aux==35 || aux==53 || aux==54 || aux==45 || aux==41 || aux==14 || aux==42 || aux==24)
    {
        return 3;
    }
    else if(aux==23 || aux==32)
    {
        return 4;
    }
    else if(aux==15 || aux==51)
    {
        return 6;
    }
    else if(aux==43 || aux==34)
    {
        return 7;
    }
    else if(aux==25 || aux==52)
    {
        return 8;
    }
    else if(aux==13 || aux==31)
    {
        return 9;
    }
}

int seleciona_pai_torneio(Cromossomo pop[])
{
    int n1, n2, n3;
    n1 = rand()%10;
    do
    {
        n2 = rand()%10;
    }
    while(n2==n1);
    do
    {
        n3 = rand()%10;
    }
    while(n3==n1 || n3==n2);
    if( (pop[n1].fitness <= pop[n2].fitness) && (pop[n1].fitness <= pop[n3].fitness) )
    {
        return n1;
    }
    else if( (pop[n2].fitness <= pop[n1].fitness) && (pop[n2].fitness <= pop[n3].fitness) )
    {
        return n2;
    }
    else
    {
        return n3;
    }
}

void cruzamento(Cromossomo c1, Cromossomo c2, Cromossomo f[])
{
    int aux1, aux2, coef = 10000;
    aux1 = c1.v[0]*10000 + c1.v[1]*1000 + c2.v[2]*100 + c2.v[3]*10 + c2.v[4];
    aux2 = c2.v[0]*10000 + c2.v[1]*1000 + c1.v[2]*100 + c1.v[3]*10 + c1.v[4];
    for(int i=0; i<5; i++)
    {
        f[0].v[i] = (aux1/coef)%10;
        coef = coef/10;
    }
    coef = 10000;
    for(int i=0; i<5; i++)
    {
        f[1].v[i] = (aux2/coef)%10;
        coef = coef/10;
    }
    validacao(f[0].v);
    f[0].fitness = calcula_fitness(f[0].v);
    validacao(f[1].v);
    f[1].fitness = calcula_fitness(f[1].v);
}

void validacao(int v[])
{
    int aux[5] = {};
    for(int i=0; i<5; i++)
    {
        aux[v[i]-1]++;
    }
    for(int i=0; i<5; i++)
    {
        if(aux[i]==0)
        {
            for(int j=0; j<5; j++)
            {
                if(aux[j]==2)
                {
                    for(int k=4; k>=0; k--)
                    {
                        if(v[k]==j+1)
                        {
                            v[k] = i+1;
                            aux[i]++;
                            aux[j]--;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void descobre_melhores(Cromossomo pop[], int melhores[])
{
    int trono = 100, melhor, segmelhor;
    for(int i=0; i<n; i++)
    {
        if(pop[i].fitness < trono)
        {
            trono = pop[i].fitness;
            melhor = i;
        }
    }
    trono = 100;
    for(int i=0; i<n; i++)
    {
        if( (pop[i].fitness < trono) && (i!=melhor) )
        {
            trono = pop[i].fitness;
            segmelhor = i;
        }
    }
    melhores[0] = melhor;
    melhores[1] = segmelhor;
}

void mutacao(int v[])
{
    int i1, i2, aux;
    i1 = rand()%5;
    do
    {
        i2=rand()%5;
    }
    while(i2==i1);
    aux = v[i1];
    v[i1] = v[i2];
    v[i2] = aux;
}

void imprime(Cromossomo pop[])
{
    for(int i=0; i<n; i++)
    {
        cout << "CROMOSSOMO " << i+1 << endl << "Caminhos: ";
        for(int j=0; j<5; j++)
        {
            cout << pop[i].v[j] << " ";
        }
        cout << endl << "Fitness: " << pop[i].fitness << endl << endl;
    }
}
