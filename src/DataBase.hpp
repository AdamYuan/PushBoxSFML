#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <fstream>
#include "Config.hpp"
#include "Level.hpp"
#include "Error.hpp"
#include "sqlite/sqlite3.h"

class DataBase {
	private:
		sqlite3 *m_db;
		sqlite3_stmt *m_insert_stmt, *m_delete_stmt, *m_decrease_stmt, 
					 *m_swap_stmt, *m_size_stmt, *m_flip_stmt, *m_query_stmt,
					 *m_update_stmt;

	public:
		inline int Size() const {
			sqlite3_reset(m_size_stmt);
			if( sqlite3_step(m_size_stmt) == SQLITE_ROW )
				return sqlite3_column_int(m_size_stmt, 0);
			return 0;
		}
		inline void Swap(int u, int v) {
			sqlite3_reset(m_swap_stmt);
			sqlite3_bind_int(m_swap_stmt, 1, u);
			sqlite3_bind_int(m_swap_stmt, 2, v);
			sqlite3_step(m_swap_stmt);

			sqlite3_reset(m_flip_stmt);
			sqlite3_step(m_flip_stmt);
		}
		inline void Delete(int id) {
			sqlite3_reset(m_delete_stmt);
			sqlite3_bind_int(m_delete_stmt, 1, id);
			sqlite3_step(m_delete_stmt);

			sqlite3_reset(m_decrease_stmt);
			sqlite3_bind_int(m_decrease_stmt, 1, id + 1);
			sqlite3_step(m_decrease_stmt);
		}
		inline void PushBack(int width, int height, const char *str) {
			sqlite3_reset(m_insert_stmt);
			sqlite3_bind_int(m_insert_stmt, 1, Size() + 1);
			sqlite3_bind_int(m_insert_stmt, 2, width);
			sqlite3_bind_int(m_insert_stmt, 3, height);
			sqlite3_bind_text(m_insert_stmt, 4, str, -1, SQLITE_STATIC);
			sqlite3_step(m_insert_stmt);
		}
		inline void LoadLevel(Level &level, int id) const {
			sqlite3_reset(m_query_stmt);
			sqlite3_bind_int(m_query_stmt, 1, id);
			sqlite3_step(m_query_stmt);
			level.LoadFromStr(sqlite3_column_int(m_query_stmt, 0), 
							  sqlite3_column_int(m_query_stmt, 1), 
							  (char *)sqlite3_column_text(m_query_stmt, 2));
		}
		inline void SaveLevel(Level &level, int id) {
			static std::string str;
			if(!level.ExportToStr(str)) return;
			sqlite3_reset(m_update_stmt);
			sqlite3_bind_int(m_update_stmt, 1, level.Width());
			sqlite3_bind_int(m_update_stmt, 2, level.Height());
			sqlite3_bind_text(m_update_stmt, 3, str.c_str(), -1, SQLITE_STATIC);
			sqlite3_bind_int(m_update_stmt, 4, id);
			sqlite3_step(m_update_stmt);
		}
		inline void SetDefault() {
			sqlite3_exec(m_db, "DELETE FROM Levels;", nullptr, nullptr, nullptr);
			for(auto &lvl : kDefaultLevels)
				PushBack(lvl.width, lvl.height, lvl.str);
		}
		inline DataBase(const char *filename) {
#define ERROR_EXIT(x) { \
	pberr::push("LOAD_DB_ERROR: " x); \
	return; }
			int stat;
			stat = sqlite3_open(filename, &m_db);
			if(stat != SQLITE_OK) ERROR_EXIT("Unable to Load DataBase");

			stat = sqlite3_exec(m_db, "PRAGMA QUICK_CHECK;", nullptr, nullptr, nullptr);
			if(stat != SQLITE_OK) { //clear file content if not a database
				std::ofstream ofs{kDataBaseFilename, std::ios::out | std::ios::trunc};
			}
			sqlite3_exec(m_db, 
						 "CREATE TABLE IF NOT EXISTS Levels (id INTEGER PRIMARY KEY, width INT, height INT, str TEXT);", 
						 nullptr, nullptr, nullptr);
			sqlite3_prepare_v2(m_db, "INSERT INTO Levels VALUES (?, ?, ?, ?);", 
							   -1, &m_insert_stmt, nullptr);
			sqlite3_prepare_v2(m_db, "DELETE FROM Levels WHERE id = ?;", 
							   -1, &m_delete_stmt, nullptr);
			sqlite3_prepare_v2(m_db, 
							   "UPDATE Levels SET id = (id - 1) WHERE id >= ?;",
							   -1, &m_decrease_stmt, nullptr);
			sqlite3_prepare_v2(m_db, "SELECT COUNT(*) FROM Levels;", -1, &m_size_stmt, nullptr);
			sqlite3_prepare_v2(m_db, 
							   "UPDATE Levels SET id = (CASE WHEN id = ?1 THEN -?2 ELSE -?1 END) WHERE id IN (?1, ?2);",
							   -1, &m_swap_stmt, nullptr);
			sqlite3_prepare_v2(m_db, "UPDATE Levels SET id = -id WHERE id < 0;", -1, &m_flip_stmt, nullptr);
			sqlite3_prepare_v2(m_db, "SELECT width, height, str FROM Levels WHERE id = ?;", -1, &m_query_stmt, nullptr);
			sqlite3_prepare_v2(m_db, "UPDATE Levels SET width = ?, height = ?, str = ? WHERE id = ?;", 
							   -1, &m_update_stmt, nullptr);

			if( Size() == 0 ) SetDefault();
		}
		~DataBase() {
			sqlite3_finalize(m_insert_stmt);
			sqlite3_finalize(m_delete_stmt);
			sqlite3_finalize(m_decrease_stmt);
			sqlite3_finalize(m_size_stmt);
			sqlite3_finalize(m_swap_stmt);
			sqlite3_finalize(m_flip_stmt);
			sqlite3_finalize(m_query_stmt);
			sqlite3_finalize(m_update_stmt);
			sqlite3_close(m_db);
		}
};
#endif
