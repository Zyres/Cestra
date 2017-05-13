package org.cestra.login.packet;

import org.cestra.object.Player;
import org.cestra.object.Server;
import org.cestra.object.Account;
import org.cestra.kernel.Main;
import org.cestra.login.LoginClient;

public class FriendServerList
{
    public static void get(final LoginClient client, final String packet) {
        try {
            final String name = Main.database.getAccountData().exist(packet);
            if (name == null) {
                client.send("AF");
                return;
            }
            final Account account = Main.database.getAccountData().load(name);
            if (account == null) {
                client.send("AF");
                return;
            }
            Main.database.getPlayerData().load(account);
            client.send("AF" + getList(account));
        }
        catch (Exception ex) {}
    }
    
    public static String getList(final Account account) {
        final StringBuilder sb = new StringBuilder();
        for (final Server server : Server.servers.values()) {
            final int i = getNumber(account, server.getId());
            if (i != 0) {
                sb.append(server.getId()).append(",").append(i).append(";");
            }
        }
        return sb.toString();
    }
    
    public static int getNumber(final Account account, final int id) {
        int i = 0;
        for (final Player character : account.getPlayers().values()) {
            if (character.getServer() != id) {
                continue;
            }
            ++i;
        }
        return i;
    }
}
