#include <iostream>
#include <string>
#include "../src/Nonogram.h"

// #define DEBUG 1

#ifdef DEBUG
#define DBG(x) std::cout << x << std::endl
#else
#define DBG(x)
#endif

class RandomGenerator
{
public:
    static Nonogram fromString(const std::string &input)
    {
        std::vector<std::string> lines = splitLines(input);

        DBG("Testando string: \n" << input);

        if (!isValidLines(lines))
        {
            std::cerr << "Invalid grid string" << std::endl;
            exit(1);
        }

        Nonogram new_nonogram(lines[0].size(), lines.size());

        // parse lines
        for (size_t i = 0; i < lines.size(); ++i)
        {
            parseLine(new_nonogram[i],lines[i]);
            DBG("Dicas da linha " << i << ": " << new_nonogram[i].getHints().toString());
        }
        
        // parse collumns
        for (size_t i = 0; i < lines[0].size(); ++i)
        {
            parseLine(new_nonogram.getColumn(i));
            DBG("Dicas da coluna " << i << ": " << new_nonogram.getColumn(i).getHints().toString());
        }
        
        new_nonogram.unsetCells();
        return new_nonogram;
    }

    static Nonogram random(int rows, int cols)
    {
        std::string grid = generateRandomGridString(rows, cols);
        Nonogram nonogram = fromString(grid);
        return nonogram;
    }

    static void printRandomString(int rows, int cols)
    {
        std::string grid = generateRandomGridString(rows, cols);
        std::cout << grid;
    }

private: // metodos auxiliares
    // Função auxiliar para gerar uma string de grid aleatória com 0s e 1s
    static std::string generateRandomGridString(int rows, int cols)
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

    // Função auxiliar para dividir uma string em suas linhas
    static std::vector<std::string> splitLines(const std::string &str)
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

    // funcao para verificar se as linhas da string sao validas (contem apenas 0 e 1s e linhas do mesmo tamanho), recebe vetor de linhas
    static bool isValidLines(const std::vector<std::string> &lines)
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
    static void parseLine(Line &line, std::string &linestr)
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

    // Parser de uma Line já inicializada
    static void parseLine(Line &line)
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

};