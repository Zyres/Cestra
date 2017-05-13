package org.cestra.db.statique.data;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;

import org.cestra.db.statique.AbstractDAO;
import org.cestra.game.world.World;
import org.cestra.other.Trunk;

public class CoffreData extends AbstractDAO<Trunk> {
	public CoffreData(Connection dataSource) {
		super(dataSource);
	}

	@Override
	public void load(Object obj) {
	}

	@Override
	public boolean update(Trunk t) {
		return false;
	}

	public int load() {
		int nbr;
		ResultSet RS = null;
		nbr = 0;
		try {
			try {
				RS = this.getData("SELECT * from coffres");

				while (RS.next()) {
					World.addTrunk(new Trunk(RS.getInt("id"), RS.getInt("id_house"), RS.getShort("mapid"),
							RS.getInt("cellid")));
					++nbr;
				}
			} catch (SQLException e) {
				e.printStackTrace();
			}
		} finally {
			this.close(RS);
		}
		return nbr;
	}
}
