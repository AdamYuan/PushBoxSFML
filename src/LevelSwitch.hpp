#ifndef LEVEL_SWITCH_HPP
#define LEVEL_SWITCH_HPP

#include "Level.hpp"
#include "DataBase.hpp"

class LevelSwitch {
	private:
		int m_cur = -1;
		bool m_editor = false;
		uint8_t m_editor_cur_block = Block::kWall;
		int m_editor_x = 0, m_editor_y = 0;
		DataBase &m_db;
		Level &m_level;

	public:
		inline LevelSwitch(DataBase &database, Level &level) : m_db{database}, m_level{level} {}

		inline int GetCurrentLevel() const { return m_cur; }
		inline void GotoLevel(int id) {
			m_db.LoadLevel(m_level, (m_cur = id));
		}
		inline void Reset() const {
			m_db.LoadLevel(m_level, m_cur);
		}
		inline void Save() const {
			m_db.SaveLevel(m_level, m_cur);
		}
		inline bool &InEditor() { return m_editor; }
		inline bool InEditor() const { return m_editor; }
		inline int &EditorX() { return m_editor_x; }
		inline int EditorX() const { return m_editor_x; }
		inline int &EditorY() { return m_editor_y; }
		inline int EditorY() const { return m_editor_y; }
		inline bool EditorPosValid() const { 
			if(!m_level.IsValidPos(m_editor_x, m_editor_y)) return false; 
			if(m_editor_cur_block == Block::kPlayer) {
				uint8_t b = m_level.GetBlock(m_editor_x, m_editor_y);
				if(b == Block::kEmpty || b == Block::kTarget)
					return true;
				return false;
			}
			else return true;
		}
		inline bool EditorPut() { //return whether it is modified
			if(!EditorPosValid()) return false;
			if(m_editor_cur_block == Block::kPlayer) {
				if(m_editor_x == m_level.PlayerX() 
				   && m_editor_y == m_level.PlayerY()) 
					return false;
				m_level.PlayerX() = m_editor_x;
				m_level.PlayerY() = m_editor_y;
			}
			else {
				if(m_level.GetBlock(m_editor_x, m_editor_y) == m_editor_cur_block)
					return false;
				m_level.GetBlock(m_editor_x, m_editor_y) = m_editor_cur_block;
			}
			return true;
		}
		inline uint8_t EditorCurrentBlock() const { return m_editor_cur_block; }
		inline uint8_t &EditorCurrentBlock() { return m_editor_cur_block; }
};

#endif
