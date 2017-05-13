package org.cestra.db;

import org.cestra.db.game.Dbgame;
import org.cestra.db.statique.Dbstatique;
import org.cestra.kernel.Main;

public class Database {
	private final static Dbgame dbgame = new Dbgame();
	private final static Dbstatique dbstatique = new Dbstatique();

	public static void launchDatabase() {
		if (!dbstatique.initializeConnection() || !dbgame.initializeConnection()) {
			Main.stop();
		}
	}

	public static Dbgame getGame() {
		return dbgame;
	}

	public static Dbstatique getStatique() {
		return dbstatique;
	}
}
