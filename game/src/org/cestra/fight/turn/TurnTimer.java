package org.cestra.fight.turn;

import java.util.TimerTask;

import org.cestra.fight.Fight;
import org.cestra.fight.Fighter;

public class TurnTimer extends TimerTask {
	private final Fight fight;
	private final Fighter fighter;
	private boolean stop;

	public TurnTimer(final Fight fight, final Fighter fighter) {
		this.stop = false;
		this.fight = fight;
		this.fighter = fighter;
	}

	public void stop() {
		this.stop = true;
		this.cancel();
	}

	@Override
	public void run() {
		if (this.stop || this.fight == null) {
			this.stop();
			return;
		}
		if (this.fight.getOrderPlaying() == null) {
			this.stop();
			return;
		}
		if (this.fight.getOrderPlaying().get(this.fight.getCurPlayer()) == null) {
			this.stop();
			return;
		}
		if (this.fight.getOrderPlaying().get(this.fight.getCurPlayer()) != this.fighter) {
			this.stop();
			return;
		}
		this.fight.endTurn(false);
	}
}
