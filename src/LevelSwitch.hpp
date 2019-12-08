#ifndef LEVEL_SWITCH_HPP
#define LEVEL_SWITCH_HPP

#include "Level.hpp"
#include "DataBase.hpp"

class LevelSwitch {
	private:
		int m_cur = -1;
		bool m_editor = false;
		uint8_t m_editor_cur = Block::kWall;
		int m_editor_x = 0, m_editor_y = 0;
	public:
		inline int GetCurrent() const { return m_cur; }
		inline void Goto(Level &level, DataBase &db, int id) {
			db.LoadLevel(level, (m_cur = id));
		}
		inline void Reset(Level &level, DataBase &db) const {
			db.LoadLevel(level, m_cur);
		}
		inline void Save(Level &level, DataBase &db) const {
			db.SaveLevel(level, m_cur);
		}
		inline bool &InEditor() { return m_editor; }
		inline bool InEditor() const { return m_editor; }
		inline int &EditorX() { return m_editor_x; }
		inline int EditorX() const { return m_editor_x; }
		inline int &EditorY() { return m_editor_y; }
		inline int EditorY() const { return m_editor_y; }
		inline bool EditorPosValid(Level &level) const { 
			if(!level.IsValidPos(m_editor_x, m_editor_y)) return false; 
			if(m_editor_cur == Block::kPlayer) {
				uint8_t b = level.GetBlock(m_editor_x, m_editor_y);
				if(b == Block::kEmpty || b == Block::kTarget)
					return true;
				return false;
			}
			else return true;
		}
		inline bool EditorPut(Level &level) { //return whether it is modified
			if(!EditorPosValid(level)) return false;
			if(m_editor_cur == Block::kPlayer) {
				if(m_editor_x == level.PlayerX() 
				   && m_editor_y == level.PlayerY()) 
					return false;
				level.PlayerX() = m_editor_x;
				level.PlayerY() = m_editor_y;
			}
			else {
				if(level.GetBlock(m_editor_x, m_editor_y) == m_editor_cur)
					return false;
				level.GetBlock(m_editor_x, m_editor_y) = m_editor_cur;
			}
			return true;
		}
		inline uint8_t EditorCurrent() const { return m_editor_cur; }
		inline uint8_t &EditorCurrent() { return m_editor_cur; }
};

#endif
