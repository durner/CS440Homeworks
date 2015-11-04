public class AlphaBetaAgent extends Agent{
	
	public AlphaBetaAgent(int depth) {
		super(depth);
	}

	@Override
	Board play(Board currentBoard, int currentPlayer, int depth) {
		return play(currentBoard, currentPlayer, depth, Integer.MIN_VALUE, Integer.MAX_VALUE);
	}
		
	Board play(Board currentBoard, int currentPlayer, int depth, int alpha, int beta) {
		expandedNodes++;
		// stop recursion on depth limit or terminal status
		if (this.depth <= depth || currentBoard.isTerminal()) {
			return currentBoard;
		}
		int bestValue;
		Board returnBoard = null;


		if (currentPlayer == 0) bestValue = Integer.MIN_VALUE;
		else bestValue = Integer.MAX_VALUE;
		Board simulBoard = currentBoard.clone();

		for (Board.Simul s : simulBoard.getPruneOrderedMoves(currentPlayer)) {
			// clone board
			Board newBoard = currentBoard.clone();

			// set the current stone and prepare the simulation tree
			newBoard.setStone(currentPlayer, s.x, s.y);

			// give the opponents the next nodes
			int nextPlayer = currentPlayer + 1;
			nextPlayer = (nextPlayer == newBoard.pointsPlayer.length) ? 0 : nextPlayer;

			// simulate the next level in the game tree
			newBoard = play(newBoard, nextPlayer, depth+1, alpha, beta);

			// maximize this players utilities
			int newVal = newBoard.playerValue(currentPlayer);

			// maximizer
			if (currentPlayer == 0) {
				// v = max(v, succ)
				if (newVal > bestValue) {
					bestValue = newVal;
					// for depth = 0 i want to return the new real game board
					// for all other depths i want to have the simulated board to compare the utilties
					if (depth != 0) returnBoard = newBoard;
					else {
						returnBoard = currentBoard.clone();
						returnBoard.setStone(currentPlayer, s.x, s.y);
					}
				}
				if (alpha < bestValue) alpha = bestValue;
				if(beta <= alpha)
				{
					return returnBoard;
				}
			}

			// minimizer
			else if (currentPlayer == 1) {
				// v = min(v, succ)
				if (newVal < bestValue) {
					bestValue = newVal;
					// for depth = 0 i want to return the new real game board
					// for all other depths i want to have the simulated board to compare the utilties
					if (depth != 0) returnBoard = newBoard;
					else {
						returnBoard = currentBoard.clone();
						returnBoard.setStone(currentPlayer, s.x, s.y);
					}
				}
				if (beta > bestValue) beta = bestValue;
				if(beta <= alpha)
				{
					return returnBoard;
				}
			}
		}
		return returnBoard;
	}

}