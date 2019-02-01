#include "../../include/Grid/grid.hpp"
#include "../../include/Grid/common_square.hpp"
#include "../../include/Grid/market_square.hpp"
#include "../../include/Grid/non_accessible_square.hpp"
#include "../../include/Grid/square.hpp"

using namespace std;

Grid::Grid()
{
    int j = 0, i = 0;
    for (i = 0; i < 12; i++)
    {
        for (j = 0; j < 12; j++)
        {
            //==================== Non Accesable Squares =============================//
            if (i == 0 ||
                i == 11 ||
                j == 0 ||
                j == 11 ||
                (i == 1 && j == 9) ||
                (i == 2 && j == 6) ||
                (i == 2 && j == 7) ||
                (i == 2 && j == 8) ||
                (i == 2 && j == 9) ||
                (i == 3 && j == 6) ||
                (i == 3 && j == 9) ||
                (i == 4 && j == 6) ||
                (i == 4 && j == 7) ||
                (i == 4 && j == 9) ||
                (i == 5 && j == 1) ||
                (i == 5 && j == 2) ||
                (i == 5 && j == 3) ||
                (i == 5 && j == 9) ||
                (i == 6 && j == 3) ||
                (i == 6 && j == 9) ||
                (i == 7 && j == 3) ||
                (i == 7 && j == 7) ||
                (i == 7 && j == 8) ||
                (i == 7 && j == 9) ||
                (i == 8 && j == 7) ||
                (i == 9 && j == 3) ||
                (i == 9 && j == 7) ||
                (i == 9 && j == 8) ||
                (i == 9 && j == 9) ||
                (i == 10 && j == 3))
            {
                this->grid[i][j] = new NonAccesableSquare();
            }
            //=========================== Market Squares =============================//
            else if (
                (i == 2 && j == 5) ||
                (i == 6 && j == 1) ||
                (i == 8 && j == 8))
            {
                switch (i)
                {
                    {
                    case 2:
                        this->grid[i][j] = new MarketSquare(1);
                        break;
                    case 6:
                        this->grid[i][j] = new MarketSquare(2);
                        break;
                    case 8:
                        this->grid[i][j] = new MarketSquare(3);
                        break;
                    }
                }
            }
            //=================== Common Squares With Monsters =======================//
            else
            {
                this->grid[i][j] = new CommonSquare();
            }
        }
    }
}

void Grid::DisplayMap(int x, int y)
{
    int i, j, height, width;
    height = (12 * 2) + 1;
    width = 12;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (i == 0 || i == height)
            {
                if (j == 0)
                {
                    cout << "-";
                }
                cout << "----";
            }
            else if (i % 2 == 0)
            {
                if (j == 0)
                {
                    cout << "-";
                }
                cout << "----";
            }
            else if (i % 2 == 1)
            {
                if (i / 2 == x && j == y)
                {
                    cout << " Η |";
                    continue;
                }
                if (j == 0)
                {
                    cout << "|";
                }
                if (NonAccesableSquare *naSq = dynamic_cast<NonAccesableSquare *>(grid[i / 2][j]))
                {
                    cout << "###|";
                }
                else if (MarketSquare *maSq = dynamic_cast<MarketSquare *>(grid[i / 2][j]))
                {
                    cout << " $ |";
                }
                else if (CommonSquare *coSq = dynamic_cast<CommonSquare *>(grid[i / 2][j]))
                {
                    cout << "   |";
                }
            }
        }
        cout << endl;
    }

    cout << "$ - are the markets, H - are the heroes, ### - are the non accessable squares," << endl;
}

Grid::~Grid() {}