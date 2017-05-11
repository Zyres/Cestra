/*
 * Decompiled with CFR 0_114.
 */
package org.aestia.kernel;

import java.io.File;
import java.util.ArrayList;

import com.typesafe.config.ConfigFactory;

public class Config
{
	private static final com.typesafe.config.Config configFile;
	
	public static final Config singleton = new Config();
	
	//TODO HEROIC
	public static boolean HEROIC = false;
	public static boolean HALLOWEEN = false;
	public static boolean NOEL = false;
	
	public static final int RAM = 600;
	public final long startTime = System.currentTimeMillis();
	
	public static String NAME;
	public static String startMessage = "";
	public static String colorMessage = "B9121B";
	
	public static boolean kickIfAfk = false;
	public static boolean autoReboot = false;
	public static boolean allZaap = false;
	public static boolean allEmote = false;
	
	public static boolean onlyLocal = false;
	
	public static int startMap = 0;
	public static int startCell = 0;
	
	public static int rateXp = 1;
	public static int rateKamas = 1;
	public static int rateDrop = 1;
	public static int rateHonor = 1;
	public static int rateJob = 1;
	public static int rateFm = 1;
	
	//Broadcast
	public static String PUB1 = "";
	public static String PUB2 = "";
	public static String PUB3 = "";
	public static String PUB4 = "";
	
	public long delayDeblo = 3600000;
	public static int config = 1;
	public String script = "";
	public static ArrayList<Integer> notInHdv = new ArrayList<Integer>();
	public static ArrayList<Integer> arenaMap = new ArrayList<Integer>();
	public static ArrayList<Integer> itemFeedMount = new ArrayList<>();

	static
    {
        configFile = ConfigFactory.parseFile(new File("game.conf"));
    }
	
	public static Config getInstance()
	{
		return singleton;
	}

	public void set()
	{
		Main.isRunning = true;
		
		try
		{
			Main.hostDB = Config.configFile.getString("Game.Static.Database.Ip");
			Main.nameDB = Config.configFile.getString("Game.Dynamic.Database.Name");
			Main.userDB = Config.configFile.getString("Game.Static.Database.User");
			Main.passDB = Config.configFile.getString("Game.Static.Database.Pass");
			
			Main.loginHostDB = Config.configFile.getString("Game.Dynamic.Database.Ip");
			Main.loginNameDB = Config.configFile.getString("Game.Static.Database.Name");
			Main.loginUserDB = Config.configFile.getString("Game.Dynamic.Database.User");
			Main.loginPassDB = Config.configFile.getString("Game.Dynamic.Database.Pass");
			
			Main.Ip = Config.configFile.getString("Game.Server.Ip");
			Main.gamePort = Config.configFile.getInt("Game.Server.Port");
			
			Config.NAME = Config.configFile.getString("Game.Server.Name");
			Config.HEROIC = Config.configFile.getBoolean("Game.Server.Heroic");
			Config.HALLOWEEN = Config.configFile.getBoolean("Game.Server.Halloween");
			Config.NOEL = Config.configFile.getBoolean("Game.Server.Christmas");
			
			Config.startMessage = Config.configFile.getString("Game.Server.Welcome.Message");
			Config.colorMessage = Config.configFile.getString("Game.Server.Welcome.Color");
			
			Config.kickIfAfk = Config.configFile.getBoolean("Game.Enable.KickAFKPlayer");
			Config.autoReboot = Config.configFile.getBoolean("Game.Enable.AutoReboot");
			Config.allZaap = Config.configFile.getBoolean("Game.Enable.AllZaap");
			Config.allEmote = Config.configFile.getBoolean("Game.Enable.AllEmote");
			
			Main.exchangeIp = Config.configFile.getString("Logon.RealmList.Ip");
			Main.exchangePort = Config.configFile.getInt("Logon.RealmList.Port");
			Main.key = Config.configFile.getString("Logon.RealmList.Key");
			
			Config.PUB1 = Config.configFile.getString("Game.Broadcast.Message1");
			Config.PUB2 = Config.configFile.getString("Game.Broadcast.Message2");
			Config.PUB3 = Config.configFile.getString("Game.Broadcast.Message3");
			Config.PUB4 = Config.configFile.getString("Game.Broadcast.Message4");
			
			//Custom
			Main.startLevel = Config.configFile.getInt("Start.Level");
			Main.startKamas = Config.configFile.getInt("Start.Kamas");
			Config.startMap = Config.configFile.getInt("Start.Map");
			Config.startCell = Config.configFile.getInt("Start.Cell");
			
			
			Config.rateXp = Config.configFile.getInt("Rate.Xp");
			Config.rateDrop = Config.configFile.getInt("Rate.Drop");
			Config.rateFm  = Config.configFile.getInt("Rate.Reforge");
			Config.rateHonor = Config.configFile.getInt("Rate.Honor");
			Config.rateJob = Config.configFile.getInt("Rate.Job");
			Config.rateKamas = Config.configFile.getInt("Rate.Kamas");
        }
        catch (Exception e) 
        {
            System.out.println(" <> Config illisible ou champs manquants: " + e.getMessage());
            System.exit(1);
        }
		
	}

	public void load()
	{
		set();
	}

	public static void loadOption()
	{
		String map = "10131,10132,10133,10134,10135,10136,10137,10138";
		String[] arrstring = map.split("\\,");
		int n = arrstring.length;
		int n2 = 0;
		while (n2 < n)
		{
			String i = arrstring[n2];
			arenaMap.add(Integer.parseInt(i));
			++n2;
		}
		
		notInHdv.add(4820);
		itemFeedMount.add(41);
	}

	public static boolean contains(ArrayList<Integer> list, int id0)
	{
		return list.contains(id0);
	}
}
