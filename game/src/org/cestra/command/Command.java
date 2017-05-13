package org.cestra.command;

import org.cestra.client.Account;
import org.cestra.client.Player;
import org.cestra.game.GameClient;

public abstract class Command {
	private Account account;
	private Player player;
	private GameClient client;
	private boolean timerStart;

	public Command(final Player player) {
		this.timerStart = false;
		this.account = player.getAccount();
		this.player = player;
		this.client = player.getAccount().getGameClient();
	}

	public Account getAccount() {
		return this.account;
	}

	public Player getPlayer() {
		return this.player;
	}

	public GameClient getClient() {
		return this.client;
	}

	public boolean isTimerStart() {
		return this.timerStart;
	}

	public abstract void apply(final String p0);
}
