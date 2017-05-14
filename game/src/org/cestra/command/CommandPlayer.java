package org.cestra.command;

import java.text.SimpleDateFormat;
import java.util.Date;

import org.cestra.client.Player;
import org.cestra.common.SocketManager;
import org.cestra.game.GameClient;
import org.cestra.game.world.World;
import org.cestra.kernel.Boutique;
import org.cestra.kernel.Config;
import org.cestra.kernel.Main;

public class CommandPlayer {
	private static String canal;

	static {
		CommandPlayer.canal = "All";
	}

	public static boolean analyse(final Player perso, final String msg) {
		if (msg.charAt(0) != '.' || msg.charAt(1) == '.') {
			return false;
		}
		if(msg.length() > 7 && msg.substring(1, 7).equalsIgnoreCase("parcho")) {
			int points = perso.getAccount().getPoints() - 250;
			if(points < 0) {
				perso.sendMessage("You need <b>" + (250 - perso.getAccount().getPoints()) + "</b> points to complete this purchase.");
				return true;
			}
			
			Player player = perso;
			player.getStatsParcho().getMap().clear();
			player.getStatsParcho().addOneStat(125, 101);
			player.getStatsParcho().addOneStat(124, 101);
			player.getStatsParcho().addOneStat(118, 101);
			player.getStatsParcho().addOneStat(126, 101);
			player.getStatsParcho().addOneStat(119, 101);
			player.getStatsParcho().addOneStat(123, 101);
			
			SocketManager.GAME_SEND_STATS_PACKET(player);
			player.sendMessage("You are now parcho 101 in all elements.");
			perso.getAccount().setPoints(points);
			return true;
		} 
		
		if(msg.length() > 7 && msg.substring(1, 7).equalsIgnoreCase("restat")) {
			int points = perso.getAccount().getPoints() - 180;
			if(points < 0) {
				perso.sendMessage("You need <b>" + (180 - perso.getAccount().getPoints()) + "</b> points to make this purchase.");
				return true;
			}
			
			perso.getStatsParcho().getMap().clear();
			perso.getStats().addOneStat(125,-perso.getStats().getEffect(125));
			perso.getStats().addOneStat(124,-perso.getStats().getEffect(124));
			perso.getStats().addOneStat(118,-perso.getStats().getEffect(118));
			perso.getStats().addOneStat(123,-perso.getStats().getEffect(123));
			perso.getStats().addOneStat(119,-perso.getStats().getEffect(119));
			perso.getStats().addOneStat(126,-perso.getStats().getEffect(126));
			perso.setCapital((perso.getLevel() * 5) - 5);
			SocketManager.GAME_SEND_STATS_PACKET(perso);
			SocketManager.GAME_SEND_Im_PACKET(perso,"023;" + (perso.getLevel() * 5 - 5));
			return true;
		} 
		
		if(msg.length() > 4 && msg.substring(1, 5).equalsIgnoreCase("obvi")) {
			int points = perso.getAccount().getPoints() - 210;
			if(points < 0) {
				perso.sendMessage("You need <b>" + (210 - perso.getAccount().getPoints()) + "</b> points to complete this purchase.");
			} else {
				org.cestra.object.Object object = World.getObjTemplate(9233).createNewItem(1, false);
				org.cestra.object.Object object2 = World.getObjTemplate(9234).createNewItem(1, false);
				if(perso.addObjet(object,true))
					World.addObjet(object, true);
				
				if(perso.addObjet(object2,true))
					World.addObjet(object2, true);
				perso.sendMessage("You have just received your living objects successfully.");
				perso.getAccount().setPoints(points);
			}
			return true;
		} 
		
		
		
		if(msg.length() > 6 && msg.substring(1, 7).equalsIgnoreCase("points")) {
			perso.sendMessage("You have <b>" + perso.getAccount().getPoints() + "</b> shop points.");
			return true;
		} 
		
		
		if(msg.length() > 8 && msg.substring(1, 9).equalsIgnoreCase("boutique")) {
			Boutique.open(perso);
			return true;
		} 
		if (msg.length() > 3 && msg.substring(1, 4).equalsIgnoreCase("all")) {
			if (perso.noall) {
				SocketManager.GAME_SEND_MESSAGE(perso,
						"Your channel " + CommandPlayer.canal + " is disabled.", "C35617");
				return true;
			}
			final String prefix = "[" + new SimpleDateFormat("HH:mm").format(new Date(System.currentTimeMillis()))
					+ "] (" + CommandPlayer.canal + ") <b><a href='asfunction:onHref,ShowPlayerPopupMenu,"
					+ perso.getName() + "'>" + perso.getName() + "</a></b> : ";
			for (final Player p : World.getOnlinePersos()) {
				if (!p.noall) {
					SocketManager.GAME_SEND_MESSAGE(p, String.valueOf(prefix) + msg.substring(5, msg.length() - 1),
							"C35617");
				}
			}
			return true;
		} else {
			if (msg.length() > 5 && msg.substring(1, 6).equalsIgnoreCase("noall")) {
				if (perso.noall) {
					perso.noall = false;
					SocketManager.GAME_SEND_MESSAGE(perso,
							"You have activated the channel " + CommandPlayer.canal + ".", "C35617");
				} else {
					perso.noall = true;
					SocketManager.GAME_SEND_MESSAGE(perso,
							"You have disabled the channel " + CommandPlayer.canal + ".", "C35617");
				}
				return true;
			}
			if (msg.length() > 5 && msg.substring(1, 6).equalsIgnoreCase("staff")) {
				String message = "staff list :";
				boolean vide = true;
				for (final GameClient client : Main.gameServer.getClients().values()) {
					final Player player = client.getPersonnage();
					if (player == null) {
						continue;
					}
					if (player.getGroupe() == null) {
						continue;
					}
					if (player.isInvisible()) {
						continue;
					}
					message = String.valueOf(message) + "\n- <b><a href='asfunction:onHref,ShowPlayerPopupMenu,"
							+ player.getName() + "'>[" + player.getGroupe().getNom() + "] " + player.getName()
							+ "</a></b>";
					vide = false;
				}
				if (vide) {
					message = "No staff online";
				}
				SocketManager.GAME_SEND_MESSAGE(perso, message);
				return true;
			}
			if (msg.length() > 5 && msg.substring(1, 6).equalsIgnoreCase("debug")) {
				if (perso.isInPrison()) {
					return true;
				}
				if (perso.cantTP()) {
					return true;
				}
				if (System.currentTimeMillis() - perso.restriction.timeDeblo < Config.getInstance().delayDeblo) {
					SocketManager.GAME_SEND_MESSAGE(perso,
							"You still have to wait "
									+ (Config.getInstance().delayDeblo / 60000L
											- (System.currentTimeMillis() - perso.restriction.timeDeblo) / 60L / 1000L)
									+ " minutes.");
					return true;
				}
				if (perso.get_fight() != null) {
					return true;
				}
				perso.setTimeDeblo(System.currentTimeMillis());
				perso.teleport((short) 7411, 311);
				return true;
			} else {
				if (msg.length() > 5 && msg.substring(1, 6).equalsIgnoreCase("infos")) {
					long uptime = System.currentTimeMillis() - Config.getInstance().startTime;
					final int jour = (int) (uptime / 86400000L);
					uptime %= 86400000L;
					final int hour = (int) (uptime / 3600000L);
					uptime %= 3600000L;
					final int min = (int) (uptime / 60000L);
					uptime %= 60000L;
					final int sec = (int) (uptime / 1000L);
					final int nbPlayer = Main.gameServer.getPlayerNumber();
					final int nbPlayerIp = Main.gameServer.getPlayerNumberByIp();
					final int maxPlayer = Main.gameServer.getMaxPlayer();
					String mess = "<b>" + Config.getInstance().NAME + "</b>\n" + "Uptime : " + jour + "j " + hour + "h "
							+ min + "m " + sec + "s.";
					if (nbPlayer > 0) {
						mess = String.valueOf(mess) + "\nPlayers Online: " + nbPlayer;
					}
					if (nbPlayerIp > 0) {
						mess = String.valueOf(mess) + "\nUnique Players Online: " + nbPlayerIp;
					}
					if (maxPlayer > 0) {
						mess = String.valueOf(mess) + "\nRecord Connection: " + maxPlayer;
					}
					SocketManager.GAME_SEND_MESSAGE(perso, mess);
					return true;
				}
				SocketManager.GAME_SEND_MESSAGE(perso,"The available commands are:\n<b>.infos</b> - Provides information about the server.\n<b>.debug</b> - Allows you to block yourself by taking you to the zaap of Astrub.\n<b>.staff</b> - Allows to see the connected staff members.\n<b>.all</b> - Sends a message to all players.\n<b>.noall</b> - Allows you to stop receiving messages from the .all channel.\n<b>.parcho</b> <b>.restat</b> <b>.obvi</b> <b>.points</b> <b>.boutique</b> <b>.boutique</b>");
				return true;
			}
		}
	}
}
