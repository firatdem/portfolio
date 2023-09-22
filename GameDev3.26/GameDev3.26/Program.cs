using System;
/*

This ai will play tic tac toe and force a tie



*/

namespace GameDev3._26
{
    class Program
    {
        static void Main(string[] args)
        {
            TicTacToeGrid grid = new TicTacToeGrid();
          
            char currentPlayer = 'X';

            while (true)
            {

                Console.Clear(); // Add this line to clear the console

                Console.WriteLine($"It is {currentPlayer}'s turn.");
                grid.PrintGrid();
                bool isValidMove = false;

                if (currentPlayer == 'X')
                {
                    Console.Write("Enter row and column (separated by space): ");
                    string input = Console.ReadLine();
                    int row = int.Parse(input.Split()[0]);
                    int col = int.Parse(input.Split()[1]);
                    isValidMove = MakeMovePlayerX(grid, row, col);
                    row = 0;
                    col = 0;

                }
                else if (currentPlayer == 'O')
                {
                    TicTacToeGrid gridCopy = new TicTacToeGrid(); // Make copy of grid to work on
                    for (int row = 0; row < 3; row++)
                    {
                        for (int col = 0; col < 3; col++)
                        {
                            gridCopy[row, col] = grid[row, col];
                        }
                    }

                    int bestScore = int.MaxValue;
                    int aiRow = -1;
                    int aiCol = -1;
                    //
                    // IMPLEMENT THE MINIMAX TO RETURN 'O's BEST MOVE
                    for (int row = 0; row < 3; row++)
                    {
                        for (int col = 0; col < 3; col++)
                        {
                            if (gridCopy[row, col] == '-')
                            {
                                gridCopy[row, col] = 'O';

                                int score = gridCopy.Minimax('X', 0, true, gridCopy);// Assume X is maximizing player
                                gridCopy[row, col] = '-';

                                if (score < bestScore)
                                {
                                    bestScore = score;
                                    aiRow = row;
                                    aiCol = col;
                                    break;
                                }
                            }
                        }
                }
                //
                isValidMove = MakeMovePlayerO(grid, aiRow, aiCol); // ROW AND COL ARE FROM MINI MAX
                    
                }                                                   // Not implemented yet

                if (isValidMove)
                {
                    if (grid.CheckWin(currentPlayer))
                    {
                        Console.WriteLine($"Congratulations, {currentPlayer}! You win!");
                        grid.PrintGrid();
                        break;
                    }
                    else if (grid.IsTie())
                    {
                        Console.WriteLine("It's a tie!");
                        grid.PrintGrid();
                        break;
                    }

                    currentPlayer = currentPlayer == 'X' ? 'O' : 'X';
                }
            }

            Console.ReadLine();
        }

        static bool MakeMovePlayerX(TicTacToeGrid grid, int row, int col)
        {
            return grid.MakeMove(row, col, 'X');
        }

        static bool MakeMovePlayerO(TicTacToeGrid grid, int row, int col)
        {
            return grid.MakeMove(row, col, 'O');
        }
    }

    public class TicTacToeGrid
    {
        private char[,] grid = new char[3, 3];
        private int[] lastMove;

        public char this[int row, int col]
        {
            get { return grid[row, col]; }
            set { grid[row, col] = value; }
        }

        public TicTacToeGrid()
        {
            grid = new char[3, 3];
            ResetGrid();
            lastMove = new int[] { 0, 0 }; // Initialize to an invalid move
        }
// MINI MAX ALGO!!!!!!!!!!!!!!!!!!!!!!!!
        public int Minimax(char player, int depth, bool isMaximizingPlayer, TicTacToeGrid grid)
        {
            char opponent = (player == 'X') ? 'O' : 'X';

            if (CheckWin(player))
            {
                return 10 - depth;
            }
            else if (CheckWin(opponent))
            {
                return depth - 10;
            }
            else if (IsTie())
            {
                return 0;
            }

            if (isMaximizingPlayer)
            {
                int bestScore = int.MinValue;
                for (int row = 0; row < 3; row++)
                {
                    for (int col = 0; col < 3; col++)
                    {
                        if (grid[row, col] == '-')
                        {
                            grid[row, col] = player;
                            int score = Minimax(player, depth + 1, false, grid);
                            grid[row, col] = '-';
                            bestScore = Math.Max(bestScore, score);
                        }
                    }
                }
                return bestScore;
            }
            else
            {
                int bestScore = int.MaxValue;
                for (int row = 0; row < 3; row++)
                {
                    for (int col = 0; col < 3; col++)
                    {
                        if (grid[row, col] == '-')
                        {
                            grid[row, col] = opponent;
                            int score = Minimax(player, depth + 1, true, grid);
                            grid[row, col] = '-';
                            bestScore = Math.Min(bestScore, score);
                        }
                    }
                }
                return bestScore;
            }
        }

        public void ResetGrid()
        {
            for (int row = 0; row < 3; row++)
            {
                for (int col = 0; col < 3; col++)
                {
                    grid[row, col] = '-';
                }
            }
        }

        public void PrintGrid()
        {
            for (int row = 0; row < 3; row++)
            {
                for (int col = 0; col < 3; col++)
                {
                    Console.Write(grid[row, col] + " ");
                }
                Console.WriteLine();
            }
        }

        public bool MakeMove(int row, int col, char player)
        {
            if (row < 0 || row > 2 || col < 0 || col > 2)
            {
                Console.WriteLine("Invalid move: row and column must be between 0 and 2.");
                return false;
            }
            else if (grid[row, col] != '-')
            {
                Console.WriteLine("Invalid move: cell is already occupied.");
                return false;
            }
            else
            {
                grid[row, col] = player;
                lastMove[0] = row;
                lastMove[1] = col;
                return true;
            }
        }

        public void UndoLastMove()
        {
            if (lastMove[0] != -1 && lastMove[1] != -1)
            {
                int row = lastMove[0];
                int col = lastMove[1];
                grid[row, col] = '-';
                lastMove[0] = -1;
                lastMove[1] = -1;
            }
            else
            {
                Console.WriteLine("No moves to undo.");
            }
        }


        public bool CheckWin(char player)
        {
            // Check rows
            for (int row = 0; row < 3; row++)
            {
                if (grid[row, 0] == player && grid[row, 1] == player && grid[row, 2] == player)
                {
                    return true;
                }
            }

            // Check columns
            for (int col = 0; col < 3; col++)
            {
                if (grid[0, col] == player && grid[1, col] == player && grid[2, col] == player)
                {
                    return true;
                }
            }

            // Check diagonals
            if (grid[0, 0] == player && grid[1, 1] == player && grid[2, 2] == player)
            {
                return true;
            }
            if (grid[0, 2] == player && grid[1, 1] == player && grid[2, 0] == player)
            {
                return true;
            }

            // No win found
            return false;
        }

        public
     bool IsTie()
        {
            for (int row = 0; row < 3; row++)
            {
                for (int col = 0; col < 3; col++)
                {
                    if (grid[row, col] == '-')
                    {
                        return false;
                    }
                }
            }
            return true;
        }

    }
}
