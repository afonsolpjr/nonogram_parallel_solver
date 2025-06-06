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


### NonogramRun.h e NonogramRun.cpp


### RandomGenerator.h e RandomGenerator.cpp

