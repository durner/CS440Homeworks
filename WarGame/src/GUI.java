import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
public class GUI extends JFrame {
	GridLayout experimentLayout = new GridLayout(6,6);
	Button[][] buttonAry = new Button[6][6];
	JComboBox selectBoard = new JComboBox();
	JComboBox selectStart = new JComboBox();
	JComboBox selectAI = new JComboBox();
	JComboBox selectDepth = new JComboBox();
	JPanel compsToExperiment = new JPanel();
	Button applyButton = new Button("Start the Game!");
	Container mPane;
	Board board;
	boolean buttonPressed = false;
	WarGameThread t;
	Label humanScore = new Label("Human: 0");
	Label aiScore = new Label("AI: 0");
	Label aiCalc = new Label();
	int humanPlayer = 0;
	int aiPlayer = 1;

	public GUI(String name) {
		super(name);
	}


	public void addComponentsToPane(Container pane) {
		mPane = pane;
		compsToExperiment.setLayout(experimentLayout);
		selectBoard.addItem("./board/Keren.txt");
		selectBoard.addItem("./board/Narvik.txt");
		selectBoard.addItem("./board/Sevastopol.txt");
		selectBoard.addItem("./board/Smolensk.txt");
		selectBoard.addItem("./board/Westerplatte.txt");
		selectStart.addItem("Human");
		selectStart.addItem("AI");
		selectAI.addItem("MiniMaxAgent");
		selectAI.addItem("AlphaBetaAgent");
		selectDepth.addItem("1");
		selectDepth.addItem("2");
		selectDepth.addItem("3");
		selectDepth.addItem("4");
		selectDepth.addItem("5");
		selectDepth.addItem("6");

		JPanel controls = new JPanel();
		controls.setLayout(new GridLayout(4, 3));

		controls.add(new Label("Score"));
		controls.add(humanScore);
		controls.add(aiScore);

		controls.add(new Label("Who should start?"));
		controls.add(selectStart);
		controls.add(aiCalc);

		controls.add(new Label("Who'll beat you?"));
		controls.add(selectAI);
		controls.add(selectDepth);

		controls.add(new Label("What's the battlefield?"));
		controls.add(selectBoard);
		controls.add(applyButton);


		pane.add(new JSeparator(), BorderLayout.CENTER);
		pane.add(controls, BorderLayout.NORTH);
		pane.add(compsToExperiment, BorderLayout.SOUTH);

		applyButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				loadGame();
			}
		});

		loadGame();
	}

	private void loadGame() {
		applyButton.setBackground(Color.ORANGE);
		aiScore.setText("AI: 0");
		humanScore.setText("Human: 0");
		if (((String) selectStart.getSelectedItem()).equals("Human")) {
			humanPlayer = 0;
			aiPlayer = 1;
		}
		else {
			humanPlayer = 1;
			aiPlayer = 0;
		}
		int[][] boardValues = WarGame.readBoard((String)selectBoard.getSelectedItem());
		board = new Board(boardValues, 2);
		t = new WarGameThread();
		t.start();
	}

	private void loadBoard (Board board, boolean turn) {
		mPane.remove(compsToExperiment);
		compsToExperiment.removeAll();
		buttonPressed = turn;
		for (int i = 0; i < 6;i++) {
			for (int j = 0; j < 6;j++) {
				buttonAry[i][j] = new Button(""+board.board[i][j]);
				buttonAry[i][j].setPreferredSize(new Dimension(40,80));
				if (board.ownershipOfElement[i][j] == 0) {
					buttonAry[i][j].setBackground(Color.BLUE);
				}
				else if (board.ownershipOfElement[i][j] == 1) {
					buttonAry[i][j].setBackground(Color.GREEN);
				}
				else {
					buttonAry[i][j].addActionListener(new WarGameListener(i,j));
				}
				compsToExperiment.add(buttonAry[i][j]);
			}
		}
		mPane.add(compsToExperiment);
		this.pack();
	}

	/**
	 * Create the GUI and show it.  For thread safety,
	 * this method is invoked from the
	 * event dispatch thread.
	 */
	private static void createAndShowGUI() {
		//Create and set up the window.
		GUI frame = new GUI("WarGame");
		frame.setVisible(true);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		//Set up the content pane.
		frame.addComponentsToPane(frame.getContentPane());
		//Display the window.
		frame.pack();
	}

	public static void main(String[] args) {
		javax.swing.SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				createAndShowGUI();
			}
		});
	}

	private class WarGameListener implements ActionListener {
		public int x;
		public int y;

		public WarGameListener(int x, int y) {
			this.x = x;
			this.y = y;
		}


		public void actionPerformed(ActionEvent e) {
			if (!buttonPressed) {
				board.setStone(humanPlayer, x, y);
				buttonPressed = true;
				synchronized (t) {t.notify();}
			}

		}
	}

	public class WarGameThread extends Thread {
		public void run(){
			Agent a;
			if (selectAI.getSelectedItem().equals("AlphaBetaAgent")) {
				a = new AlphaBetaAgent(Integer.parseInt((String) selectDepth.getSelectedItem()));
			} else {
				a = new MiniMaxAgent(Integer.parseInt((String) selectDepth.getSelectedItem()));
			}


			if (humanPlayer == 0) loadBoard(board, false);
			for (int round = 0; round < board.board.length * board.board.length; round++) {
				if (round % 2 == humanPlayer) {
					aiCalc.setText("Your Turn");
					aiCalc.setBackground(Color.GREEN);
					synchronized (t) {
						while (!buttonPressed) {
							try {
								t.wait();
							} catch (InterruptedException e) {
								e.printStackTrace();
							}
						}
					}
					humanScore.setText("Human: " + board.pointsPlayer[humanPlayer]);
					aiScore.setText("AI: "+board.pointsPlayer[aiPlayer]);
					loadBoard(board, true);

				} else {
					aiCalc.setText("AI IS THINKING");
					aiCalc.setBackground(Color.RED);
					board = a.play(board, aiPlayer, 0);
					humanScore.setText("Human: " + board.pointsPlayer[humanPlayer]);
					aiScore.setText("AI: "+board.pointsPlayer[aiPlayer]);
					aiCalc.setText("Your Turn");
					aiCalc.setBackground(Color.GREEN);
					loadBoard(board, false);
				}
			}
		}
	}

}