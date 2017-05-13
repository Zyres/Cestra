package org.cestra.db.game;

interface DAO<T> {
	public void load(Object var1);

	public boolean update(T var1);
}
