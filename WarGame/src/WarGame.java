import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class WarGame {
	public static void main(String[] args) {
		if (args.length < 1)
			return;

		System.out.println("=======================================");
		System.out.println("Board: "+ args[0]);
		System.out.println("---------------------------------------");
		
		int players = 2;

		Agent[] agents = new Agent[players];
		long[] timeAgent = new long[players];

		agents[0] = new MiniMaxAgent(4);
		agents[1] = new MiniMaxAgent(4);
		timeAgent[0] = 0;
		timeAgent[1] = 0;
		long time = 0;

		int[][] startBoard = readBoard(args[0]);
		Board board = new Board(startBoard, players);
		int length = startBoard.length * startBoard.length;

		System.out.println("MiniMax(4) vs MiniMax(4) - Scores");
		for (int round = 0; round < length; round++) {
			time = System.currentTimeMillis();
			board = agents[round % players].play(board, round % players, 0);
			timeAgent[round % players] += System.currentTimeMillis() - time;
		}
		board.printBoard();
		for (int i = 0; i < players; i++) {
			System.out.println("Player "+i+": "+board.pointsPlayer[i] + " -- TotalTime: " + timeAgent[i]
				+ "ms -- Time/Move: " + timeAgent[i]/(length/2)+ "ms -- TotalExpandedNode: "
				+ agents[i].expandedNodes + " -- ExpandedNode/Move: " + agents[i].expandedNodes/(length/2));
		}


		System.out.println("---------------------------------------");

		agents[0] = new AlphaBetaAgent(6);
		agents[1] = new AlphaBetaAgent(6);
		timeAgent[0] = 0;
		timeAgent[1] = 0;
		time = 0;

		startBoard = readBoard(args[0]);
		board = new Board(startBoard, players);
		length = startBoard.length * startBoard.length;

		System.out.println("AlphaBeta(6) vs AlphaBeta(6) - Scores");
		for (int round = 0; round < length; round++) {
			time = System.currentTimeMillis();
			board = agents[round % players].play(board, round % players, 0);
			timeAgent[round % players] += System.currentTimeMillis() - time;
		}
		board.printBoard();
		for (int i = 0; i < players; i++) {
			System.out.println("Player "+i+": "+board.pointsPlayer[i] + " -- TotalTime: " + timeAgent[i]
					+ "ms -- Time/Move: " + timeAgent[i]/(length/2)+ "ms -- TotalExpandedNode: "
					+ agents[i].expandedNodes + " -- ExpandedNode/Move: " + agents[i].expandedNodes/(length/2));
		}


		System.out.println("---------------------------------------");

		agents[0] = new MiniMaxAgent(4);
		agents[1] = new AlphaBetaAgent(6);
		timeAgent[0] = 0;
		timeAgent[1] = 0;
		time = 0;

		startBoard = readBoard(args[0]);
		board = new Board(startBoard, players);
		length = startBoard.length * startBoard.length;

		System.out.println("MiniMax(4) vs AlphaBeta(6) - Scores");
		for (int round = 0; round < length; round++) {
			time = System.currentTimeMillis();
			board = agents[round % players].play(board, round % players, 0);
			timeAgent[round % players] += System.currentTimeMillis() - time;
		}
		board.printBoard();
		for (int i = 0; i < players; i++) {
			System.out.println("Player "+i+": "+board.pointsPlayer[i] + " -- TotalTime: " + timeAgent[i]
					+ "ms -- Time/Move: " + timeAgent[i]/(length/2)+ "ms -- TotalExpandedNode: "
					+ agents[i].expandedNodes + " -- ExpandedNode/Move: " + agents[i].expandedNodes/(length/2));
		}


		System.out.println("---------------------------------------");

		agents[0] = new AlphaBetaAgent(6);
		agents[1] = new MiniMaxAgent(4);
		timeAgent[0] = 0;
		timeAgent[1] = 0;
		time = 0;

		startBoard = readBoard(args[0]);
		board = new Board(startBoard, players);
		length = startBoard.length * startBoard.length;

		System.out.println("AlphaBeta(6) vs MiniMax(4) - Scores");
		for (int round = 0; round < length; round++) {
			time = System.currentTimeMillis();
			board = agents[round % players].play(board, round % players, 0);
			timeAgent[round % players] += System.currentTimeMillis() - time;
		}
		board.printBoard();
		for (int i = 0; i < players; i++) {
			System.out.println("Player "+i+": "+board.pointsPlayer[i] + " -- TotalTime: " + timeAgent[i]
					+ "ms -- Time/Move: " + timeAgent[i]/(length/2)+ "ms -- TotalExpandedNode: "
					+ agents[i].expandedNodes + " -- ExpandedNode/Move: " + agents[i].expandedNodes/(length/2));
		}
		System.out.println("=======================================");
	}
	
	public static int[][] readBoard(String boardPath) {
		BufferedReader br = null;
		int[][] solution = null;
		try {
			br = new BufferedReader(new FileReader(boardPath));
			String line = br.readLine();
			int lineCnt = 0;
			while (line != null) {
				String[] svals = line.split("\\t");
				if (solution == null) solution = new int[svals.length][svals.length];
				for (int i = 0; i < svals.length; i++) {
					solution[lineCnt][i] = Integer.parseInt(svals[i]);
				}
				line = br.readLine();
				lineCnt++;
			}
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (br != null)
			try {
				br.close();
			} catch (IOException e) {}
		}
		return solution;
	}

}
