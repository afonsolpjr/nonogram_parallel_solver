#include "NonogramPuzzleFactory.h"
#include "NonogramRun.h"
#include <fstream>
#include <sstream>

std::vector<RawPuzzleData> NonogramPuzzleFactory::loadGamesFromFile(const std::string &filename)
{
    std::vector<RawPuzzleData> games;
    std::ifstream in(filename);
    std::string line;
    int gameCounter = 0;
    while (std::getline(in, line))
    {
        std::istringstream iss(line);
        int dimension;
        std::string gridstr;

        if (!(iss >> dimension >> gridstr))
            continue;

        std::vector<bool> grid;
        for (char c : gridstr)
            grid.push_back(c == '1');
        games.push_back({dimension, ++gameCounter, grid});
    }
    return games;
}

void NonogramPuzzleFactory::create_games(const std::vector<int> dimensions, int num_games, const std::string filename)
{
    std::ofstream out(filename, std::ios::trunc);
    out.close();
    int unsolvables = 0;
    std::srand(static_cast<unsigned int>(time(0)));

    for (const auto dimension : dimensions)
    {
        std::vector<std::vector<bool>> grids;
        grids.reserve(num_games);

        while (grids.size() != num_games)
        {
            printf("\r %ld\\%d jogos de tamanho %d gerados", grids.size(), num_games, dimension);
            fflush(stdout);

            std::vector<bool> grid = getDifferentGrid(grids, dimension);
            Nonogram puzzle = fromBool(grid, dimension);
            NonogramSolver solver(puzzle);
            solver.init();
            if (solver.solve() && NonogramRun::verifyCorrectness({dimension, grid}, puzzle))
            {
                grids.push_back(grid);
                printf("\r %ld\\%d jogos de tamanho %d gerados", grids.size(), num_games, dimension);
                fflush(stdout);
                writeGridToFile(grid, dimension, filename);
                grid.clear();
            }
            else
                printf(" %d sem solução..", ++unsolvables);
        }
    }
}

std::vector<bool> NonogramPuzzleFactory::getDifferentGrid(const std::vector<std::vector<bool>> &grid, int dimension)
{
    int static c = 0;
    while (true)
    {

        bool unique = true;

        std::vector<bool> newGrid = RandomGenerator::generateRandomBoolGrid(dimension, dimension);
        for (const auto &oldGrid : grid)
        {
            if (equalGrids(oldGrid, newGrid))
            {
                unique = false;
                break;
            }
        }
        if (unique)
            return newGrid;
        else
        {
            printf("tentativa de numero %d \n", c++);
            fflush(stdout);
        }
    }
}

bool NonogramPuzzleFactory::equalGrids(const std::vector<bool> &oldGrid, const std::vector<bool> &newGrid)
{
    if (oldGrid.size() != newGrid.size())
        return false;

    for (int i = 0; i < oldGrid.size(); i++)
    {
        if (oldGrid[i] != newGrid[i])
            return false;
    }
    return true;
}

void NonogramPuzzleFactory::writeGridToFile(const std::vector<bool> &grid, int dimension, const std::string &filename)
{
    FILE *out = fopen(filename.c_str(), "a");
    if (!out)
        return;
    fprintf(out, "%d ", dimension);
    for (bool b : grid)
        fputc(b ? '1' : '0', out);
    fputc('\n', out);
    fclose(out);
}

void NonogramPuzzleFactory::loadGames(std::string filename)
{
    std::vector<RawPuzzleData> gamesToInit;
    gamesToInit = loadGamesFromFile(filename);

    for (auto it = gamesToInit.begin(); it != gamesToInit.end();)
    {
        Nonogram puzzle = NonogramPuzzleFactory::fromBool(it->grid, it->dimension);
        NonogramSolver solver(puzzle);
        solver.solve();
        puzzle.print();
        it = gamesToInit.erase(it);
    }
}

Nonogram NonogramPuzzleFactory::fromString(const std::string &input)
{
    std::vector<std::string> lines = splitLines(input);

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
    }

    // parse collumns
    for (size_t i = 0; i < lines[0].size(); ++i)
    {
        parseLine(newNonogram.getColumn(i));
    }

    newNonogram.unsetCells();
    return newNonogram;
}

Nonogram NonogramPuzzleFactory::fromBool(const std::vector<bool> &input, int cols)
{
    std::vector<std::vector<bool>> lines = splitLines(input, cols);

    Nonogram newNonogram(lines[0].size(), lines.size());

    // parse lines
    for (size_t i = 0; i < lines.size(); ++i)
    {
        parseLine(newNonogram[i], lines[i]);
    }

    // parse collumns
    for (size_t i = 0; i < lines[0].size(); ++i)
    {
        parseLine(newNonogram.getColumn(i));
    }

    newNonogram.unsetCells();
    return newNonogram;
}

std::vector<std::string> NonogramPuzzleFactory::splitLines(const std::string &str)
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

std::vector<std::vector<bool>> NonogramPuzzleFactory::splitLines(const std::vector<bool> &input, int row_length)
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

bool NonogramPuzzleFactory::isValidLines(const std::vector<std::string> &lines)
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

void NonogramPuzzleFactory::parseLine(Line &line, std::string &linestr)
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

void NonogramPuzzleFactory::parseLine(Line &line, std::vector<bool> boolLine)
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

void NonogramPuzzleFactory::parseLine(Line &line)
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
