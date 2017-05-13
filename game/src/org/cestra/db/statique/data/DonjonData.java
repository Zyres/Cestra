package org.cestra.db.statique.data;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;

import org.cestra.db.statique.AbstractDAO;
import org.cestra.game.world.World;
import org.cestra.kernel.Main;
import org.cestra.other.Dopeul;

public class DonjonData extends AbstractDAO<Object> {
	public DonjonData(Connection dataSource) {
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
				RS = this.getData("SELECT * FROM donjons");

				while (RS.next()) {
					Dopeul.getDonjons().put(RS.getInt("map"),
							new World.Couple<Integer, Integer>(RS.getInt("npc"), RS.getInt("key")));
				}
			} catch (SQLException e) {
				e.printStackTrace();
				Main.stop();
			}
		} finally {
			this.close(RS);
		}
	}

	public String get_all_keys() {
		ResultSet RS = null;
		try {
			RS = this.getData("SELECT key FROM donjons");

			String keys = "";
			while (RS.next()) {
				String key = Integer.toHexString(RS.getInt("key"));
				keys = String.valueOf(keys) + (keys.isEmpty() ? key : new StringBuilder(",").append(key).toString());
			}
			String string = keys;
			return string;
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			this.close(RS);
		}
		return "";
	}
}
