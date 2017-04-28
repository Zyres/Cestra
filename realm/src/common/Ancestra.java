package common;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Calendar;

import communication.ComServer;


import realm.RealmServer;

public class Ancestra
{
	private static final String CONFIG = "Realm_Config.txt";
	public static String REALM_VERSION = "0.3";
	public static int REALM_PORT = -1;
	public static int REALM_COM_PORT = -1;
	public static int REALM_TIME_DATA = 0;
	public static String REALM_DB_HOST = null;
	public static String REALM_DB_USER = null;
	public static String REALM_DB_PASSWORD = null;
	public static String REALM_DB_NAME = null;
	public static int REALM_DB_COMMIT = 30*1000;
	public static boolean REALM_DEBUG = false;
	public static boolean REALM_IGNORE_VERSION = false;
	public static String CLIENT_VERSION = "1.29.1";
	public static boolean isInit = false;
	public static boolean isRunning = false;
	public static boolean USE_SUBSCRIBE = false;
	/** LOGS **/
	public static BufferedWriter Log_Realm;
	public static BufferedWriter Log_Com;
	public static BufferedWriter Log_Errors;
	/** THREADS **/
	public static RealmServer realmServer;
	public static ComServer comServer;
	
	
	public static void main(String[] args) 
	{
		Runtime.getRuntime().addShutdownHook(new Thread() 
		{
			public void run() 
			{
				Ancestra.closeServers();
			}
		});
		
		System.out.println("==============================================================\n\n");
		System.out.println(makeHeader());
		System.out.println("==============================================================\n");
		System.out.print("Chargement de la configuration : ");
		loadConfiguration();
		System.out.println("Configuration OK!");
		Ancestra.isInit = true;
		System.out.print("Connexion a la base de donnees : ");
		if(SQLManager.setUpConnexion())
		{
			System.out.println("Connexion OK!");
		}
		else 
		{
			System.out.println("Connexion echouee!");
			Ancestra.addToErrorLog("SQL : Connexion failed");
			System.exit(0);
		}
		Realm.loadRealm();
		System.out.print("\n\nCreation du RealmServer sur le port "+Ancestra.REALM_PORT);
		realmServer = new RealmServer();
		System.out.println(" : RealmServer OK!");
		System.out.print("Creation du ComServer sur le port "+Ancestra.REALM_COM_PORT);
		comServer = new ComServer();
		System.out.println(" : ComServer OK!");
		
		System.out.println("\n\nEn attente de connexions\n\n");
		addToRealmLog("Realm start : En attente de connexions");
		addToComLog("Com start : En attente de connexions");
	}
	
	private static void loadConfiguration() 
	{
		try
		{
			BufferedReader config = new BufferedReader(new FileReader(CONFIG));
			String line = "";
			while((line = config.readLine()) != null) 
			{
				if (line.split("=").length == 1)
					continue;
				
				// string var e.g. REALM_IP
				String param = line.split("=")[0].trim();
				// config value e.g. 127.0.0.1
				String value = line.split("=")[1].trim();
				
				if(param.equalsIgnoreCase("LogonServer.ListenClientPort")) 
				{
					try
					{
						Ancestra.REALM_PORT = Integer.parseInt(value);
					}
					catch(Exception e)
					{
						System.out.println("REALM_PORT doit etre un entier!");
						System.exit(1);
					}
				}
				else if(param.equalsIgnoreCase("LogonServer.ListenGameServerComPort"))
				{
					Ancestra.REALM_COM_PORT = Integer.parseInt(value);
				}
				else if(param.equalsIgnoreCase("LogonServer.IgnoreClientVersion"))
				{
					Ancestra.REALM_IGNORE_VERSION = (value.equalsIgnoreCase("true") ? true : false);
				}
				else if (param.equalsIgnoreCase("LogonServer.AllowedClientVersion"))
				{
					Ancestra.CLIENT_VERSION = value;
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
						Ancestra.REALM_DB_PASSWORD = "";
					else
						Ancestra.REALM_DB_PASSWORD = value;
				}
				else if(param.equalsIgnoreCase("LogonServer.DBName")) 
				{
					Ancestra.REALM_DB_NAME = value;
				}
				else if(param.equalsIgnoreCase("LogonServer.DBConnectionTimer")) 
				{
					Ancestra.REALM_DB_COMMIT = Integer.parseInt(value);
				}
				else if(param.equalsIgnoreCase("EnableDebug"))
				{
					Ancestra.REALM_DEBUG = (value.equalsIgnoreCase("true") ? true : false);
				}
				else if(param.equalsIgnoreCase("General.EnableSubscriberSystem"))
				{
					Ancestra.USE_SUBSCRIBE = (value.equalsIgnoreCase("true") ? true : false);
				}
			}
			
			if (REALM_DB_NAME == null || REALM_DB_HOST == null || REALM_DB_PASSWORD == null || REALM_DB_USER == null || REALM_PORT == -1 || REALM_COM_PORT == -1) 
			{
				config.close();
				throw new Exception();
			}
			config.close();
			
		}
		catch(Exception e)
		{
            System.out.println(e.getMessage());
			System.out.println("Fichier de configuration non existant ou illisible !");
			System.out.println("Fermeture du serveur de connexion.");
			System.exit(1);
		}
		
		try
		{
			String date = Calendar.getInstance().get(Calendar.DAY_OF_MONTH)+"-"+(Calendar.getInstance().get(Calendar.MONTH) + 1)+"-"+Calendar.getInstance().get(Calendar.YEAR);
			if(!(new File("Realm_logs")).exists()) 
			{
				new File("Realm_logs").mkdir();
			}
			if(!(new File("Error_logs")).exists())
			{
				new File("Error_logs").mkdir();
			}
			if(!(new File("Com_logs")).exists())
			{
				new File("Com_logs").mkdir();
			}
			Log_Realm = new BufferedWriter(new FileWriter("Realm_logs/"+date+".txt", true));
			Log_Com = new BufferedWriter(new FileWriter("Com_logs/"+date+".txt", true));
			Log_Errors = new BufferedWriter(new FileWriter("Error_logs/"+date+".txt", true));
		}catch(IOException e)
		{
			System.out.println("La creation des logs a echouee !");
			System.out.println(e.getMessage());
			System.exit(0);
		}
	}
	
	public synchronized static void addToErrorLog(String str)
	{
			try
			{
				String date = Calendar.HOUR_OF_DAY+":"+Calendar.MINUTE+":"+Calendar.SECOND;
				Ancestra.Log_Errors.write(date+": "+str);
				Ancestra.Log_Errors.newLine();
				Ancestra.Log_Errors.flush();
			}catch(Exception e)
			{
				System.out.println("L'ecriture des logs a echouee !");
			}
	}
	
	public synchronized static void addToRealmLog(String str) 
	{
		try
		{
			String date = Calendar.HOUR_OF_DAY+":"+Calendar.MINUTE+":"+Calendar.SECOND;
			Ancestra.Log_Realm.write(date+": "+str);
			Ancestra.Log_Realm.newLine();
			Ancestra.Log_Realm.flush();
		}catch(Exception e)
		{
			System.out.println("L'ecriture des logs a echouee !");
		}
	}
	
	public synchronized static void addToRealmSockLog(String str) 
	{
		try
		{
			String date = Calendar.HOUR_OF_DAY+":"+Calendar.MINUTE+":"+Calendar.SECOND;
			Ancestra.Log_Realm.write(date+": "+str);
			Ancestra.Log_Realm.newLine();
			Ancestra.Log_Realm.flush();
		}catch(Exception e)
		{
			System.out.println("L'ecriture des logs a echouee !");
		}
	}
	
	public synchronized static void addToComLog(String str)
	{
		try
		{
			String date = Calendar.HOUR_OF_DAY+":"+Calendar.MINUTE+":"+Calendar.SECOND;
			Ancestra.Log_Com.write(date+": "+str);
			Ancestra.Log_Com.newLine();
			Ancestra.Log_Com.flush();
		}catch(Exception e)
		{
			System.out.println("L'ecriture des logs a echouee !");
		}
	}
	
	public static void closeServers()
	{
		if (isRunning) 
		{
			addToRealmLog("Realm close : kick des connexions");
			isRunning = false;
			try
			{
				if(realmServer != null) realmServer.kickAll();
			}catch(Exception e)
			{
				System.out.println(e.getMessage());
			}
			addToComLog("Com close : kick des connexions");
			try
			{
				if(comServer != null) comServer.kickAll();
			}catch(Exception e)		
			{ 
				System.out.println(e.getMessage());
			}
		}
		isRunning = false;
		System.out.println("Arret du serveur de connexion : OK!");
	}
	
	public static String makeHeader()
	{
		StringBuilder mess = new StringBuilder();
		mess.append("Ancestar-R Realm v"+REALM_VERSION);
		mess.append("\nPar DeathDown pour Dofus "+CLIENT_VERSION);
		mess.append("\nhttp://sourceforge.net/projects/ancestrar/\n\n");
		return mess.toString();
	}
}