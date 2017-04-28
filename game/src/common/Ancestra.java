package common;

import game.GameServer;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintStream;
import java.io.UnsupportedEncodingException;
import java.net.InetAddress;
import java.util.ArrayList;
import java.util.Calendar;

import communication.ComServer;

public class Ancestra {
	
	/** RealmServer **/
	public static String REALM_IP;
	public static String REALM_DB_HOST;
	public static String REALM_DB_NAME;
	public static String REALM_DB_USER;
	public static String REALM_DB_PASS;
	/** GameServer **/
	public static GameServer gameServer;
	public static String AUTH_KEY;
	private static final String CONFIG_FILE = "game.conf";
	public static boolean CONFIG_USE_IP = false;
	public static String IP = "127.0.0.1";
	public static String GAMESERVER_IP;
	public static int CONFIG_GAME_PORT = 5555;
	public static boolean isInit = false;
	public static boolean isRunning = false;
	public static boolean isSaving = false;
	public static boolean CONFIG_DEBUG = false;
	//SQL
	public static String DB_HOST;
	public static String DB_USER;
	public static String DB_PASS;
	public static String DB_NAME;
	//Timer
	public static long FLOOD_TIME = 1*60000;
	public static int CONFIG_SAVE_TIME = 30*60000;
	public static int CONFIG_LOAD_DELAY = 10*60000;
	public static int CONFIG_RELOAD_MOB_DELAY = 300*60000;
	//Message
	public static String CONFIG_MOTD = "";
	public static String CONFIG_MOTD_COLOR = "";
	//Nombre de joueurs max
	public static int CONFIG_PLAYER_LIMIT = 30;
	//Configuration
	public static int CONFIG_MAX_PERSOS = 5;
	public static short CONFIG_START_MAP = 10298;
	public static int CONFIG_START_CELL = 314;
	public static int CONFIG_START_LEVEL = 1;
	public static int CONFIG_START_KAMAS = 0;
	public static boolean CONFIG_ALLOW_MULE_PVP = false;
	public static boolean CONFIG_AURA_SYSTEM = false;
	public static boolean CONFIG_ZAAP = false;
	public static boolean CONFIG_CUSTOM_STARTMAP;
	public static boolean CONFIG_USE_MOBS = false;
	public static int CONFIG_LVL_PVP = 15;
	//Rate Xp/Kamas/Drop/Honneur
	public static int RATE_DROP = 1;
	public static int RATE_KAMAS = 1;
	public static int RATE_HONOR = 1;
	public static int RATE_PVM = 1;
	public static int RATE_PVP = 1;
	public static int RATE_METIER = 1;
	//Arene
	public static ArrayList<Integer> arenaMap = new ArrayList<Integer>(8);
	public static int CONFIG_ARENA_TIMER = 10*60*1000;// 10 minutes
	//BDD
	public static int CONFIG_DB_COMMIT = 30*1000;
	//Inactivité
	public static int CONFIG_MAX_IDLE_TIME = 1800000;//in milliseconds
	//HDV
	public static ArrayList<Integer> NOTINHDV = new ArrayList<Integer>();
	//Abonnement
	public static boolean USE_SUBSCRIBE = false;
	/** ComServer **/
	public static ComServer comServer;
	public static int COM_PORT = -1;
	public static int com_Try = 0;
	public static boolean com_Running = false;
	/** Logs **/
	public static BufferedWriter Log_GameSock;
	public static BufferedWriter Log_Game;
	public static BufferedWriter Log_MJ;
	public static BufferedWriter Log_Shop;
	public static PrintStream Log_Errors;
	public static boolean canLog;
	
	public static void main(String[] args)
	{
		Runtime.getRuntime().addShutdownHook(new Thread()
		{
			public void run()
			{
				Ancestra.closeServers();
			}
		}
		);
		
		try {
	         System.setOut(new PrintStream(System.out, true, "IBM850"));
		} catch (final UnsupportedEncodingException e)
		{
		         e.printStackTrace();
		}
		
		System.out.println("==============================================================\n\n");
		System.out.println(makeHeader());
		System.out.println("==============================================================\n");
		System.out.print("Chargement de la configuration : ");
		loadConfiguration();
		isInit = true;
		System.out.println("Configuration OK!");
		System.out.print("Connexion a la base de donnee : ");
		if(SQLManager.setUpConnexion()) System.out.println("Connexion OK!");
		else
		{
			System.out.println("Connexion echouee!");
			System.exit(0);
		}
		System.out.println("Utilisation des monstres : "+CONFIG_USE_MOBS);
		
		System.out.println("\n");
		System.out.println("Creation du Monde :");
		long startTime = System.currentTimeMillis();
		World.createWorld();
		long endTime = System.currentTimeMillis();
		long differenceTime = (endTime - startTime)/1000;
		System.out.println("Monde OK! en : "+differenceTime+" s");
		isRunning = true;
		
		System.out.println("\n");
		System.out.print("Creation du GameServer sur le port "+CONFIG_GAME_PORT);
		String Ip = "";
		try
		{
			Ip = InetAddress.getLocalHost().getHostAddress();
		}
		catch(Exception e)
		{
			System.out.println(e.getMessage());
			try
			{
				Thread.sleep(10000);
			}
			catch (InterruptedException e1)
			{
				
			}
			System.exit(1);
		}
		
		Ip = IP;
		gameServer = new GameServer(Ip);
		System.out.println(" : GameServer OK!");
		if(CONFIG_USE_IP)
			System.out.println("Ip du serveur "+IP+" crypt "+GAMESERVER_IP);
		
		System.out.print("Creation du ComServer sur le port "+Ancestra.COM_PORT);
		comServer = new ComServer();
		System.out.println(" : ComServer OK!");
		
		System.out.println("\n\nEn attente de connexions");
	}
	
	private static void loadConfiguration()
	{
		boolean log = false;
		try
		{
			BufferedReader config = new BufferedReader(new FileReader(CONFIG_FILE));
			String line = "";
			while ((line=config.readLine())!=null)
			{
				if(line.split("=").length == 1)
				{
					continue;
				}
				
				// string var e.g. REALM_IP
				String param = line.split("=")[0].trim();
				// config value e.g. 127.0.0.1
				String value = line.split("=")[1].trim();
				
				if(param.equalsIgnoreCase("LogonServer.Ip"))
				{
					Ancestra.REALM_IP = value;
				}
				else if(param.equalsIgnoreCase("LogonServer.GameServerComPort"))
				{
					Ancestra.COM_PORT = Integer.parseInt(value);
				}
				else if(param.equalsIgnoreCase("LogonServer.Key"))
				{
					Ancestra.AUTH_KEY = value;
				}
				else if(param.equalsIgnoreCase("LogonServer.DBHost"))
				{
					Ancestra.REALM_DB_HOST = value;
				}
				else if(param.equalsIgnoreCase("LogonServer.DBUser"))
				{
					Ancestra.REALM_DB_USER = value;
				}
				else if(param.equalsIgnoreCase("LogonServer.DBPass"))
				{
					if(value == null)
					{
						value = "";
					}
					
					Ancestra.REALM_DB_PASS = value;
				}
				else if(param.equalsIgnoreCase("LogonServer.DBName"))
				{
					Ancestra.REALM_DB_NAME = value;
				}
				else if(param.equalsIgnoreCase("EnableDebug"))
				{
					if(value.equalsIgnoreCase("true"))
					{
						Ancestra.CONFIG_DEBUG = true;
					}
				}
				else if(param.equalsIgnoreCase("EnableLog"))
				{
					if(value.equalsIgnoreCase("true"))
					{
						log = true;
					}
				}
				else if(param.equalsIgnoreCase("GameServer.Ip"))
				{
					Ancestra.IP = value;
				}
				else if(param.equalsIgnoreCase("GameServer.PrintIpHash"))
				{
					if(value.equalsIgnoreCase("true"))
					{
						Ancestra.CONFIG_USE_IP = true;
					}
				}
				else if(param.equalsIgnoreCase("GameServer.Port"))
				{
					Ancestra.CONFIG_GAME_PORT = Integer.parseInt(value);
				}
				else if(param.equalsIgnoreCase("GameServer.DBHost"))
				{
					Ancestra.DB_HOST = value;
				}
				else if(param.equalsIgnoreCase("GameServer.DBUser"))
				{
					Ancestra.DB_USER = value;
				}
				else if(param.equalsIgnoreCase("GameServer.DBPass"))
				{
					if(value == null)
					{
						value = "";
					}
					
					Ancestra.DB_PASS = value;
				}
				else if(param.equalsIgnoreCase("GameServer.DBName"))
				{
					Ancestra.DB_NAME = value;
				}
				else if(param.equalsIgnoreCase("Motd.HtmlString "))
				{
					Ancestra.CONFIG_MOTD = line.split("=",2)[1];
				}
				else if(param.equalsIgnoreCase("Motd.HtmlColorCode"))
				{
					Ancestra.CONFIG_MOTD_COLOR = value;
				}
				else if (param.equalsIgnoreCase("General.PlayerLimit"))
				{
					Ancestra.CONFIG_PLAYER_LIMIT = Integer.parseInt(value);
				}
				else if (param.equalsIgnoreCase("General.ReloadActionsInMinutes"))
				{
					Ancestra.CONFIG_LOAD_DELAY = (Integer.parseInt(value)*60000);
				}
				else if(param.equalsIgnoreCase("General.SavePeriodInMinutes"))
				{
					Ancestra.CONFIG_SAVE_TIME = (Integer.parseInt(value)*60000);
				}
				else if (param.equalsIgnoreCase("General.EnableSubscriberSystem"))
				{
					Ancestra.USE_SUBSCRIBE = value.equalsIgnoreCase("true");
				}
				else if(param.equalsIgnoreCase("General.CharacterLimitOnAccount"))
				{
					Ancestra.CONFIG_MAX_PERSOS = Integer.parseInt(value);
				}
				else if(param.equalsIgnoreCase("General.EnableAllZaap"))
				{
					if(value.equalsIgnoreCase("true"))
					{
						Ancestra.CONFIG_ZAAP = true;
					}
				}
				else if(param.equalsIgnoreCase("General.MaxLvlDiffForPvpHonor"))
				{
					Ancestra.CONFIG_LVL_PVP = Integer.parseInt(value);
				}
				else if(param.equalsIgnoreCase("General.EnableSameIpAggress"))
				{
					Ancestra.CONFIG_ALLOW_MULE_PVP = value.equalsIgnoreCase("true");
				}
				else if (param.equalsIgnoreCase("General.DisplayLvlAura"))
				{
					Ancestra.CONFIG_AURA_SYSTEM = value.equalsIgnoreCase("true");
				}
				else if (param.equalsIgnoreCase("General.IdleTimeInMinBeforeKick"))
				{
					Ancestra.CONFIG_MAX_IDLE_TIME = (Integer.parseInt(value)*60000);
				}
				else if (param.equalsIgnoreCase("General.BannedItemsForAuction"))
				{
					for(String curID : value.split(","))
					{
						Ancestra.NOTINHDV.add(Integer.parseInt(curID));
					}
				}
				else if (param.equalsIgnoreCase("General.SpawnCityGuards"))
				{
					Ancestra.CONFIG_USE_MOBS = value.equalsIgnoreCase("true");
				}
				else if(param.equalsIgnoreCase("PlayerStart.EnableCustom"))
				{
					if(value.equalsIgnoreCase("true"))
					{
						Ancestra.CONFIG_CUSTOM_STARTMAP = true;
					}
				}
				else if(param.equalsIgnoreCase("PlayerStart.Map"))
				{
					Ancestra.CONFIG_START_MAP = Short.parseShort(value);
				}
				else if(param.equalsIgnoreCase("PlayerStart.Cell"))
				{
					Ancestra.CONFIG_START_CELL = Integer.parseInt(value);
				}
				else if(param.equalsIgnoreCase("PlayerStart.Level"))
				{
					Ancestra.CONFIG_START_LEVEL = Integer.parseInt(value);
					if(Ancestra.CONFIG_START_LEVEL < 1)
					{
						Ancestra.CONFIG_START_LEVEL = 1;
					}
					
					if(Ancestra.CONFIG_START_LEVEL > 200)
					{
						Ancestra.CONFIG_START_LEVEL = 200;
					}
				}
				else if(param.equalsIgnoreCase("PlayerStart.Kamas"))
				{
					Ancestra.CONFIG_START_KAMAS = Integer.parseInt(value);
					if(Ancestra.CONFIG_START_KAMAS < 0)
					{
						Ancestra.CONFIG_START_KAMAS = 0;
					}
					
					if(Ancestra.CONFIG_START_KAMAS > 1000000000)
					{
						Ancestra.CONFIG_START_KAMAS = 1000000000;
					}
				}
				else if(param.equalsIgnoreCase("Rate.PvpXp"))
				{
					Ancestra.RATE_PVP = Integer.parseInt(value);
				}
				else if(param.equalsIgnoreCase("Rate.JobXp"))
				{
					Ancestra.RATE_METIER = Integer.parseInt(value);
				}
				else if(param.equalsIgnoreCase("Rate.PvmXp"))
				{
					Ancestra.RATE_PVM = Integer.parseInt(value);
				}
				else if(param.equalsIgnoreCase("Rate.Drops"))
				{
					Ancestra.RATE_DROP = Integer.parseInt(value);
				}
				else if(param.equalsIgnoreCase("Rate.Kamas"))
				{
					Ancestra.RATE_KAMAS = Integer.parseInt(value);
				}
				else if(param.equalsIgnoreCase("Rate.Honor"))
				{
					Ancestra.RATE_HONOR = Integer.parseInt(value);
				}
				else if (param.equalsIgnoreCase("Arena.Maps"))
				{
					for(String curID : value.split(","))
					{
						Ancestra.arenaMap.add(Integer.parseInt(curID));
					}
				}
				else if (param.equalsIgnoreCase("Arena.PrivateMobSpawnTimer"))
				{
					Ancestra.CONFIG_ARENA_TIMER = (Integer.parseInt(value)*60000);
				}
			}
			
			// check valid db values
			if(REALM_IP == null || REALM_DB_HOST == null || REALM_DB_NAME == null || REALM_DB_USER == null || REALM_DB_PASS == null)
			{
				System.out.println("One or more RealmDatabase values are missing!");
				config.close();
				throw new Exception();
			}
			if(AUTH_KEY == null || COM_PORT == -1)
			{
				System.out.println("One or more RealmConnection values are missing!");
				config.close();
				throw new Exception();
			}
			if(DB_NAME == null || DB_HOST == null || DB_PASS == null || DB_USER == null)
			{
				System.out.println("One or more GameDatabase values are missing!");
				config.close();
				throw new Exception();
			}
				
			config.close();
		}
		catch(Exception e)
		{
			System.out.println(e.getMessage());
			System.out.println("Server is shutting down.");
			System.exit(1);
		}
		
		if(CONFIG_DEBUG)
		{
			Constants.DEBUG_MAP_LIMIT = 20000;
		}
		
		try
		{
			String date = Calendar.getInstance().get(Calendar.DAY_OF_MONTH)+"-"+(Calendar.getInstance().get(Calendar.MONTH)+1)+"-"+Calendar.getInstance().get(Calendar.YEAR);
			if(log)
			{
				if(!(new File("Game_logs")).exists()) 
				{
					new File("Game_logs").mkdir();
				}
				if(!(new File("Error_logs")).exists())
				{
					new File("Error_logs").mkdir();
				}
				if(!(new File("Shop_logs")).exists())
				{
					new File("Shop_logs").mkdir();
				}
				if(!(new File("Gms_logs")).exists())
				{
					new File("Gms_logs").mkdir();
				}
				Log_Game = new BufferedWriter(new FileWriter("Game_logs/"+date+".txt", true));
				Log_GameSock = new BufferedWriter(new FileWriter("Game_logs/"+date+"_packets.txt", true));
				Log_Shop = new BufferedWriter(new FileWriter("Shop_logs/"+date+".txt", true));
				Log_MJ = new BufferedWriter(new FileWriter("Gms_logs/"+date+"_GM.txt",true));
				Log_Errors = new PrintStream(new File("Error_logs/"+date+"_error.txt"));
				
				Log_Errors.append("Lancement du serveur..\n");
				Log_Errors.flush();
				System.setErr(Log_Errors);
				canLog = true;
				
				String str = "Lancement du serveur...\n";
				
				Log_GameSock.write(str);
				Log_Game.write(str);
				Log_MJ.write(str);
				Log_Shop.write(str);
				Log_GameSock.flush();
				Log_Game.flush();
				Log_MJ.flush();
				Log_Shop.flush();
			}
		}
		catch(IOException e)
		{
			System.out.println("La creation des logs a echouee !");
			System.out.println(e.getMessage());
			System.exit(1);
		}
	}
	
	public static void closeServers()
	{
		System.out.println("Arret du serveur demande ...");
		if(isRunning)
		{
			isRunning = false;
			try
			{
				gameServer.kickAll();
			}catch(Exception e)
			{
				System.out.println(e.getMessage());
			}
			
			World.saveAll(null);
			SQLManager.closeCons();
		}
		System.out.println("Arret du serveur: OK");
		isRunning = false;
	}

	public static void addToMjLog(String str)
	{
		if(!canLog)return;
		String date = Calendar.getInstance().get(Calendar.HOUR_OF_DAY)+":"+Calendar.getInstance().get(+Calendar.MINUTE)+":"+Calendar.getInstance().get(Calendar.SECOND);
		try {
			Log_MJ.write("["+date+"]"+str);
			Log_MJ.newLine();
			Log_MJ.flush();
		} catch (IOException e) {}
	}
	
	public static void addToShopLog(String str)
	{
		if(!canLog)return;
		String date = Calendar.getInstance().get(Calendar.HOUR_OF_DAY)+":"+Calendar.getInstance().get(+Calendar.MINUTE)+":"+Calendar.getInstance().get(Calendar.SECOND);
		try {
			Log_Shop.write("["+date+"]"+str);
			Log_Shop.newLine();
			Log_Shop.flush();
		} catch (IOException e) {}
	}
	
	public static String makeHeader()
	{
		StringBuilder mess = new StringBuilder();
		mess.append("Ancestra-R Game v"+Constants.SERVER_VERSION);
		mess.append("\nPar DeathDown & DeeZ pour Dofus "+Constants.CLIENT_VERSION);
		mess.append("\nThanks Diabu.");
		mess.append("\nhttp://sourceforge.net/projects/ancestrar/\n\n");
		return mess.toString();
	}
	
	public static void try_ComServer()
	{
		if(com_Try == 0 && isRunning)
		{
			try {
				System.out.print("Creation d'une nouvelle connexion avec le Realm (ComServer) ... ");
				com_Try = 1;
				while(Ancestra.com_Running == false && isRunning)
				{
					comServer = new ComServer();
					Thread.sleep(10000);
				}
				System.out.println("ComServer de nouveau operationnel !");
				com_Try = 0;
			} catch (InterruptedException e) {}
		}
	}
}
