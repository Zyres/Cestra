package org.cestra.db.statique.data;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;

import org.cestra.db.statique.AbstractDAO;
import org.cestra.kernel.Constant;

public class ZaapData extends AbstractDAO<Object> {
	public ZaapData(Connection dataSource) {
		super(dataSource);
	}

	@Override
	public void load(Object obj) {
	}

	@Override
	public boolean update(Object obj) {
		return false;
	}

	public int load() {
		int i;
		ResultSet RS = null;
		i = 0;
		try {
			try {
				RS = this.getData("SELECT mapID, cellID from zaaps");

				while (RS.next()) {
					Constant.ZAAPS.put(RS.getInt("mapID"), RS.getInt("cellID"));
					++i;
				}
			} catch (SQLException e) {
				super.sendError("ZaapData load", e);
			}
		} finally {
			this.close(RS);
		}
		return i;
	}
}
