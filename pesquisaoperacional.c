/*Fernanda Sanches
Myrna Shuman*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double n [70][50];
double term_ind [50];
int ind[50];
int v, r,cont_imprime = 0;
int min = 0;
void solucao_negativa();
void z_negativo();
void pivotamento(int sai, int entra);
void imprimindo(int sai, int entra);
void ultimaparte();

int main()
{

    char restricao[3];
    int qt = 0;
    int i = 0,j = 0;
    int i_linha = 1, linha = 1, cont_base = 0, cont_term_ind = 0;

    printf ("Quantas variaveis (max 20):\n ");
    scanf ("%d", &v);
    printf ("Quantas restricoes (max 20):\n");
    scanf ("%d", &r);
    printf ("\nFuncao objetivo\n");

    for(j = 0; j < v+r*2; j++)
    {
        if (j >= v)
        {
            n[i][j] = 0;
        }
        else
        {
            printf("\nCoeficiente da variavel x%d:\n ", j);
            scanf ("%lf", &n[i][j]);
            if(n[i][j] < 0)
            {
                min = 1;
            }
            n[i][j] *= -1;
        }
    }

    for(i = 1; i <= r; i++)
    {

        printf ("\nRestricao %d\n", linha);
        linha++;

        for (j = 0; j <v; j++)
        {
            printf("Coeficiente da variavel x%d\n: ", j);
            scanf("%lf", &n[i_linha][j]);
        }

        printf("\nTipo de restricao:\n ");
        scanf("%s", restricao);

        printf ("\nTermo independente:\n ");
        scanf ("%lf", &term_ind[i_linha]);

        /* restricao == */
        if((strcmp(restricao, "==")) == 0)
        {
            int aux;
            qt++;
            i_linha++;
            for(aux = 0; aux <= v + r + qt; aux++)
            {
                n[i_linha][aux] = -n[i_linha-1][aux];
            }
            term_ind[i_linha] = -term_ind[i_linha-1] ;
        }

        /* restricao >= */
        if((strcmp(restricao, ">=")) == 0)
        {
            int aux;
            for(aux = 0; aux <= v + r+ qt; aux++)
            {
                if(n[i_linha][aux] != 0)
                {
                    n[i_linha][aux] *= -1;
                }
            }

        }
        i_linha++;

    }

    r += qt;

    int i_indice = 0;
    for(i=0; i < r+v; i++)
    {
        if(v-1 < i)
        {
            ind[i_indice++] = i;
        }
    }
    /*variaveis de folga*/
    for (i = 1; i <=r+1; i++)
    {
        for (j = v; j < r + v; j++)
        {
            if (i+1 == j)
            {
                n[i][j] = 1;
            }
            else
            {
                n[i][j] = 0;
            }
        }
    }

    int in = 0;
    for(i = 0; i < r+1; i++)
    {
        if(i == 0)
        {
            int k;
            printf("\tBase");
            for(k = 0; k < r+v; k++)
            {
                printf("\tx%d",k);
            }
            printf("\tSolucao\n");
            printf("\tz");
            for(k = 0; k < r + v ; k++)
            {
                printf("\t%.2lf",n[0][k]);
            }
            printf("\t%.2lf\n",term_ind[i]);
        }
        else
        {
            printf("\tx%d",ind[i-1]);
            for(j = 0; j < r + v; j++)
            {
                printf("\t%.2lf", n[i][j]);
            }
            printf("\t%.2lf\n", term_ind[i]);
        }
    }

    do
    {

        cont_base = 0;
        cont_term_ind = 0;

        for(j = 0; j < r + v; j++)
        {
            if(n[0][j] < 0) cont_base++;
        }

        for(i = 1; i <= r; i++)
        {
            if(term_ind[i] < 0) cont_term_ind++;
        }

        if(cont_term_ind > 0)
        {
            solucao_negativa();

        }
        else if(cont_base > 0)
        {
            z_negativo();
        }
    }
    while((cont_base > 0 || cont_term_ind > 0) || (cont_base > 0 && cont_term_ind > 0));

    ultimaparte();
    return 0;
}

void solucao_negativa()
{

    int i,j,sai = 1, i_sai = 0, entra = 0, i_entra = 0;
    double razao[r+v],aux, maior_razao = 0;
    int kill = 0;

    aux = term_ind[1];
    for(i=2; i <= r;  i++)
    {
        if(term_ind[i] < aux)
        {
            aux = term_ind[i];
            i_sai = ind[i-2];
            sai = i;
        }
    }
    maior_razao = 0;
    kill = 0;
    /*Calcula a razao */
    for(i=0; i < r+v; i++)
    {

        if(n[sai][i] < 0)
        {
            if(n[0][i] != 0 && n[sai][i] != 0)
            {
                if(kill == 0)
                {
                    maior_razao = (n[0][i] / n[sai][i]);
                    i_entra = i;
                    kill = 1;
                }
                else if(fabs((n[0][i] /n[sai][i])) < fabs(maior_razao))
                {
                    maior_razao = (n[0][i] / n[sai][i]);
                    i_entra = i;
                }
            }
        }
    }


    ind[sai-1] = i_entra;

    pivotamento(sai,i_entra);
    imprimindo(i_sai+1, i_entra);
}

void z_negativo()
{

    int i,j;
    int entra, sai, i_sai, maior_z = 0;
    double razao[40],menor_pos_razao = 100000;

    /* Acha o mais negativo do Z */
    for(i = 0; i < r+v; i++)
    {
        if(n[0][i] <  maior_z)
        {
            maior_z = n[0][i];
            entra = i;
        }
    }

    /*Calcula a razao */
    for(i = 1; i <= r; i++)
    {
        razao[i] = (double)(term_ind[i] / n[i][entra]);
    }

    /*Define a menor razao positiva, e acha o indice que sai */
    for(i = 1; i <= r; i++)
    {
        if(razao[i] < menor_pos_razao && razao[i] > 0)
        {
            menor_pos_razao = razao[i];
            i_sai = ind[i-1];
            sai = i;
        }
    }

    ind[sai-1] = entra;

    pivotamento(sai,entra);
    imprimindo(i_sai, entra);
}

void pivotamento(int sai, int entra)
{

    double pivo = n[sai][entra];
    double NLP[60], quociente;
    int i,j;

    for(j=0; j < r + v; j++)
    {
        NLP[j] = n[sai][j] / pivo;
        n[sai][j] /= pivo;
    }
    term_ind[sai]/= pivo;
    NLP[r+v] = term_ind[sai];

    for(i = 0; i < r+1; i++)
    {
        quociente = n[i][entra];
        if(i!=sai)
        {
            for(j = 0; j < r+v; j++)
            {
                if(fabs((NLP[j] * (quociente*(-1)))) == fabs(n[i][j]))
                {
                    n[i][j] = 0;
                }
                else
                {
                    n[i][j] = NLP[j] * (quociente*(-1)) + n[i][j];

                }

            }
            term_ind[i] = NLP[r+v] * (quociente*(-1)) + term_ind[i];
        }
    }

}

void imprimindo(int sai, int entra)
{

    int i,j;

    cont_imprime++;

    printf("\tInteracao %d: sai x%d, entra x%d\n", cont_imprime,sai,entra);
    for(i = 0; i < r+1; i++)
    {
        if(i == 0)
        {
            int k;
            printf("\tBase");
            for(k = 0; k < r+v; k++)
            {
                printf("\tx%d",k);
            }
            printf("\tSolucao\n");
            printf("\tz");
            for(k = 0; k < r + v ; k++)
            {
                printf("\t%.2lf",n[0][k]);
            }
            printf("\t%.2lf\n",term_ind[i]);
        }
        else
        {
            printf("\tx%d",ind[i-1]);
            for(j = 0; j < r + v; j++)
            {
                printf("\t%.2lf", n[i][j]);
            }
            printf("\t%.2lf\n", term_ind[i]);
        }
    }

}

void ultimaparte()
{

    int i = 0, j = 1, k = 0;
    printf("\n\n\tSolucao: %.2lf", term_ind[0]);

    do
    {
        if(ind[i] == k)
        {
            printf("\n\tx%d = %.2lf", k, term_ind[j]);
            k++;
            j = 1;
            i = 0;
        }
        else
        {
            i++;
            j++;
        }
    }
    while (k != v);
    printf("\n\n");

}
