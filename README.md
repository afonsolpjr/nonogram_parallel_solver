# Nonogram Parallel Solver
O projeto é dividido entre duas pastas, src e utils, contendo cabeçalhos de classes c++ e suas implementações. Todos os jogos estão salvos em Games.txt e o projeto pode ser compilado ao executar compilar.sh que utiliza o arquivo CMakeLists.txt.  

## src

### main.cpp
Função que importa completamente os jogos e soluciona tanto sequencialmente quanto paralelamente.

### Cell.h
Por conta da simplicidade da classe, o arquivo de header já possui as implementações dos métodos. Essa classe representa cada uma das células do tabuleiro de jogo, podendo estar preenchidas, bloqueadas ou vazias (ainda não preenchidas ou bloqueadas).

Funções para definir o estado da célula, sendo respectivamente marcar, desmarcar e bloquear:
```c++
    void set(); 
    void unset();
    void block();
```
Funções para verificar se a célula está em determinado estado:
```c++
    bool isSet();
    bool isEmpty();
    bool isBlocked();
```
Função para converter o estado atual, em int, para um char representando visualmente o estado:
```c++
    std::string toString(bool zeros = false);
```
Função para obter o estado atual da célula:
```c++
    int getState() const;
```

### Hint.h e Hint.cpp
Construtor padrão da classe Hint. Inicializa um objeto de dica sem valores definidos.  
Construtor da classe Hint que inicializa as dicas de uma linha com base em um vetor de inteiros. Cada inteiro representa o tamanho de um bloco contínuo de células preenchidas, e o vetor deve estar ordenado conforme a sequência das dicas na linha.
```c++
    Hint() = default;
    Hint(const std::vector<int> &blocks);
```
Função para adicionar uma dica correspondente a uma sequência de blocos. O parâmetro size representa o tamanho do bloco contínuo que será adicionado à lista de dicas da linha ou coluna:
```c++
    void addBlock(int size);
```
Função para obter a quantidade de blocos (dicas) em uma linha. Retorna o número total de dicas armazenadas:
```c++
    int getSize() const;
```
Função para obter o tamanho da dica em um índice específico. O parâmetro index indica a posição da dica desejada no vetor de blocos:
```c++
    int getHint(int index) const;
```
Função para converter o objeto Hint em uma representação textual. Retorna uma string contendo os tamanhos dos blocos separados por espaço:
```c++
    std::string toString() const;
```
Função para acessar diretamente o vetor de dicas (tamanhos dos blocos). Retorna uma referência constante ao vetor de inteiros armazenado no objeto:
```c++
    const std::vector<int> &getBlocks() const
```

### Line.h e Line.cpp
Função para criar uma linha com um determinado comprimento. Caso o parâmetro init seja true, as células da linha são inicializadas. Caso contrário, apenas o espaço é alocado para posterior atribuição:
```c++
    Line(int length, bool init = false);
```
Função para adicionar uma célula à linha. Recebe um ponteiro para uma estrutura Cell que será incluída no final da linha:
```c++
    void addCell(Cell *cell);
```
Função para associar uma célula a uma posição específica da linha. Define, no vetor de células, o ponteiro da célula na posição index como sendo o ponteiro fornecido no parâmetro cell:
```c++
    void bindCell(int index, Cell *cell);
```
Função para adicionar uma dica à linha. O parâmetro block_size representa o tamanho do bloco a ser adicionado:
```c++
    void addHint(int block_size);
```
Função para retornar a estrutura Hint associada à linha:
```c++
    const Hint &getHints()
```
Função para obter a quantidade total de dicas da linha:
```c++
    int getHintSize() const
```
Função para obter a dica localizada no índice especificado da linha:
```c++
    int getHint(int index) const;
```
Função para obter o comprimento da linha:
```c++
    int getLength() const
```
Função para definir o estado da célula localizada no índice especificado da linha:
```c++
    void setCell(int index);
```
Função para bloquear a célula localizada no índice especificado da linha:
```c++
    void blockCell(int index);
```
Função para obter a representação da linha como uma string. Caso o parâmetro zeros seja true, células vazias são representadas como zeros:
```c++
    std::string toString(bool zeros = false) const;
```
Função para imprimir a linha no console:
```c++
    void print() const;
```
Função para acessar, por referência, a célula localizada no índice especificado da linha e função para acessar, por referência constante, a célula localizada no índice especificado da linha:
```c++
    Cell &operator[](int index) { return *cells[index]; }
    const Cell &operator[](int index) const { return *cells[index]; }   
```

### Nonogram.h e Nonogram.cpp
Função para criar um objeto Nonogram vazio utilizando o construtor padrão. Função para criar um objeto Nonogram vazio com alocação de linhas e colunas, conforme os parâmetros de largura (width) e altura (height):
```c++
    Nonogram() = default;
    Nonogram(int width, int height);
```
Função para obter uma referência à linha localizada no índice especificado:
```c++
    Line &getRow(int index);
```
Função para obter uma referência à coluna localizada no índice especificado:
```c++
    Line &getColumn(int index);
```
Função para obter a largura (número de colunas) do Nonogram:
```c++
    int getWidth() const 
```
Função para obter a altura (número de linhas) do Nonogram:
```c++
    int getHeight()
```
Função para acessar, por referência, a linha localizada no índice especificado:
```c++
    Line &operator[](int index)
```
Função para acessar, por referência constante, a linha localizada no índice especificado:
```c++
    const Line &operator[](int index) const
```
Função para imprimir a grade do Nonogram no console. Cada linha é impressa em uma nova linha e as células são representadas como '1' (preenchida) ou '0' (vazia):
```c++
    void print() const;
```
Função para redefinir o estado de todas as células do Nonogram:
```c++
    void unsetCells();
```

## utils  
### NonogramPuzzleFactory.h e NonogramPuzzleFactory.cpp
Função para criar um objeto Nonogram a partir de uma string contínua que representa o puzzle. A função divide a string em linhas, valida a entrada, cria o Nonogram com as dimensões apropriadas, interpreta as linhas e colunas, reseta o estado das células e retorna o objeto criado:
```c++
    static Nonogram fromString(const std::string &input);
```
Função para criar um objeto Nonogram a partir de um vetor booleano contínuo que representa o puzzle e o número de colunas. A função divide o vetor em linhas, cria o Nonogram com as dimensões corretas, interpreta as linhas e colunas, reseta o estado das células e retorna o objeto criado:
```c++
    static Nonogram fromBool(const std::vector<bool> &input, int cols);
```
Função para gerar múltiplos puzzles do tipo Nonogram para diferentes dimensões especificadas. Para cada dimensão, a função cria uma quantidade definida de jogos únicos que possuem solução válida. Ela verifica a solução usando um solver, salva os puzzles válidos em arquivo e imprime o progresso no console. Caso um puzzle seja insolvable, ele é descartado e contabilizado:
```c++
    static void create_games(const std::vector<int> dimensions, int num_games, const std::string filename);
    static void loadGames(std::string filename);
```
Função para comparar se dois vetores booleanos são iguais. Retorna true se forem iguais, false caso contrário:
```c++
    static bool equalGrids(const std::vector<bool> &oldGrid, const std::vector<bool> &newGrid);
```
Função para carregar jogos a partir de um arquivo. Recebe o nome do arquivo como parâmetro e retorna um vetor contendo os dados dos puzzles carregados:
```c++
    static std::vector<RawPuzzleData> loadGamesFromFile(const std::string &filename);
```
Função para gerar uma grade booleana diferente de qualquer grade presente no vetor fornecido. Recebe como parâmetros o vetor de grades e a dimensão da grade:
```c++
    std::vector<bool> static getDifferentGrid(const std::vector<std::vector<bool>> &grid, int dimension);
```
Função para escrever uma grade booleana em um arquivo. Recebe a grade, sua dimensão e o nome do arquivo como parâmetros:
```c++
    static void writeGridToFile(const std::vector<bool> &grid, int dimension, const std::string &filename);
```
Função para dividir uma string contínua que representa um jogo em múltiplas linhas. Retorna um vetor de strings, cada uma correspondendo a uma linha do puzzle:
```c++
    static std::vector<std::string> splitLines(const std::string &str);
```
Função para dividir um vetor booleano contínuo, que representa um puzzle, em múltiplas linhas. Recebe o vetor de entrada e o comprimento das linhas, retornando um vetor contendo as linhas como vetores booleanos:
```c++
    static std::vector<std::vector<bool>> splitLines(const std::vector<bool> &input, int row_length);
```
Função para verificar se as linhas de uma string são válidas, ou seja, se contêm apenas '0' e '1' e se todas as linhas têm o mesmo tamanho. Recebe um vetor de linhas como parâmetro:
```c++
    static bool isValidLines(const std::vector<std::string> &lines);
```
Função para criar uma linha a partir de uma string que representa os dados da linha:
```c++
    static void parseLine(Line &line, std::string &linestr);
```
Função para criar uma linha a partir de um vetor booleano que representa os dados da linha:
```c++
    static void parseLine(Line &line, std::vector<bool> boolLine);
```
Função para criar uma linha a partir do estado atual do jogo, usada especialmente para construir as colunas:
```c++
    static void parseLine(Line &line);
```
### NonogramRun.h e NonogramRun.cpp
Função construtora para inicializar um objeto NonogramRun. Recebe os dados brutos do puzzle, um indicador para execução paralela e o número de threads. Cria internamente um objeto Nonogram a partir dos dados recebidos:
```c++
    NonogramRun(const RawPuzzleData gameData, bool parallel = false, int nThreads = 0);
```
Função para executar a resolução do puzzle Nonogram. Dependendo da configuração, utiliza um solver paralelo ou sequencial. Mede o tempo gasto na inicialização e na resolução, verifica a correção da solução e imprime estatísticas do processo:
```c++
    void run();
```
Função para imprimir estatísticas da execução do puzzle. Exibe no terminal o tamanho do jogo, o tempo de inicialização, o tempo de resolução e o tempo total da execução. Informa também se a solução foi considerada correta, com base na verificação feita anteriormente:
```c++
    void printStats() const;
```
Função para verificar a correção da solução atual do puzzle em relação aos dados originais. Converte o puzzle resolvido em um vetor de booleanos e o compara com o vetor original armazenado em gameData. Se forem equivalentes, define is_correct como verdadeiro e o retorna:
```c++
    bool NonogramRun::verifyCorrectness()
```
Função estática para verificar a correção de um puzzle resolvido em relação aos dados originais fornecidos como parâmetro. Compara o puzzle convertido em vetor com o vetor original do RawPuzzleData. Retorna true se forem iguais e false caso contrário:
```c++
    static bool verifyCorrectness(const RawPuzzleData gameData, const Nonogram &puzzle);
```
Função para converter um objeto Nonogram em um vetor linear de booleanos. A conversão percorre o puzzle linha por linha, armazenando true para cada célula marcada como "set". Esse vetor representa a solução do puzzle de forma contínua, sendo útil para comparações:
```c++
    static std::vector<bool> puzzleToVector(const Nonogram &puzzle);
```

### RandomGenerator.h e RandomGenerator.cpp
Função para gerar um objeto Nonogram a partir de uma string aleatória. Cria uma string representando uma grade de rows x cols com caracteres '0' e '1', convertendo-a para um Nonogram por meio da NonogramPuzzleFactory:
```c++
    static Nonogram randomFromString(int rows, int cols);
```
Função para gerar um objeto Nonogram a partir de um vetor de booleanos aleatórios. Gera um vetor de tamanho rows * cols com valores true ou false, com 60% de chance de ser true, e o converte para um Nonogram utilizando a NonogramPuzzleFactory:
```c++
    static Nonogram randomFromBool(int rows, int cols);
```
Função para imprimir no terminal uma representação em string de uma grade aleatória de rows x cols, composta por caracteres '0' e '1', com quebras de linha ao final de cada linha da grade:
```c++
    static void printRandomString(int rows, int cols);
```
Função auxiliar para gerar uma string representando uma grade de rows x cols com valores binários aleatórios ('0' ou '1'). Cada linha é separada por uma quebra de linha ('\n'). A distribuição dos bits é uniforme:
```c++
    static std::string generateRandomStringGrid(int rows, int cols);
```
Função auxiliar para gerar um vetor de booleanos aleatórios representando uma grade de rows x cols. Cada valor tem 60% de chance de ser true e 40% de ser false, criando uma densidade maior de células preenchidas (true):
```c++
    static std::vector<bool> generateRandomBoolGrid(int rows, int cols);
```
## solvers
### BaseSolver.h
Função construtora para inicializar um objeto BaseSolver com referência a um objeto Nonogram. Inicializa solucionadores de linha e coluna (rowSolvers e columnSolvers) usando o tipo genérico LineSolverType.
Cada linha e cada coluna do Nonogram é passada como argumento para um novo objeto LineSolverType, e os ponteiros são armazenados nos vetores de solucionadores:
```c++
    template <typename LineSolverType>
    BaseSolver<LineSolverType>::BaseSolver(Nonogram &nonogram_ref)
```
Função para verificar se o Nonogram foi completamente resolvido:
```c++
    template <typename LineSolverType>
    bool BaseSolver<LineSolverType>::isSolved()
```

### NonogramSolver.h e NonogramSolver.cpp
Função construtora para inicializar um objeto BaseSolver com um Nonogram de referência. Cria vetores de ponteiros para solucionadores de linha e coluna (rowSolvers e columnSolvers), instanciando objetos do tipo LineSolverType para cada linha e coluna do puzzle, com base nas pistas obtidas diretamente do objeto Nonogram:
```c++
    NonogramSolver(Nonogram &nonogramref): BaseSolver<SequentialLineSolver>(nonogramref)
```

```c++
    template <typename LineSolverType>
    bool BaseSolver<LineSolverType>::isSolved()
```
Função para resolver o puzzle Nonogram por meio de iterações sucessivas entre solucionadores de linha e de coluna. Enquanto o puzzle não estiver totalmente resolvido (!isSolved()), cada solver gera e aplica atualizações de padrões comuns, propagando alterações entre linhas e colunas. Se, em uma passada completa, nenhuma mudança for feita, conclui que não é possível avançar e retorna false; caso contrário, retorna true quando todas as linhas estiverem solucionadas:
```c++
    bool solve()
```
Função para inicializar o solver gerando todas as combinações possíveis de preenchimento para cada linha e coluna, preparando os solucionadores internos (rowSolvers e columnSolvers) antes de iniciar o processo de resolução:
```c++
    void init();
```

### ParallelNonogramSolver.h e ParallelNonogramSolver.cpp
Função construtora para inicializar um ParallelNonogramSolver. Encadeia o construtor de BaseSolver com o Nonogram de referência e armazena a quantidade de threads que serão utilizadas na solução paralela:
```c++
    ParallelNonogramSolver(Nonogram &nonogram_ref, int nThreads);
```
Função para resolver o puzzle Nonogram em paralelo: cria um pool com nThreads threads, delega a cada uma a execução do método worker, aguarda a conclusão de todas elas com join() e, ao final, devolve o resultado de isSolved() indicando se o puzzle foi inteiramente resolvido:
```c++
    bool solve();
```
Função para executar, em uma thread, o ciclo principal de resolução paralela: alterna fases de processamento de linhas e colunas chamando processPhase, sincroniza-se com as demais threads por meio de barreiras (completionCheckBarrier) e encerra quando o puzzle estiver completamente resolvido:
```c++
    void worker(int id);
```
Função para preparar a execução paralela do solver. Cria os jobs iniciais para todas as linhas e colunas, instancia o pool de threads com nThreads, delega a cada thread a execução de init_worker() e aguarda a conclusão de todas as threads com join():
```c++
    void init();
```
Função worker executada por cada thread do pool. Processa, em paralelo, os jobs de geração de possibilidades primeiro para as linhas e depois para as colunas, e por fim sincroniza todas as threads na barreira de inicialização (init_barrier()):
```c++
    void init_worker();
```
Função para inserir um trabalho em uma fila de jobs. Recebe um objeto UpdateJob e um identificador booleano isRow. Se isRow for true, insere o trabalho na fila de linhas (rowJobs); caso contrário, insere na fila de colunas (columnJobs):
```c++
    void insertUpdateAndJob(UpdateJob update, bool isRow);
```
Função para obter um trabalho de uma fila de jobs. O parâmetro booleano isRow indica se o job requerido é de linha (true) ou de coluna (false). Retorna o índice da linha ou coluna a ser processada, ou um valor negativo se não houver jobs disponíveis:
```c++
    int getJob(bool isRow);
```
Função para realizar uma barreira de sincronização entre threads. Garante que todas as threads tenham concluído a etapa atual antes de prosseguir. Retorna true se o puzzle estiver completamente resolvido; caso contrário, retorna false:
```c++
    bool completionCheckBarrier();
```
Função para sincronizar a inicialização das threads. A última thread a alcançar essa barreira gera jobs para todas as linhas e colunas, assegurando que cada linha e coluna tenha, ao menos uma vez, suas possibilidades verificadas:
```c++
    void init_barrier();
```
Função para gerar possibilidades para todas as linhas ou colunas: enquanto existirem jobs na fila correspondente (rowJobs ou columnJobs), obtém o índice da linha/coluna via getJob(isRowInit) e chama generatePossibilities() no solver respectivo para esse índice:
```c++
    void init_work(bool isRowInit);
```
Função para processar uma fase da resolução (linha ou coluna): repetidamente obtém jobs da fila, executa phaseWork para cada índice, acumulando atualizações em phaseUpdates. Ao fim, registra todas as atualizações coletadas via registerPhaseUpdates:
```c++
    void processPhase(std::stack<UpdateJob> &phaseUpdates, bool isRowPhase);
```
Função para processar o trabalho em uma linha ou coluna específica: chama updatePossibilities() para atualizar o solver, obtém atualizações parciais com resolveCommonPatterns() e adiciona todas essas atualizações na pilha geral phaseUpdates, ajustando o índice para a linha/coluna atual:
```c++
    void phaseWork(int index, std::stack<UpdateJob> &phaseUpdates, bool isRowPhase);
```
Função para registrar atualizações da fase atual: enquanto houver atualizações em phaseUpdates, remove cada uma e chama insertUpdateAndJob para inseri-las na estrutura de jobs, protegendo a seção crítica para atualização concorrente dos dados:
```c++
    void registerPhaseUpdates(std::stack<UpdateJob> &phaseUpdates, bool isRowPhase);
```

## linvesolvers
### BaseLineSolver.h e BaseLineSolver.cpp
```c++
    template <typename UpdateType>
    void updatePossibilities();
```

```c++
    template <typename UpdateType>
    virtual void insertUpdate(UpdateType update) = 0;
```

```c++
    template <typename UpdateType>
    std::stack<UpdateType> resolveCommonPatterns();
```

```c++
    template <typename UpdateType>
    void print_possibility(const std::vector<bool> &possibility);
```

```c++
    template <typename UpdateType>
    void print_possibilities();
```

```c++
    template <typename UpdateType>
    bool isSolved();
```

```c++
    template <typename UpdateType>
    void generatePossibilities();
```

```c++
    template <typename UpdateType>
    std::vector<std::vector<int>> generateCombinations(std::vector<int> n, int k);
```

```c++
    template <typename UpdateType>
    std::vector<bool> composeBlockLine(int length, int block_size, int start);
```

```c++
    template <typename UpdateType>
    void eliminatePossibilities(int index, bool status);
```

```c++
    template <typename UpdateType>
    void play(int index, bool cell_value);
```

```c++
    template <typename UpdateType>
    std::list<int> getCommonIndexes();
```

```c++
    template <typename UpdateType>
    void combinationToPossibility(std::vector<int> combination);
```

### ParallelLineSolver.h e ParallelLineSolver.cpp
```c++
    ParallelLineSolver(Line &lineRef);
```

```c++
    std::stack<UpdateJob> resolveCommonPatterns();
```

```c++
    void insertUpdate(UpdateJob updateJob);
```

```c++
    void init();
```
### SequentialLineSolver.h e SequentialLineSolver.cpp

```c++
    SequentialLineSolver(Line &line_ref);
```

```c++
    std::stack<Update> resolveCommonPatterns();
```

```c++
    void insertUpdate(Update update);
```