#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXSATIR 513

//temel hatlari belirliyorum. her sey yeniden guzel bir sekilde belirlenebilir

char ***tokenAyir(char *);   // tum komutlari belirle (ileride liste kullanilarak yeniden duzenlenebilir)
int calistir(char ***); //execute commands
int komutSayisi;             //toplam komut sayilari
int maxArgumanSayisi;        //tespit edilen maksimumu komut argumani
int komutlariTemizle(char ***);

int main(int argc, char *argv[])
{
    char satir[MAXSATIR];
    char ***deneme;
    if (argc > 2)
    {
        fprintf(stderr, "maksimum 1 arguman girebilirsiniz\n");
        exit(-1);
    }
    if (argc == 2)
    {
        //bash gelecek
    }
    else
    {
        while (1)
        {
            komutSayisi = 0;
            maxArgumanSayisi = 0;
            printf("prompt> ");
            if (fgets(satir, MAXSATIR, stdin) != NULL)
            {
                deneme = tokenAyir(satir);
                calistir(deneme);
                komutlariTemizle(deneme);
            }
        }
    }

    return 0;
}

char ***tokenAyir(char *satir)
{
    if (satir[MAXSATIR - 2] != '\0')
    {
        fprintf(stderr, "Cok uzun bir komut satiri (bu proje icin '\\n' dahil 512 karakterden fazla)\n");
        satir[MAXSATIR - 2] = '\0';
    }

    char **tokens = malloc(sizeof(char *) * MAXSATIR);
    int tokenSayisi = 0;
    int argumentSira = 0;

    char ***tempCommandTokens = malloc(sizeof(char **) * MAXSATIR);
    for (int i = 0; i < MAXSATIR; i++)
    {
        tempCommandTokens[i] = malloc(sizeof(char *) * MAXSATIR);
    }

    tokens[0] = strtok(satir, ";");
    if (tokens[0] != NULL)
    {
        tokenSayisi++;
        for (int i = 1; i < MAXSATIR; i++)
        {
            tokens[i] = strtok(NULL, ";");
            if (tokens[i] == NULL)
            {
                break;
            }
            tokenSayisi++;
        }
    }

    for (int i = 0; i < tokenSayisi; i++)
    {
        tempCommandTokens[komutSayisi][argumentSira] = strtok(tokens[i], " \n\t");
        if (tempCommandTokens[komutSayisi][argumentSira] != NULL)
        {
            while (tempCommandTokens[komutSayisi][argumentSira] != NULL)
            {
                argumentSira++;
                tempCommandTokens[komutSayisi][argumentSira] = strtok(NULL, " \n\t");
            }
            komutSayisi++;
        }

        if (argumentSira > maxArgumanSayisi)
        {
            maxArgumanSayisi = argumentSira;
        }
        argumentSira = 0;
    }

    argumentSira = 0;

    free(tokens);

    return tempCommandTokens;
}

int calistir(char ***komutlar)
{
    pid_t pid, wpid;
    int status = 0;
    for (int i = 0; i < komutSayisi; i++)
    {
        if ((pid = fork()) == 0) // child processler olusturuluyor
        {
            int say = 0;
            char *argumanlar[maxArgumanSayisi];
            printf("cocuk olustu");
            while (komutlar[0][say] != NULL)
            {
                argumanlar[say] = komutlar[0][say];
                say++;
            }
            argumanlar[say] = NULL;
            printf("%d\n", execvp(argumanlar[0], argumanlar));
            exit(0);
        }
    }
    while ((wpid = wait(&status)) > 0); //tum processlerin bitmesini bekle
    return 1;
}

int komutlariTemizle(char ***komutlar) //heapte tutulan yerleri memory leak olmamasi icin birak (daha sonra liste yapilirsa degistirilebilir)
{
    for (int i = 0; i < komutSayisi; i++)
    {
        free(komutlar[i]);
    }
    free(komutlar);
    return 1;
}