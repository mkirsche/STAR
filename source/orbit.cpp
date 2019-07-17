#include <sstream>
#include <string>

#include "Genome.h"
#include "Parameters.h"
#include "ReadAlign.h"
#include "Transcriptome.h"
#include "Variation.h"
#include "orbit.h"

// in orbit.h
extern "C" {
    struct Aligner;
    char* align_read(...);
    Aligner* init_aligner(...);
    void destroy_aligner(Aligner*);
}


struct Aligner
{
    Parameters *p;
    Genome *g;
    ReadAlign *ra;
   
    Aligner(int, char* argIn[]) {
        
    }
    
    ~Aligner() {
        delete ra;
        delete g;
        delete p;
    }
};

string align_read(Aligner* a, char *Read1, char *Qual1, unsigned long long read_length)
{
    a.p->readNmates = 1;
    a.ra->readNmates = 1;
    a.ra->Read0 = &Read1;
    a.ra->Qual0 = &Qual1;
    a.ra->Lread = read_length;
    a.ra->readLength[0] = read_length;
    a.ra->readLength[1] = read_length;
    int readStatus = a.ra->oneRead();
    if(readStatus != 0)
    {
        return "";
    }
    string str = a.ra->outputAlignments();
    char* ret = (char*) malloc((str.length() + 1));
    strncpy(
    return ret;
}

Aligner* init_aligner(int argInN, char* argIn[])
{
    return new Aligner(argc, argv);
    auto *P = new Parameters();
    P->inputParameters(argInN, argIn);

    Genome *mainGenome = new Genome(*P);
    mainGenome->genomeLoad();

    Transcriptome *mainTranscriptome = nullptr;

    mainGenome->Var = new Variation(*P, mainGenome->chrStart, mainGenome->chrNameIndex);

    ReadAlign *RA = new ReadAlign(*P, *mainGenome, mainTranscriptome, 0);

    Aligner res = { RA, P };
    return res;
}
        
void destroy_aligner(Aligner* aln) { delete aln; }
        

int main()
{
    char* arr[] = {
            "STAR", "--genomeDir", "/mnt/opt/refdata_cellranger/GRCh38-3.0.0/star",
            "--outSAMmultNmax", "50",
            "--runThreadN", "1",
            "--readNameSeparator", "space",
            "--outSAMunmapped", "Within", "KeepPairs",
            "--outSAMtype", "SAM",
            "--outStd", "SAM",
            "--outSAMorder", "PairedKeepInputOrder",
    };
    int len = sizeof(arr) / sizeof(arr[0]);
    Aligner *a = init(len, arr);

    std::string line;
    std::ifstream infile("1.fastq");
    int lineNum = 0;
    char** curRead = (char**)malloc(sizeof(char*));
    curRead[0] = (char*)malloc(500*sizeof(char));
    while(std::getline(infile, line))
    {
        //printf("line %s\n", line.c_str());
        //printf("lineNum %d\n", lineNum);
        if(lineNum%4 == 1)
        {
            strcpy(curRead[0], line.c_str());
        }
        else if(lineNum%4 == 3)
        {
            char** curQual = (char**)malloc(sizeof(char*));
            curQual[0] = (char*)malloc(500*sizeof(char));
            strcpy(curQual[0], line.c_str());
            printf("read = %s\n", curRead[0]);
            printf("qual = %s\n", curQual[0]);
            string bam_line = align_read(a, curRead, curQual, line.length());
            printf("%s\n", bam_line.c_str());
            free(curQual[0]);
            free(curQual);
        }
        lineNum++;
        if(lineNum == 100) break;
    }
    free(curRead[0]);
    free(curRead);
    delete a;
    return 0;
}
