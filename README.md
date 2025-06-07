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
```c++
    Nonogram(int width, int height);
```

```c++
    Line &getRow(int index);
```

```c++
    Line &getColumn(int index);
```

```c++
    int getWidth() const 
```

```c++
    int getHeight()
```

```c++
    Line &operator[](int index)
```

```c++
    const Line &operator[](int index) const
```

```c++
    void print() const;
```

```c++
    void unsetCells();
```

## utils  
### NonogramPuzzleFactory.h e NonogramPuzzleFactory.cpp
```c++
    static Nonogram fromString(const std::string &input);
```

```c++
    static Nonogram fromBool(const std::vector<bool> &input, int cols);
```

```c++
    static void main(std::string task);
```

```c++
    static void create_games(const std::vector<int> dimensions, int num_games, const std::string filename);
    static void loadGames(std::string filename);
```

```c++
    static bool equalGrids(const std::vector<bool> &oldGrid, const std::vector<bool> &newGrid);
```

```c++
    static std::vector<RawPuzzleData> loadGamesFromFile(const std::string &filename);
```

```c++
    std::vector<bool> static getDifferentGrid(const std::vector<std::vector<bool>> &grid, int dimension);
```

```c++
    static void writeGridToFile(const std::vector<bool> &grid, int dimension, const std::string &filename);
```

```c++
    static std::vector<std::string> splitLines(const std::string &str);
```

```c++
    static std::vector<std::vector<bool>> splitLines(const std::vector<bool> &input, int row_length);
```
```c++
    static bool isValidLines(const std::vector<std::string> &lines);
```

```c++
    static void parseLine(Line &line, std::string &linestr);
    static void parseLine(Line &line, std::vector<bool> boolLine);
    static void parseLine(Line &line);
```

### NonogramRun.h e NonogramRun.cpp
```c++
    NonogramRun(const RawPuzzleData gameData, bool parallel = false, int nThreads = 0);
```

```c++
    void run();
```

```c++
    void printStats() const;
```

```c++
    static bool verifyCorrectness(const RawPuzzleData gameData, const Nonogram &puzzle);
```

```c++
    static std::vector<bool> puzzleToVector(const Nonogram &puzzle);
```

```c++
    bool verifyCorrectness();
```
### RandomGenerator.h e RandomGenerator.cpp

```c++
    static Nonogram randomFromString(int rows, int cols);
```

```c++
    static Nonogram randomFromBool(int rows, int cols);
```

```c++
    static void printRandomString(int rows, int cols);
```

```c++
    static std::string generateRandomStringGrid(int rows, int cols);
```

```c++
    static std::vector<bool> generateRandomBoolGrid(int rows, int cols);
```
## solvers
### BaseSolver.h
```c++
    template <typename LineSolverType>
    bool BaseSolver<LineSolverType>::isSolved()
```

```c++
    template <typename LineSolverType>
    BaseSolver<LineSolverType>::BaseSolver(Nonogram &nonogram_ref)
```

### NonogramSolver.h e NonogramSolver.cpp
```c++
    NonogramSolver(Nonogram &nonogramref): aseSolver<SequentialLineSolver>(nonogramref)
```

```c++
    bool solve()
```

```c++
    void init();
```

### ParallelNonogramSolver.h e ParallelNonogramSolver.cpp
```c++
    ParallelNonogramSolver(Nonogram &nonogram_ref, int nThreads);
```

```c++
    bool solve();
```

```c++
    void worker(int id);
```

```c++
    void init();
```

```c++
    void init_worker();
```

```c++
    void insertUpdateAndJob(UpdateJob update, bool isRow);
```

```c++
    int getJob(bool isRow);
```

```c++
    bool completionCheckBarrier();
```

```c++
    void init_barrier();
```

```c++
    void init_work(bool isRowInit);
```

```c++
    void processPhase(std::stack<UpdateJob> &phaseUpdates, bool isRowPhase);
```

```c++
    void phaseWork(int index, std::stack<UpdateJob> &phaseUpdates, bool isRowPhase);
```

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