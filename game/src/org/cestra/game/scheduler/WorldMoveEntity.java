package org.cestra.game.scheduler;

import java.util.concurrent.TimeUnit;

import org.cestra.game.world.World;
import org.cestra.map.Map;

public class WorldMoveEntity extends Manageable {
	@Override
	public void run() {
		for (final Map map : World.getAllMaps()) {
			map.onMapMonstersDisplacement();
			if (map.getMountPark() != null) {
				map.getMountPark().startMoveMounts();
			}
			if (map.getNpcs() != null) {
				map.onMapNpcDisplacement(false);
			}
		}
	}

	@Override
	public void launch() {
		GlobalManager.worldSheduler.scheduleWithFixedDelay(this, 30L, 30L, TimeUnit.MINUTES);
	}
}
