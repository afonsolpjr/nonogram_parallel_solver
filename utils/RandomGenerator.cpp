#include <iostream>
#include <string>
#include "RandomGenerator.h"

// #define DEBUG 1

#ifdef DEBUG
#define DBG(x) std::cout << x << std::endl
#else
#define DBG(x)
#endif

Nonogram RandomGenerator::randomFromString(int rows, int cols)
{
    std::string grid = generateRandomStringGrid(rows, cols);
    Nonogram nonogram = fromString(grid);
    return nonogram;
}

Nonogram RandomGenerator::randomFromBool(int rows, int cols)
{
    std::vector<bool> grid = generateRandomBoolGrid(rows, cols);
    Nonogram nonogram = fromBool(grid, cols);
    return nonogram;
}

Nonogram RandomGenerator::fromString(const std::string &input)
{
    std::vector<std::string> lines = splitLines(input);

    DBG("Testando string: \n"
        << input);

    if (!isValidLines(lines))
    {
        std::cerr << "Invalid grid string" << std::endl;
        exit(1);
    }

    Nonogram newNonogram(lines[0].size(), lines.size());

    // parse lines
    for (size_t i = 0; i < lines.size(); ++i)
    {
        parseLine(newNonogram[i], lines[i]);
        DBG("Dicas da linha " << i << ": " << newNonogram[i].getHints().toString());
    }

    // parse collumns
    for (size_t i = 0; i < lines[0].size(); ++i)
    {
        parseLine(newNonogram.getColumn(i));
        DBG("Dicas da coluna " << i << ": " << newNonogram.getColumn(i).getHints().toString());
    }

    newNonogram.unsetCells();
    return newNonogram;
}

Nonogram RandomGenerator::fromBool(const std::vector<bool> &input, int cols)
{
    std::vector<std::vector<bool>> lines = splitLines(input, cols);

    Nonogram newNonogram(lines[0].size(), lines.size());

    // parse lines
    for (size_t i = 0; i < lines.size(); ++i)
    {
        parseLine(newNonogram[i], lines[i]);
        DBG("Dicas da linha " << i << ": " << newNonogram[i].getHints().toString());
    }

    // parse collumns
    for (size_t i = 0; i < lines[0].size(); ++i)
    {
        parseLine(newNonogram.getColumn(i));
        DBG("Dicas da coluna " << i << ": " << newNonogram.getColumn(i).getHints().toString());
    }

    newNonogram.unsetCells();
    return newNonogram;
}

void RandomGenerator::printRandomString(int rows, int cols)
{
    std::string grid = generateRandomStringGrid(rows, cols);
    std::cout << grid;
}

// Função auxiliar para gerar uma string de grid aleatória com 0s e 1s
std::string RandomGenerator::generateRandomStringGrid(int rows, int cols)
{
    std::string grid;

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            grid += (rand() % 2) ? '1' : '0';
        }
        grid += '\n';
    }
    return grid;
}

std::vector<bool> RandomGenerator::generateRandomBoolGrid(int rows, int cols)
{
    std::vector<bool> grid;

    grid.reserve(rows * cols);
    for (int i = 0; i < rows * cols; ++i)
    {
        grid.push_back((rand() % 10 < 6) ? true : false);
    }
    return grid;
}

// Função auxiliar para dividir uma string em suas linhas
std::vector<std::string> RandomGenerator::splitLines(const std::string &str)
{
    std::vector<std::string> lines;
    std::string line;
    for (char ch : str)
    {
        if (ch == '\n')
        {
            lines.push_back(line);
            line.clear();
        }
        else
        {
            line += ch;
        }
    }
    if (!line.empty())
    {
        lines.push_back(line);
    }
    return lines;
}

std::vector<std::vector<bool>> RandomGenerator::splitLines(const std::vector<bool> &input, int row_length)
{
    std::vector<std::vector<bool>> lines;
    std::vector<bool> line;
    line.reserve(row_length);
    for (size_t i = 0; i < input.size(); i++)
    {
        line.push_back(input[i]);
        if (line.size() == row_length)
        {
            lines.push_back(line);
            line.clear();
        }
    }

    return lines;
}

// funcao para verificar se as linhas da string sao validas (contem apenas 0 e 1s e linhas do mesmo tamanho), recebe vetor de linhas
bool RandomGenerator::isValidLines(const std::vector<std::string> &lines)
{
    if (lines.empty())
        return false;

    size_t length = lines[0].size();
    for (const auto &line : lines)
    {
        if (line.size() != length)
            return false;
        for (char ch : line)
        {
            if (ch != '0' && ch != '1')
                return false;
        }
    }
    return true;
}

// Parser da string de uma linha somente
void RandomGenerator::parseLine(Line &line, std::string &linestr)
{
    int sequence = 0;

    for (size_t i = 0; i < line.getLength(); i++)
    {
        if (linestr[i] == '1')
        {
            line[i].set();
            sequence++;
        }
        else
        {
            if (sequence > 0)
            {
                line.addHint(sequence);
                sequence = 0;
            }
        }
    }
    if (sequence > 0)
        line.addHint(sequence);
    if (!line.getHintSize())
        line.addHint(0);
}

void RandomGenerator::parseLine(Line &line, std::vector<bool> boolLine)
{
    int sequence = 0;

    for (size_t i = 0; i < line.getLength(); i++)
    {
        if (boolLine[i])
        {
            line[i].set();
            sequence++;
        }
        else
        {
            if (sequence > 0)
            {
                line.addHint(sequence);
                sequence = 0;
            }
        }
    }
    if (sequence > 0)
        line.addHint(sequence);
    if (!line.getHintSize())
        line.addHint(0);
}

// Parser de uma Line já inicializada
void RandomGenerator::parseLine(Line &line)
{
    // gerar dicas
    int sequence = 0;
    for (int i = 0; i < line.getLength(); ++i)
    {
        if (line[i].isSet())
            sequence++;
        else
        {
            if (sequence > 0)
            {
                line.addHint(sequence);
                sequence = 0;
            }
        }
    }
    if (sequence > 0)
        line.addHint(sequence);
    if (!line.getHintSize())
        line.addHint(0);
}
