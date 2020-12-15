#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXSATIR 513

char ***tokenAyir(char *); // tum komutlari belirle
int calistir(char ***);    //execute commands
int komutSayisi;           //toplam komut sayilari
int maxArgumanSayisi;      //tespit edilen maksimumu komut argumani
int komutlariTemizle(char ***);

int main(int argc, char *argv[])
{
    char satir[MAXSATIR];
    char ***komutlar;
    if (argc > 2)
    {
        fprintf(stderr, "maksimum 1 arguman girebilirsiniz\n");
        exit(-1);
    }
    if (argc == 2)
    {
        FILE *fp;
        if ((fp = fopen(argv[1], "r")) == NULL)
        {
            perror("Dosya acilamadi");
            exit(1);
        }
        while (fgets(satir, sizeof satir, fp))
        {
            komutlar = tokenAyir(satir);

            printf("prompt> ");
            for (int i = 0; i < komutSayisi; i++)
            {
                for (int j = 0; j < maxArgumanSayisi; j++)
                {
                    if(komutlar[i][j] == NULL)
                    {
                        break;
                    }
                    printf("%s ",komutlar[i][j]);
                }
                if(i != komutSayisi - 1)
                {
                    printf("; ");
                }
            }
            printf("\n");
            
            if (calistir(komutlar))
            {
                exit(0);
            }
            komutlariTemizle(komutlar);
        }

        fclose(fp);
        return 0;
    }
    else
    {
        while (1)
        {
            memset(satir, 0,MAXSATIR);
            printf("prompt> ");
            if (fgets(satir, sizeof satir, stdin))
            {
                komutlar = tokenAyir(satir);
                if (calistir(komutlar))
                {
                    exit(0);
                }
                komutlariTemizle(komutlar);
            }
            else
            {
                printf("\n");
                break;
            }
        }
    }

    return 0;
}

char ***tokenAyir(char *satir)
{
    if (satir[MAXSATIR - 2] != '\n' && satir[MAXSATIR - 2] != '\0')
    {
        int c;
        fprintf(stderr, "Cok uzun bir komut satiri (bu proje icin '\\n' dahil 512 karakterden fazla)\n");
        satir[MAXSATIR - 2] = '\0';
        while((c = getchar()) != '\n' && c != EOF ); //flush all stdin
    }
    komutSayisi = 0;
    maxArgumanSayisi = 0;

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
    int quitGeldi = 0;
    char *name = "./shell";

    for (int i = 0; i < komutSayisi; i++)
    {
        if (strcmp(komutlar[i][0], "quit") == 0)
        {
            quitGeldi = 1;
            break;
        }
        else if (strcmp(komutlar[i][0], name) == 0)
        {
            for (int s = i; s < komutSayisi - 1; s ++) 
            {
                komutlar[i][0] = komutlar[i + 1][0];
            }
        }
        else if ((pid = fork()) == 0) // child processler olusturuluyor
        {
            int say = 0;
            char *argumanlar[maxArgumanSayisi + 1];
            while (komutlar[i][say] != NULL)
            {
                argumanlar[say] = komutlar[i][say];
                say++;
            }
            argumanlar[say] = NULL;
            if(execvp(argumanlar[0], argumanlar) < 0)
            {
                perror("execvp calismadi");
            }
            exit(0);
        }
    }
    while ((wpid = wait(&status)) > 0); //tum processlerin bitmesini bekle
    return quitGeldi;
}

int komutlariTemizle(char ***komutlar) //heapte tutulan yerleri memory leak olmamasi icin birak
{
    for (int i = 0; i < komutSayisi; i++)
    {
        free(komutlar[i]);
    }
    free(komutlar);
    return 1;
}