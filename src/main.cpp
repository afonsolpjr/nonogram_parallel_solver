#include <iostream>
#include "../utils/NonogramRun.h"
#include <chrono>

void batchExecution(int timesToRun, int maxThreads, std::string gamesFilename, std::string resultFilename)
{
    std::vector<RawPuzzleData> gamesData = NonogramPuzzleFactory::loadGamesFromFile("Games.txt");

    FILE *fp;
    fp = fopen(resultFilename.c_str(), "w");

    // escreve nome das colunas
    //  game number : dimension : nThreads : initTime : resolutionTime
    fputs("\"gameId\",\"dimension\",\"nThreads\",\"initTime\",\"resolutionTime\"\n", fp);

    printf("\n%ld Jogos a serem executados em até %d threads, %d vezes.",
           gamesData.size(), maxThreads, timesToRun);

    for (int nThreads = 1; nThreads <= maxThreads; nThreads++)
    {

        for (int i = 0; i < timesToRun; i++)
        {
            for (int gameId = 0; gameId < gamesData.size(); gameId++)
            {
                printf("\r %d Execução. | Threads = %d | Id do jogo = %d | dimensão = %d",
                       i, nThreads, gameId, gamesData[gameId].dimension);
                fflush(stdout);
                NonogramRun execution(gamesData[gameId], nThreads);
                if (execution.run())
                    execution.appendResult(fp);
                else
                {
                    printf("\n solucao errada do jogo %d.", gameId);
                    exit(1);
                }
            }
        }
    }
    fclose(fp);
}

int main(int argc, char* argv[])
{
    std::cout << "Hello, Nonogram Solver!" << std::endl;
    
    if(argc <2)
    {
        printf("\n Insira o número maximo de threads a serem testadas como argumento do programa\n");
        exit(1);
    }
    
    batchExecution(10, atoi(argv[1]), "Games.txt", "data.csv");
    return 0;
}
