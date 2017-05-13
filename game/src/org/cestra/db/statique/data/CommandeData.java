package org.cestra.db.statique.data;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;

import org.cestra.command.server.Commandes;
import org.cestra.db.statique.AbstractDAO;
import org.cestra.kernel.Main;

public class CommandeData extends AbstractDAO<Commandes> {
	public CommandeData(Connection dataSource) {
		super(dataSource);
	}

	@Override
	public void load(Object obj) {
	}

	@Override
	public boolean update(Commandes obj) {
		return false;
	}

	public void load() {
		ResultSet RS = null;
		try {
			try {
				RS = this.getData("SELECT * FROM commandes");

				while (RS.next()) {
					new org.cestra.command.server.Commandes(RS.getInt("id"), RS.getString("commande"),
							RS.getString("args"), RS.getString("description"));
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
