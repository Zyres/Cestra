package org.cestra.fight.turn;

import java.util.Timer;

import org.cestra.fight.Fight;
import org.cestra.fight.Fighter;

public class Turn extends Timer {
	private final TurnTimer timer;

	public Turn(final Fight fight, final Fighter fighter) {
		this.schedule(this.timer = new TurnTimer(fight, fighter), 30000L);
	}

	public TurnTimer getTimer() {
		return this.timer;
	}

	public void stop() {
		try {
			this.getTimer().stop();
			this.cancel();
			this.purge();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
