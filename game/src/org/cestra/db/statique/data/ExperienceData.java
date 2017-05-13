package org.cestra.db.statique.data;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;

import org.cestra.db.statique.AbstractDAO;
import org.cestra.game.world.World;
import org.cestra.kernel.Main;

public class ExperienceData extends AbstractDAO<World.ExpLevel> {
	public ExperienceData(Connection dataSource) {
		super(dataSource);
	}

	@Override
	public void load(Object obj) {
	}

	@Override
	public boolean update(World.ExpLevel obj) {
		return false;
	}

	public void load() {
		ResultSet RS = null;
		try {
			try {
				RS = this.getData("SELECT * from experience");

				while (RS.next()) {
					World.addExpLevel(RS.getInt("lvl"), new World.ExpLevel(RS.getLong("perso"), RS.getInt("metier"),
							RS.getInt("dinde"), RS.getInt("pvp"), RS.getLong("tourmenteurs"), RS.getLong("bandits")));
				}
			} catch (SQLException e) {
				e.printStackTrace();
				Main.stop();
			}
		} finally {
			this.close(RS);
		}
	}
}
