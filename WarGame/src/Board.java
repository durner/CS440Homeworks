import java.util.ArrayList;
import java.util.*;

public class Board implements Cloneable {
	int[][] board;
	int[][] ownershipOfElement; //player 0 or 1 or -1
	int[] pointsPlayer;

	Board(int[][] board, int players) {
		this.board = board;
		ownershipOfElement = new int[board.length][board.length];
		for (int i = 0; i < board.length; i++) {
			for (int j = 0; j < board.length; j++) {
				ownershipOfElement[i][j] = -1;
			}
		}
		pointsPlayer = new int[players];
	}

	Board(int edgeSize, int players) {
		board = new int[edgeSize][edgeSize];
		for (int i = 0; i < edgeSize; i++) {
			for (int j = 0; j < edgeSize; j++) {
				board[i][j] = 1;
				ownershipOfElement[i][j] = -1;
			}
		}
		pointsPlayer = new int[players];
	}

	private Board() {
	}

	boolean setStone(int player, int x, int y) {
		if (ownershipOfElement[x][y] == -1) {
			if (x > 0 && ownershipOfElement[x - 1][y] == player) {
				deathBlitz(player, x, y);
			} else if (x < board.length - 1 && ownershipOfElement[x + 1][y] == player) {
				deathBlitz(player, x, y);
			} else if (y > 0 && ownershipOfElement[x][y - 1] == player) {
				deathBlitz(player, x, y);
			} else if (y < board.length - 1 && ownershipOfElement[x][y + 1] == player) {
				deathBlitz(player, x, y);
			} else {
				ownershipOfElement[x][y] = player;
				pointsPlayer[player] += board[x][y];
			}
			return true;
		}
		return false;
	}

	private void deathBlitz(int player, int x, int y) {
		ownershipOfElement[x][y] = player;
		pointsPlayer[player] += board[x][y];
		if (x > 0 && ownershipOfElement[x - 1][y] != player
				&& ownershipOfElement[x - 1][y] != -1) {
			pointsPlayer[ownershipOfElement[x - 1][y]] -= board[x - 1][y];
			ownershipOfElement[x - 1][y] = player;
			pointsPlayer[player] += board[x - 1][y];
		}
		if (x < board.length - 1 && ownershipOfElement[x + 1][y] != player
				&& ownershipOfElement[x + 1][y] != -1) {
			pointsPlayer[ownershipOfElement[x + 1][y]] -= board[x + 1][y];
			ownershipOfElement[x + 1][y] = player;
			pointsPlayer[player] += board[x + 1][y];
		}
		if (y > 0 && ownershipOfElement[x][y - 1] != player
				&& ownershipOfElement[x][y - 1] != -1) {
			pointsPlayer[ownershipOfElement[x][y - 1]] -= board[x][y - 1];
			ownershipOfElement[x][y - 1] = player;
			pointsPlayer[player] += board[x][y - 1];
		}
		if (y < board.length - 1 && ownershipOfElement[x][y + 1] != player
				&& ownershipOfElement[x][y + 1] != -1) {
			pointsPlayer[ownershipOfElement[x][y + 1]] -= board[x][y + 1];
			ownershipOfElement[x][y + 1] = player;
			pointsPlayer[player] += board[x][y + 1];
		}
	}



	public void printBoard() {
		for (int i = 0; i < board.length; i++) {
			for (int j = 0; j < board.length; j++) {
				System.out.print("P" + ownershipOfElement[i][j] + ":" + board[i][j] + "\t");
			}
			System.out.print("\n");
		}
	}

	protected Board clone() {
		Board b = new Board();
		b.board = new int[board.length][board.length];
		b.ownershipOfElement = new int[board.length][board.length];
		b.pointsPlayer = new int[pointsPlayer.length];
		for (int i = 0; i < pointsPlayer.length; i++) {
			b.pointsPlayer[i] = pointsPlayer[i];
		}
		for (int i = 0; i < board.length; i++) {
			for (int j = 0; j < board.length; j++) {
				b.ownershipOfElement[i][j] = ownershipOfElement[i][j];
				b.board[i][j] = board[i][j];
			}
		}
		return b;
	}

	public boolean playable(int x, int y) {
		if (x >= 0 && y >= 0 && x < board.length && y < board.length) {
			return (ownershipOfElement[x][y] == -1) ? true : false;
		}
		return false;
	}

	public int playerValue(int player) {
		if (pointsPlayer.length == 2) {
			return pointsPlayer[0] - pointsPlayer[1];
		} else
			return pointsPlayer[player];
	}

	public boolean isTerminal() {
		int free = 0;
		for (int x = 0; x < board.length; x++) {
			for (int y = 0; y < board.length; y++) {
				if (ownershipOfElement[x][y] == -1) free++;
			}
		}
		return (free == 0) ? true : false;
	}

	public ArrayList<Simul> getPruneOrderedMoves(int player) {
		ArrayList<Simul> cords = new ArrayList<Simul>();
		for (int x = 0; x < board.length; x++) {
			for (int y = 0; y < board[x].length; y++) {
				if (playable(x, y)) {
					Simul simul = setStoneSimulation(player, x, y);
					cords.add(simul);
				}
			}
		}
		Collections.sort(cords,new Comparator<Simul>() {
			public int compare(Simul s1, Simul s2) {
				int comp =  s1.value - s2.value;
				return comp;
			}
		});
		if (player == 0) Collections.reverse(cords);
		return cords;
	}

	Simul setStoneSimulation(int player, int x, int y) {
		int[] pointsPlayerSimul = new int[2];
		pointsPlayerSimul[0] = pointsPlayer[0];
		pointsPlayerSimul[1] = pointsPlayer[1];
		if (ownershipOfElement[x][y] == -1) {
			if (x > 0 && ownershipOfElement[x - 1][y] == player) {
				pointsPlayerSimul = deathBlitzSimulation(player, x, y, pointsPlayerSimul);
			} else if (x < board.length - 1 && ownershipOfElement[x + 1][y] == player) {
				pointsPlayerSimul = deathBlitzSimulation(player, x, y, pointsPlayerSimul);
			} else if (y > 0 && ownershipOfElement[x][y - 1] == player) {
				pointsPlayerSimul = deathBlitzSimulation(player, x, y, pointsPlayerSimul);
			} else if (y < board.length - 1 && ownershipOfElement[x][y + 1] == player) {
				pointsPlayerSimul = deathBlitzSimulation(player, x, y, pointsPlayerSimul);
			} else {
				pointsPlayerSimul[player] += board[x][y];
			}
		}
		Simul simul = new Simul(x,y,pointsPlayerSimul[0] - pointsPlayerSimul[1]);
		return simul;
	}

	private int[] deathBlitzSimulation(int player, int x, int y, int[] pointsPlayerSimul) {
		pointsPlayerSimul[player] += board[x][y];
		if (x > 0 && ownershipOfElement[x - 1][y] != player
				&& ownershipOfElement[x - 1][y] != -1) {
			pointsPlayerSimul[ownershipOfElement[x - 1][y]] -= board[x - 1][y];
			pointsPlayerSimul[player] += board[x - 1][y];
		}
		if (x < board.length - 1 && ownershipOfElement[x + 1][y] != player
				&& ownershipOfElement[x + 1][y] != -1) {
			pointsPlayerSimul[ownershipOfElement[x + 1][y]] -= board[x + 1][y];
			pointsPlayerSimul[player] += board[x + 1][y];
		}
		if (y > 0 && ownershipOfElement[x][y - 1] != player
				&& ownershipOfElement[x][y - 1] != -1) {
			pointsPlayerSimul[ownershipOfElement[x][y - 1]] -= board[x][y - 1];
			pointsPlayerSimul[player] += board[x][y - 1];
		}
		if (y < board.length - 1 && ownershipOfElement[x][y + 1] != player
				&& ownershipOfElement[x][y + 1] != -1) {
			pointsPlayerSimul[ownershipOfElement[x][y + 1]] -= board[x][y + 1];
			pointsPlayerSimul[player] += board[x][y + 1];
		}
		return pointsPlayerSimul;
	}

	public class Simul{
		int x;
		int y;
		int value;

		public  Simul(int x, int y, int value) {
			this.x = x;
			this.y = y;
			this.value = value;
		}
	};

}