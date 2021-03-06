from Board import Board

bin_quantity = 6
alpha_beta_pruning = 1
human = 2
inf = 1e9
win = 1e4
heuristic1 = 3
heuristic2 = 4
heuristic3 = 5
class Player(object):
    def __init__(self, playerNo, playerType, heuristicNo, W1, W2, W3, W4, depth):
        self.playerNo = playerNo
        self.playerType = playerType
        self.tempBoard = Board()
        self.turn = -1

        # if alpha-beta pruning is used then these are required
        self.heuristicNo = heuristicNo
        self.W1 = W1
        self.W2 = W2
        self.W3 = W3
        self.W4 = W4
        self.depth = depth

        # additional move
        self.additionalMoveEarned = None

        self.otherPlayer = 1
        if playerNo == 1:
            self.otherPlayer = 2
        
    def getNextMove(self, board):
        if self.playerType == human:
            return self.__getHumanMove(board)

        elif self.playerType == alpha_beta_pruning:
            self.additionalMoveEarned = 0
            bn = self.__MiniMax(board, self.depth, True, -inf, inf)
          # print("Ckati linx avanyra:", bn)
            print("Computer :", bn)
            return bn

        else:
            print("invalid player type")
            return 1
 
    #print("Cc chekkkk :", bn)

    # minimax algorithm using alpha-beta pruning
    def __MiniMax(self, board, depth, isMax, alpha, beta):
        # if the game is over
        if board.gameOver(False):
            if board.checkWinner() == self.playerNo:
                return win
            elif board.checkWinner() == self.otherPlayer:
                return -win
            else:
                return self.__evaluateNode(board)

        if depth == 0:
            return self.__evaluateNode(board)

        # save a copy of the board
        myBoard = Board()
        myBoard.copyBoardDetail(board)

        if isMax:
            best_value = -inf
            successor = -1
            for i in range(1, bin_quantity + 1):
                if board.bin[self.playerNo][i] > 0:
                    temp_bn = board.updateBoard(self.playerNo, i)

                    # if additional turn achieved
                    if temp_bn == self.playerNo:
                        self.additionalMoveEarned += 1
                        curr_value = self.__MiniMax(board, depth - 1, True, alpha, beta)
                        self.additionalMoveEarned -= 1
                    else:
                        curr_value = self.__MiniMax(board, depth - 1, False, alpha, beta)

                    # we do this only in the maximizer because the root is "Max"
                    # the root only needs to decide the successor
                    if curr_value > best_value:
                        best_value = curr_value
                        successor = i

                    alpha = max(best_value, alpha)
                    if beta <= alpha:
                        break

                    board.copyBoardDetail(myBoard)  # restore original board

            # if root then return the successor
            if self.depth == depth:
                return successor

            return best_value

        else:
            best_value = inf
            for i in range(1, bin_quantity + 1):
                if board.bin[self.otherPlayer][i] > 0:
                    # min is the opponent aka other-player
                    temp_bn = board.updateBoard(self.otherPlayer, i)

                    # if other player gets an additional turn
                    if temp_bn == self.otherPlayer:
                        self.additionalMoveEarned -= 1
                        curr_value = self.__MiniMax(board, depth - 1, False, alpha, beta)
                        self.additionalMoveEarned += 1
                    else:
                        curr_value = self.__MiniMax(board, depth - 1, True, alpha, beta)

                    best_value = min(best_value, curr_value)
                    beta = min(best_value, beta)

                    if beta <= alpha:
                        break

                    board.copyBoardDetail(myBoard)  # restore original board
            return best_value

    def __evaluateNode(self, board):
        if self.heuristicNo == heuristic1:
            return self.__HeuristicOne(board)
        elif self.heuristicNo == heuristic2:
            return self.__HeuristicTwo(board)
        elif self.heuristicNo == heuristic3:
            return self.__HeuristicThree(board)
       

    def __getHumanMove(self, board):
        while True:
            bn = int(input())
           # bn = int(input('Enter move '))
          #  print("Cc chekkkk :", bn)
            if bn <= 0 or bn > bin_quantity:
                print("input out of range")
            elif board.bin[self.playerNo][bn] == 0:
                print("empty bin :", bn)
            else:
                return bn

    def __HeuristicOne(self, board):
        return board.storage[self.playerNo] - board.storage[self.otherPlayer]

    def __HeuristicTwo(self, board):
        stones_on_my_side = sum(board.bin[self.playerNo][1:bin_quantity + 1])
        stones_on_opponents_side = sum(board.bin[self.otherPlayer][1:bin_quantity + 1])
        stones_in_my_storage = board.storage[self.playerNo]
        stones_in_opponents_storage = board.storage[self.otherPlayer]

        ret = self.W1 * (stones_in_my_storage - stones_in_opponents_storage) + \
              self.W2 * (stones_on_my_side - stones_on_opponents_side)

        return ret

    def __HeuristicThree(self, board):
        additional_move_earned = self.additionalMoveEarned
        return self.__HeuristicTwo(board) + self.W3 * additional_move_earned

    
