package org.cestra.db.statique.data;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;

import org.cestra.db.statique.AbstractDAO;
import org.cestra.game.world.World;

public class Full_morphData extends AbstractDAO<Object> {
	public Full_morphData(Connection dataSource) {
		super(dataSource);
	}

	@Override
	public void load(Object obj) {
	}

	@Override
	public boolean update(Object obj) {
		return false;
	}

	public void load() {
		ResultSet RS = null;
		try {
			try {
				RS = this.getData("SELECT * FROM `full_morphs`");

				while (RS.next()) {
					String[] args = null;
					if (!RS.getString("args").equals("0")) {
						args = RS.getString("args").split("@")[1].split(",");
					}
					World.addFullMorph(RS.getInt("id"), RS.getString("name"), RS.getInt("gfxId"),
							RS.getString("spells"), args);
				}
			} catch (SQLException e) {
				e.printStackTrace();
			}
		} finally {
			this.close(RS);
		}
	}
}
