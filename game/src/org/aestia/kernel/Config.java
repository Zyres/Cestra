/*
 * Decompiled with CFR 0_114.
 */
package org.aestia.kernel;

import java.io.File;
import java.util.ArrayList;

import com.typesafe.config.ConfigFactory;

public class Config {
	private static final com.typesafe.config.Config configFile;
	
	//TODO HEROIC
	public static boolean HEROIC = false;
	public static final Config singleton = new Config();
	public static final int RAM = 600;
	public boolean HALLOWEEN = false;
	public boolean NOEL = false;
	public final long startTime = System.currentTimeMillis();
	public long delayDeblo = 3600000;
	public String NAME;
	public String startMessage = "";
	public String colorMessage = "B9121B";
	public boolean kickIfAfk = false;
	public boolean autoReboot = false;
	public boolean allZaap = false;
	public boolean allEmote = false;
	public boolean onlyLocal = false;
	public int startMap = 0;
	public int startCell = 0;
	public int rateXp = 1;
	public int rateKamas = 1;
	public static int config = 1;
	public int rateDrop = 1;
	public int rateHonor = 1;
	public int rateJob = 1;
	public int rateFm = 1;
	public String PUB1 = "(Message Auto) : ";
	public String PUB2 = "(Message Auto) : ";
	public String PUB3 = "(Message Auto) : ";
	public String PUB4 = "(Message Auto) : ";
	public String script = "";
	public static ArrayList<Integer> notInHdv = new ArrayList<Integer>();
	public static ArrayList<Integer> arenaMap = new ArrayList<Integer>();
	public static ArrayList<Integer> itemFeedMount = new ArrayList<>();

	static
    {
        configFile = ConfigFactory.parseFile(new File("config.properties"));
    }
	
	public static Config getInstance() {
		return singleton;
	}

	public void set()
	{
		Main.isRunning = true;
		Main.exchangePort = 451;
		
		Main.exchangeIp = "127.0.0.1";
		
		Main.loginHostDB = "127.0.0.1";
		Main.loginNameDB = "aestia_game";
		Main.loginUserDB = "root";
		Main.loginPassDB = "root";
		
		Main.hostDB = "127.0.0.1";
		Main.nameDB = "aesti_other";
		Main.userDB = "root";
		Main.passDB = "root";
		
		Main.gamePort = 5555;
		Main.Ip = "127.0.0.1";
		
		this.onlyLocal = false;
		
		Config.getInstance().NAME = "jiva";
		Config.getInstance().kickIfAfk = false;
		Config.getInstance().autoReboot = false;
		Config.getInstance().PUB1 = "(Message Auto) : message 1.";
		Config.getInstance().PUB2 = "(Message Auto) : message 2.";
		Config.getInstance().PUB3 = "(Message Auto) : message 3.";
		Config.getInstance().PUB4 = "(Message Auto) : message 4.";
		
		try
		{
			Main.hostDB = Config.configFile.getString("gameDbIp");
			Main.nameDB = Config.configFile.getString("gameDbName");
			Main.userDB = Config.configFile.getString("gameDbUser");
			Main.passDB = Config.configFile.getString("gameDbPass");
			
			Main.loginHostDB = Config.configFile.getString("dynamicDbHost");
			Main.loginNameDB = Config.configFile.getString("dynamicDbName");
			Main.loginUserDB = Config.configFile.getString("dynamicDbUser");
			Main.loginPassDB = Config.configFile.getString("dynamicDbPass");
			
			Main.exchangePort = Config.configFile.getInt("exchangePort");
			Main.exchangeIp = Config.configFile.getString("exchangeIp");
			
			Main.key = Config.configFile.getString("serverKey");
			
        }
        catch (Exception e) 
        {
            System.out.println(" <> Config illisible ou champs manquants: " + e.getMessage());
            System.exit(1);
        }
		
	}

	public void load() {
		set();
	}

	public static void loadOption() {
		String map = "10131,10132,10133,10134,10135,10136,10137,10138";
		String[] arrstring = map.split("\\,");
		int n = arrstring.length;
		int n2 = 0;
		while (n2 < n) {
			String i = arrstring[n2];
			arenaMap.add(Integer.parseInt(i));
			++n2;
		}
		notInHdv.add(4820);
		itemFeedMount.add(41);
	}

	public static boolean contains(ArrayList<Integer> list, int id0) {
		return list.contains(id0);
	}
}
