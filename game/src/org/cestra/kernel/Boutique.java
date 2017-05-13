package org.cestra.kernel;

import java.util.ArrayList;
import java.util.List;

import org.cestra.client.Player;
import org.cestra.client.other.Stats;
import org.cestra.common.SocketManager;
import org.cestra.game.world.World;
import org.cestra.object.ObjectTemplate;

public class Boutique {
	public static final List<ObjectTemplate> items = new ArrayList<>();
	private static String packet;

	public static void initPacket() {
		packet = getObjectList();
	}

	public static void open(Player player) {
		player.boutique = true;
		SocketManager.send(player, "ECK0|1");
		SocketManager.send(player, "EL" + packet);
	}

	private static String getObjectList() {
		StringBuilder items = new StringBuilder();
		for (ObjectTemplate obj : Boutique.items) {
			Stats stats = obj.generateNewStatsFromTemplate(obj.getStrTemplate(), true);
			items.append(obj.getId() + ";" + stats.parseToItemSetStats()).append("|");

		}
		return items.toString();
	}
}
