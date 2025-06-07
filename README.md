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
```c++
    void addBlock(int size);
```

```c++
    int getSize() const;
```

```c++
    int getHint(int index) const;
```

```c++
    std::string toString() const;
```

```c++
    const std::vector<int> &getBlocks() const
```

### Line.h e Line.cpp
```c++
    Line(int length, bool init = false);
```

```c++
    void addCell(Cell *cell);
```

```c++
    void bindCell(int index, Cell *cell);
```

```c++
    void addHint(int block_size);
```

```c++
    const Hint &getHints()
```

```c++
    int getHintSize() const
```

```c++
    int getHint(int index) const;
```

```c++
    int getLength() const
```

```c++
    void setCell(int index);
```

```c++
    void blockCell(int index);
```

```c++
    std::string toString(bool zeros = false) const;
```

```c++
    void print() const;
```

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