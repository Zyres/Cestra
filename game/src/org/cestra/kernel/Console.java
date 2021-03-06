package org.cestra.kernel;

import org.fusesource.jansi.AnsiConsole;

public class Console {
	public static void initialize() {
		setTitle("Game Server - Loading...");
	}

	public static void begin() {
		initialize();
		println("\nThe server has started successfully! Waiting to connect ...", Color.SUCCESS);
		println("\nHELP To see the list of commands available in this console.\n", Color.INFORMATION);
	}

	public static void refreshTitle() {
		if (!Main.isRunning) {
			return;
		}
		final String title = "Game Server - Port : " + Main.gamePort + " | " + Main.key + " | " + Main.gameServer.getPlayerNumber() + " Player(s)";
		setTitle(title);
	}

	public static void println(final Object o, final Color color) {
		AnsiConsole.out.println("\u001b[1m\u001b[" + color.get() + "m" + o + "\u001b[0m");
	}

	public static void print(final Object o, final Color color) {
		AnsiConsole.out.print("\u001b[1m\u001b[" + color.get() + "m" + o + "\u001b[0m");
	}

	public static void clear() {
		AnsiConsole.out.print("\u001b[H\u001b[2J");
	}

	public static void setTitle(final String title) {
		AnsiConsole.out.printf("%c]0;%s%c", '\u001b', title, '\u0007');
	}

	public enum Color {
		ERROR("ERROR", 0, 31), SUCCESS("SUCCESS", 1, 32), WAITING("WAITING", 2, 33), EXCHANGE("EXCHANGE", 3,
				34), INFORMATION("INFORMATION", 4, 35), GAME("GAME", 5, 36), BOLD("BOLD", 6, 1), UNDERLINE("UNDERLINE",
						7, 4), BLACK("BLACK", 8, 30), RED("RED", 9, 31), GREEN("GREEN", 10,
								32), YELLOW("YELLOW", 11, 33), BLUE("BLUE", 12, 34), WHITE("WHITE", 13, 37);

		private int color;

		private Color(final String s, final int n, final int color) {
			this.color = color;
		}

		public int get() {
			return this.color;
		}
	}
}
