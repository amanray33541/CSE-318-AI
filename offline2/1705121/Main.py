from Player import Player
from Board import Board

alpha_beta_pruning = 1
heuristic1 = 3
heuristic2 = 4
heuristic3 = 5


# required only when minimax algo is used
heuristicNo = heuristic3
W1 = 30
W2 = 20
W3 = 5
W4 = 5
depth = 5

board = Board()
p1 = Player(1, alpha_beta_pruning, heuristic2, W1, W2, W3, W4, 5)
p2 = Player(2, alpha_beta_pruning, heuristic3, W1, W2, W3, W4, 10)

turn = 1

# when True is passed to gameOver it will show result
while board.gameOver(True) == False:
    board.printBoard()
    print("player -", turn, "\'s turn")
  #  print("//aman")

    if turn == 1:
        bn = p1.getNextMove(board)
    else:
        bn = p2.getNextMove(board)

    turn = board.updateBoard(turn, bn)


