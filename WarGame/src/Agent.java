public class Agent {
	protected int depth;
	protected int expandedNodes = 0;
	
	public Agent(int depth) {
		this.depth = depth;
	}
	
	Board play(Board currentBoard, int currentPlayer, int depth) {
		return null;
	}

	public int getExpandedNode() {
		return expandedNodes;
	}
}