// 
// Decompiled by Procyon v0.5.30
// 

package org.aestia.game.scheduler;

import java.util.concurrent.TimeUnit;

import org.aestia.common.Formulas;
import org.aestia.common.SocketManager;
import org.aestia.kernel.Config;

public class WorldPub extends Manageable {
	@Override
	public void run()
	{
		switch (Formulas.getRandomValue(1, 4)) {
		case 1:
		{
			if (!Config.PUB1.isEmpty())
			{
				SocketManager.GAME_SEND_MESSAGE_TO_ALL(Config.PUB1, "046380");
				break;
			}
			break;
		}
		case 2:
		{
			if (!Config.PUB2.isEmpty())
			{
				SocketManager.GAME_SEND_MESSAGE_TO_ALL(Config.PUB2, "046380");
				break;
			}
			break;
		}
		case 3:
		{
			if (!Config.PUB3.isEmpty())
			{
				SocketManager.GAME_SEND_MESSAGE_TO_ALL(Config.PUB3, "046380");
				break;
			}
			break;
		}
		case 4:
		{
			if (!Config.PUB4.isEmpty())
			{
				SocketManager.GAME_SEND_MESSAGE_TO_ALL(Config.PUB4, "046380");
				break;
			}
			break;
		}
		}
	}

	@Override
	public void launch()
	{
		if (!Config.PUB1.isEmpty() && !Config.PUB2.isEmpty()
				&& !Config.PUB3.isEmpty() && !Config.PUB4.isEmpty())
		{
			GlobalManager.worldSheduler.scheduleWithFixedDelay(this, 15L, 15L, TimeUnit.MINUTES);
		}
	}
}
