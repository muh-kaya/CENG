#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GAP -2
#define MATCH 4
#define MISMATCH -3

int max(int A, int B, int C, int D)  //Retrieved from http://user.ceng.metu.edu.tr/~tcan/ceng465_f1718/Schedule/align.c
{
    if (A>=B && A>=C && A>=D) return A;
    else if (B>=A && B>=C && B>=D) return B;
    else if (C>=A && C>=B && C>=D) return C;
    else return D;
}

char Tmax(int A, int B, int C)  //Retrieved from http://user.ceng.metu.edu.tr/~tcan/ceng465_f1718/Schedule/align.c
{
    if (A>B && A>C) return 'D';
    else if (B>A && B>C) return 'L';
    else return 'U';
}

int m(char p, char q)  //Retrieved from http://user.ceng.metu.edu.tr/~tcan/ceng465_f1718/Schedule/align.c
{
    if (p==q) return MATCH;
    else return MISMATCH;
}

void append(char *st,int L,char c)  //Retrieved from http://user.ceng.metu.edu.tr/~tcan/ceng465_f1718/Schedule/align.c
{
     int i;
     for (i=L;i>0;i--)
         st[i]=st[i-1];
     st[L+1] = '\0';
     st[0] = c;
}

int triplet_counts(char a, char b, char c){

    int p = 0;

    if(a == 'T')
        p = p + 16*1;

    else if(a == 'G')
        p = p + 16*2;

    else if(a == 'C')
        p = p + 16*3;


    if(b == 'T')
        p = p + 4*1;

    else if(b == 'G')
        p = p + 4*2;

    else if(b == 'C')
        p = p + 4*3;


    if(c == 'T')
        p = p + 1;

    else if(c == 'G')
        p = p + 2;

    else if(c == 'C')
        p = p + 3;


    return p;
}

void align(char best1[], char best2[])  //Some parts retrieved from http://user.ceng.metu.edu.tr/~tcan/ceng465_f1718/Schedule/align.c
{
    int best = 0;
    int M, N, i, j, a, b, L;

    char *RA;
    char *RM;
    char *RB;

    N = strlen(best1);
    M = strlen(best2);

    int S[N+1][N+1];
    int T[N+1][N+1];

    RA = (char *)malloc(sizeof(char)*N);
    RM = (char *)malloc(sizeof(char)*N);
    RB = (char *)malloc(sizeof(char)*N);

    S[0][0] = 0;
    T[0][0] = 'D';

    for (i=0;i<=N;i++)
    {
        S[i][0] = 0;
        T[i][0] = 'U';
    }
    for (i=0;i<=M;i++)
    {
        S[0][i] = 0;
        T[0][i] = 'L';
    }

    for (i=1;i<=N;i++){
        for (j=1;j<=M;j++)
        {
            S[i][j] = max(0, S[i-1][j-1]+m(best1[i-1],best2[j-1]),S[i][j-1]+GAP,S[i-1][j]+GAP);
            T[i][j] = Tmax(S[i-1][j-1]+m(best1[i-1],best2[j-1]),S[i][j-1]+GAP,S[i-1][j]+GAP);
            if(S[i][j] >= best){
                best = S[i][j];
                a = i;
                b = j;
            }
        }
    }

    printf("The score of the alignment is : %d\n", best);

    i=a;
    j=b;
    L=0;
    RA[0]='\0';
    RB[0]='\0';
    RM[0]='\0';

    while (i!=0 && j!=0)
    {
          if (T[i][j]=='D')
          {
             append(RA,L,best1[i-1]);
             append(RB,L,best2[j-1]);
             if (best1[i-1]==best2[j-1]) append(RM,L,'|');
             else append(RM,L,'*');
             i--; j--;
          }
          else if (T[i][j]=='L')
          {
             append(RA,L,'-');
             append(RB,L,best2[j-1]);
             append(RM,L,' ');
             j--;
          }
          else if (T[i][j]=='U')
          {
             append(RA,L,best1[i-1]);
             append(RB,L,'-');
             append(RM,L,' ');
             i--;
          }

          L++;

    }

    printf("%s\n",RA);
    printf("%s\n",RM);
    printf("%s\n",RB);

}

int main(){

    char *RS = (char *)malloc(sizeof(char)*10000001);
    char *G1 = (char *)malloc(sizeof(char)*100000001);
    char *G2 = (char *)malloc(sizeof(char)*100000001);

    int VRS[64];
    int i, j, k;
    int K, M, N;

    int distance = 0;
    int distance2 = 0;
    int distance3 = 0;
    int distance4 = 0;

    char *subsequence1;
    char *subsequence2;

    int VG1[64];
    int VG2[64];

    char *best1;
    char *best2;

    FILE *fp;
    FILE *fp1;
    FILE *fp2;

    //INPUTS

    fp = fopen("RS.txt", "r+");
    fscanf(fp,"%s",RS);

    fp1 = fopen("G1.txt", "r+");
    fscanf(fp1,"%s",G1);

    fp2 = fopen("G2.txt", "r+");
    fscanf(fp2,"%s",G2);


    K = strlen(RS);

    M = strlen(G1);

    N = strlen(G2);


    subsequence1 = (char *)malloc(sizeof(char)*K);
    subsequence2 = (char *)malloc(sizeof(char)*K);

    best1 = (char *)malloc(sizeof(char)*K);
    best2 = (char *)malloc(sizeof(char)*K);

    RS = realloc(RS, sizeof(char)*K);
    G1 = realloc(G1, sizeof(char)*M);
    G2 = realloc(G2, sizeof(char)*N);


    //VRS

    for(i=0; i<64; i++)
        VRS[i] = 0;

    for(i=0; i<(K-2); i++){
        VRS[triplet_counts(RS[i], RS[i+1], RS[i+2])]++ ;
    }


    //FOR G1

    strncpy(subsequence1, G1, K);
    subsequence1[K] = '\0';


    for(i=0; i<64; i++)
        VG1[i] = 0;

    for(i=0; i<(K-2); i++){
        VG1[triplet_counts(subsequence1[i], subsequence1[i+1], subsequence1[i+2])]++ ;
    }


    for(i=0; i<64; i++){
        distance += abs(VRS[i] - VG1[i]);
    }


    for(i=0; i<K; i++){
        best1[i] = subsequence1[i];
    }
    best1[K] = '\0';


    for(i=0; i<(M-K); i++){

        VG1[triplet_counts(subsequence1[0], subsequence1[1], subsequence1[2])]--;

        for(j=1; j<K; j++){
            subsequence1[j-1] = subsequence1[j];
        }
        subsequence1[K-1] = G1[K+i];
        subsequence1[K] = '\0';

        VG1[triplet_counts(subsequence1[K-3], subsequence1[K-2], subsequence1[K-1])]++;


        for(k=0; k<64; k++){
            distance2 += abs(VRS[k] - VG1[k]);
        }


        if(distance2 < distance){
            distance = distance2;
            for(k=0; k<K; k++){
                best1[k] = subsequence1[k];
            }
            best1[K] = '\0';
        }
        distance2 = 0;
    }



    //FOR G2

    strncpy(subsequence2, G2, K);
    subsequence2[K] = '\0';


    for(i=0; i<64; i++)
        VG2[i] = 0;

    for(i=0; i<(K-2); i++){
        VG2[triplet_counts(subsequence2[i], subsequence2[i+1], subsequence2[i+2])]++ ;
    }


    for(i=0; i<64; i++){
        distance3 += abs(VRS[i] - VG2[i]);
    }

    for(i=0; i<K; i++){
        best2[i] = subsequence2[i];
    }
    best2[K] = '\0';


    for(i=0; i<(N-K); i++){

        VG2[triplet_counts(subsequence2[0], subsequence2[1], subsequence2[2])]--;

        for(j=1; j<K; j++){
            subsequence2[j-1] = subsequence2[j];
        }
        subsequence2[K-1] = G2[K+i];
        subsequence2[K] = '\0';

        VG2[triplet_counts(subsequence2[K-3], subsequence2[K-2], subsequence2[K-1])]++;


        for(k=0; k<64; k++){
            distance4 += abs(VRS[k] - VG2[k]);
        }


        if(distance4 < distance3){
            distance3 = distance4;
            for(k=0; k<K; k++){
                best2[k] = subsequence2[k];
            }
            best2[K] = '\0';
        }
        distance4 = 0;
    }

    free(subsequence1);
    free(subsequence1);
    free(RS);
    free(G1);
    free(G2);

    align(best1, best2);

    free(best1);
    free(best2);

    return 0;
}
