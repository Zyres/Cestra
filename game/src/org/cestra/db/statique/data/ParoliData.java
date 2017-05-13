package org.cestra.db.statique.data;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import org.cestra.db.statique.AbstractDAO;

public class ParoliData extends AbstractDAO<org.cestra.object.Object> {
	public ParoliData(Connection dataSource) {
		super(dataSource);
	}

	@Override
	public void load(Object obj) {
	}

	@Override
	public boolean update(org.cestra.object.Object obj) {
		return false;
	}

	public void add(org.cestra.object.Object obvi, org.cestra.object.Object victime) {
		PreparedStatement p = null;
		try {
			try {
				p = this.getPreparedStatement("INSERT INTO paroli(`template_obvi`,`id_victime`) VALUES(?,?)");
				p.setInt(1, obvi.getTemplate().getId());
				p.setInt(2, victime.getGuid());
				this.execute(p);
			} catch (Exception e) {
				super.sendError("ParoliData add", e);
			}
		} finally {
			this.close(p);
		}
	}

	public int getAndDelete(org.cestra.object.Object victime, boolean delete) {
		int res;
		ResultSet RS = null;
		res = -1;
		try {
			try {
				RS = this.getData("SELECT * FROM paroli WHERE id_victime='" + victime.getGuid() + "'");
				if (RS.next()) {
					res = RS.getInt("template_obvi");
					if (delete) {
						PreparedStatement ps = this.getPreparedStatement(
								"DELETE FROM paroli WHERE id_victime='" + victime.getGuid() + "'");
						this.execute(ps);
					}
				}
			} catch (SQLException e) {
				super.sendError("ParoliData getAndDelete", e);
			}
		} finally {
			this.close(RS);
		}
		return res;
	}
}
