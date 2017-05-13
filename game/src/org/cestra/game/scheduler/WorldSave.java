package org.cestra.game.scheduler;

import java.util.concurrent.TimeUnit;

import org.cestra.client.Account;
import org.cestra.client.Player;
import org.cestra.common.SocketManager;
import org.cestra.db.Database;
import org.cestra.entity.Collector;
import org.cestra.entity.Dragodinde;
import org.cestra.entity.Prism;
import org.cestra.game.GameServer;
import org.cestra.game.world.World;
import org.cestra.kernel.Console;
import org.cestra.kernel.Main;
import org.cestra.map.MountPark;
import org.cestra.other.Guild;
import org.cestra.other.House;
import org.cestra.other.Trunk;

public class WorldSave extends Manageable {
	@Override
	public void run() {
		if (!Main.isSaving) {
			cast(0);
		}
	}

	@Override
	public void launch() {
		GlobalManager.worldSheduler.scheduleWithFixedDelay(this, 30L, 30L, TimeUnit.MINUTES);
	}

	public static void cast(final int trys) {
		GameServer.setState(2);
		Label_0913: {
			try {
				Console.println("Starting Backup:", Console.Color.GAME);
				SocketManager.GAME_SEND_Im_PACKET_TO_ALL("1164;");
				Main.isSaving = true;
				Console.println("-> Accounts.", Console.Color.GAME);
				for (final Account account : World.getAccounts().values()) {
					if (account == null) {
						continue;
					}
					Database.getStatique().getAccountData().update(account);
				}
				Console.println("-> Characters.", Console.Color.GAME);
				for (final Player player : World.getPlayers().values()) {
					if (player == null) {
						continue;
					}
					if (!player.isOnline()) {
						continue;
					}
					Database.getStatique().getPlayerData().update(player, true);
				}
				Console.println("-> Prismatic.", Console.Color.GAME);
				for (final Prism prism : World.getPrisms().values()) {
					if (World.getMap(prism.getMap()).getSubArea().getPrismId() != prism.getId()) {
						Database.getGame().getPrismeData().delete(prism.getId());
					} else {
						Database.getGame().getPrismeData().update(prism);
					}
				}
				Console.println("-> Mounts.", Console.Color.GAME);
				for (final Dragodinde mount : World.getMounts().values()) {
					if (mount.getId() > 0) {
						Database.getStatique().getMounts_dataData().update(mount, true);
					}
				}
				Console.println("-> Guilds.", Console.Color.GAME);
				for (final Guild guilde : World.getGuilds().values()) {
					Database.getGame().getGuildData().update(guilde);
				}
				Console.println("-> Guild members.", Console.Color.GAME);
				for (final Player player : World.getPlayers().values()) {
					if (player.isOnline() && player.getGuildMember() != null) {
						Database.getGame().getGuild_memberData().update(player);
					}
				}
				Console.println("-> Perceptor.", Console.Color.GAME);
				for (final Collector collector : World.getCollectors().values()) {
					if (collector.getInFight() <= 0) {
						Database.getGame().getPercepteurData().update(collector);
					}
				}
				Console.println("-> Houses.", Console.Color.GAME);
				for (final House house : World.getHouses().values()) {
					if (house.getOwnerId() > 0) {
						Database.getGame().getHouseData().update(house);
					}
				}
				Console.println("-> Chests.", Console.Color.GAME);
				for (final Trunk trunk : World.getTrunks().values()) {
					Database.getGame().getCoffreData().update(trunk);
				}
				Console.println("-> Mountparks.", Console.Color.GAME);
				for (final MountPark mp : World.getMountparks().values()) {
					if (mp.getOwner() > 0 || mp.getOwner() == -1) {
						Database.getGame().getMountpark_dataData().update(mp);
					}
				}
				Console.println("-> Areas.", Console.Color.GAME);
				for (final World.Area area : World.getAreas().values()) {
					Database.getGame().getArea_dataData().update(area);
				}
				for (final World.SubArea subarea : World.getSubAreas().values()) {
					Database.getGame().getSubarea_dataData().update(subarea);
				}
				Console.println("Saving done!", Console.Color.GAME);
				SocketManager.GAME_SEND_Im_PACKET_TO_ALL("1165;");
			} catch (Exception exception) {
				exception.printStackTrace();
				Console.println("Backup Error: " + exception.getMessage(), Console.Color.ERROR);
				if (trys < 10) {
					Console.println("Failed to save. Restart number " + (trys + 1) + ".",
							Console.Color.ERROR);
					cast(trys + 1);
					return;
				}
				Console.println("Too much backup failed.", Console.Color.ERROR);
				Main.isSaving = false;
				break Label_0913;
			} finally {
				Main.isSaving = false;
			}
			Main.isSaving = false;
		}
		GameServer.setState(1);
		Console.refreshTitle();
	}
}
