package org.cestra.command;

import org.cestra.client.Player;
import org.cestra.command.server.CDefault;
import org.cestra.kernel.Config;

public class Admin {
	public static Command get(final Player player) {
		Config.getInstance().NAME.toUpperCase().hashCode();
		return new CDefault(player);
	}
}
