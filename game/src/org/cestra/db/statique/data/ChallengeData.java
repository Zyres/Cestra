package org.cestra.db.statique.data;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;

import org.cestra.db.statique.AbstractDAO;
import org.cestra.game.world.World;

public class ChallengeData extends AbstractDAO<Object> {
	public ChallengeData(Connection dataSource) {
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
				RS = this.getData("SELECT * from challenge");

				while (RS.next()) {
					StringBuilder chal = new StringBuilder();
					chal.append(RS.getInt("id")).append(",");
					chal.append(RS.getInt("gainXP")).append(",");
					chal.append(RS.getInt("gainDrop")).append(",");
					chal.append(RS.getInt("gainParMob")).append(",");
					chal.append(RS.getInt("conditions"));
					World.addChallenge(chal.toString());
				}
			} catch (SQLException e) {
				e.printStackTrace();
			}
		} finally {
			this.close(RS);
		}
	}
}
