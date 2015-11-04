public class MiniMaxAgent extends Agent{
	
	public MiniMaxAgent(int depth) {
		super(depth);
	}

	@Override
	Board play(Board currentBoard, int currentPlayer, int depth) {
		expandedNodes++;

		// stop recursion on depth limit or terminal status
		if (this.depth <= depth || currentBoard.isTerminal()) {
			return currentBoard;
		}


		int bestValue;
		if (currentPlayer == 0) bestValue = Integer.MIN_VALUE;
		else bestValue = Integer.MAX_VALUE;
		
		Board returnBoard = null;
		for (int x = 0; x < currentBoard.board.length; x++) {
			for (int y = 0; y < currentBoard.board[x].length; y++) {
				// for minimax this is fine since the order is unimportant!
				if (currentBoard.playable(x,y)) {
					// clone board
					Board newBoard = currentBoard.clone();
					
					// set the current stone and prepare the simulation tree
					newBoard.setStone(currentPlayer, x, y);
					
					// give the opponents the next nodes
					int nextPlayer = currentPlayer + 1;
					nextPlayer = (nextPlayer == newBoard.pointsPlayer.length) ? 0 : nextPlayer;
					
					// simulate the next level in the game tree
					newBoard = play(newBoard, nextPlayer, depth+1);
					
					// maximize this players utilities
					int newVal = newBoard.playerValue(currentPlayer);
					
					// maximizer
					if (currentPlayer == 0 && newVal > bestValue) {
						bestValue = newVal;
						// for depth = 0 i want to return the new real game board
						// for all other depths i want to have the simulated board to compare the utilties
						if (depth != 0) returnBoard = newBoard;
						else {
							returnBoard = currentBoard.clone();
							returnBoard.setStone(currentPlayer, x, y);
						}
					}
					
					// minimizer
					else if (currentPlayer == 1 && newVal < bestValue) {
						bestValue = newVal;
						// for depth = 0 i want to return the new real game board
						// for all other depths i want to have the simulated board to compare the utilties
						if (depth != 0) returnBoard = newBoard;
						else {
							returnBoard = currentBoard.clone();
							returnBoard.setStone(currentPlayer, x, y);
						}
					}
				}
			}
		}
		return returnBoard;
	}

}