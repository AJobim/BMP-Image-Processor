#include <stdio.h>
#include <stdlib.h>

//codigo para deixar as coisas preto e branco

#pragma pack (push, 1)

//struct com os dados de cabeçalho

typedef struct
{
    unsigned char assinatura[2];
    unsigned int tam_arq;
    unsigned int reservado;
    unsigned int offset_dados;
    unsigned int tamanho_header;
    int largura;
    int altura;
    unsigned short planos;
    unsigned short bit_p_pixel;
    unsigned int compress;
    unsigned int tam_dados;
    int res_vert;
    int res_horiz;
    unsigned int cores;
    unsigned int cores_importantes;
} header_bmp;
#pragma pack (pop)

int main()
{
    char nome[20];

    printf ("\nDigite o nome do arquivo:  ");
    fgets(nome, sizeof(nome), stdin);

    nome[strcspn(nome, "\n")] = '\0'; //Nunca esqueça de colocar isso

    strcat(nome, ".bmp");

    FILE *entrada = fopen (nome, "rb");
        if (entrada==NULL)
        {
            perror ("Nao existe arquivo de entrada");
            return 1;
        }

    header_bmp header;
    fread (&header, sizeof(header_bmp), 1, entrada);

    if (header.bit_p_pixel != 24)
    {
        printf ("Imagem de BMP nao eh de 24 bits p/ pixel");
        fclose (entrada);
        return 1;
    }

    int tam_img = header.largura * header.altura * 3;

    unsigned char *imgdados = (unsigned char *) malloc(tam_img);

    if (imgdados == NULL)
    {
        perror ("Erro de alocacao de memoria para os dados da imagem");
        fclose (entrada);
        return 1;
    }

    fread (imgdados, 1, tam_img, entrada);
    fclose (entrada);


    FILE *saida1 = fopen ("invertecor.bmp", "wb");
        if (entrada==NULL)
        {
            perror ("Erro criando arquivo de saida");
            free (imgdados);
            return 1;
        }

    for (int i = 0; i < tam_img; i+=3)
    {
        unsigned char red   = imgdados[i];
        unsigned char green = imgdados[i+1];
        unsigned char blue  = imgdados[i+2];
    //agora fazemos a troca de valores

        unsigned char r = imgdados[i]   = green;
        unsigned char g = imgdados[i+1] = blue;
        unsigned char b = imgdados[i+2] = red;
    }

    fwrite (&header, sizeof(header_bmp), 1, saida1);
    fwrite (imgdados, 1, tam_img, saida1);
    fclose (saida1);

    //parte do tons de cinza
    for (int i = 0; i < tam_img; i+=3)
    {
        unsigned char red = imgdados[i];
        unsigned char green = imgdados[i+1];
        unsigned char blue = imgdados[i+2];

        unsigned char gray = (red + green + blue)/3;

        imgdados[i] = imgdados[i+1] = imgdados[i+2] = gray;
    }

    // aqui ele passa tudo pra saida
    FILE *saida2 = fopen ("tonsdecinza.bmp", "wb");
        if (entrada==NULL)
        {
            perror ("Erro criando arquivo de saida");
            free (imgdados);
            return 1;
        }

    fwrite (&header, sizeof(header_bmp), 1, saida2);
    fwrite (imgdados, 1, tam_img, saida2);
    fclose (saida2);
    free (imgdados);

    printf ("Conversao realizada com sucesso! Cheque a imagem de saida na pasta deste codigo");
    return 0;

}
