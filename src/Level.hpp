#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <iostream>
#include <cstring>
#include <cinttypes>
#include "Config.hpp"
#include "Error.hpp"

enum Block {
	kEmpty = 0,
	kWall = 1,
	kTarget = 2,
	kBox = 4,
	kBoxTarget = 2 | 4,
	kPlayer = 8 //only for editor
};

class Level {
	private:
		int m_width, m_height, m_player_x, m_player_y;
		uint8_t m_grid[kMaxLevelWidth][kMaxLevelHeight];

		inline int clamp(int x, int mn, int mx) { return std::max(std::min(x, mx), mn); }

	public:
		inline bool CheckVictory() {
			if(Empty()) return false;
		
			bool box_exist = false;
			for(int y = 0; y < m_height; ++y)
				for(int x = 0; x < m_width; ++x) {
					uint8_t data = m_grid[x][y];
					if(data & Block::kBox) box_exist = true;
					if((data & Block::kBox) && !(data & Block::kTarget))
						return false;
				}
			return box_exist;
		}
		inline bool ExportToStr(std::string &ret) {
#define ERROR_EXIT(x) { \
	pberr::push("EXPORT_LEVEL_ERR: " x); \
	ret.clear(); \
	return false; }
			if(CheckVictory())
				ERROR_EXIT("Win without any Operation");
			ret.clear();
			ret.reserve(m_width * m_height);

			if(m_width < 1 || m_width > kMaxLevelWidth || m_height < 1 || m_height > kMaxLevelHeight)
				ERROR_EXIT("Invalid Level Size");

			if(m_player_x < 0 || m_player_x >= m_width || m_player_y < 0 || m_player_y >= m_height)
				ERROR_EXIT("Invalid Player Position");

			//int box_cnt = 0, target_cnt = 0;
			for(int y = 0; y < m_height; ++y)
				for(int x = 0; x < m_width; ++x) {
					int8_t c = m_grid[x][y];
					if(x == m_player_x && y == m_player_y) {
						if(c == Block::kEmpty)
							ret += 'P';
						else if(c == Block::kTarget)
							ret += 'p';//, ++target_cnt;
						else
							ERROR_EXIT("Invalid Block");
					}
					else {
						if(c == Block::kEmpty)
							ret += '.';
						else if(c == Block::kWall)
							ret += '#';
						else if(c == Block::kTarget)
							ret += 'T';//, ++target_cnt;
						else if(c == Block::kBox)
							ret += 'B';//, ++box_cnt;
						else if(c == Block::kBoxTarget)
							ret += 'b';//, ++box_cnt, ++target_cnt;
						else
							ERROR_EXIT("Invalid Block");
					}
				}
			/*if(target_cnt == 0)
				ERROR_EXIT("No Box and Target Exists");
			if(box_cnt != target_cnt)
				ERROR_EXIT("Box and Target don't Match");*/
#undef ERROR_EXIT
			return true;
		}
		inline bool LoadFromStr(int width, int height, const char *str) {
			//空: '.'
			//玩家: 'P'
			//踩目标点的玩家: 'p'
			//墙: '#'
			//箱子: 'B'
			//放在目标点上的箱子: 'b'
			//目标点: 'T'
#define ERROR_EXIT(x) { \
	pberr::push("LOAD_LEVEL_ERR: " x); \
	m_player_x = m_player_y = -1;\
	return false; }
			if(str == nullptr) ERROR_EXIT("Level Str is NULL");
			int len = strlen(str);
			if(len != width * height) ERROR_EXIT("Width and Height don't match Level String");
			if(width < 1 || width > kMaxLevelWidth || height < 1 || height > kMaxLevelHeight)
				ERROR_EXIT("Width or Height exceed range");

			m_player_x = m_player_y = -1;
			m_width = width, m_height = height;

			for(int i = 0; i < len; ++i) {
				int x = i % m_width, y = i / m_width;
				char c = str[i];
				if(c == '.')
					m_grid[x][y] = Block::kEmpty;
				else if(c == '#')
					m_grid[x][y] = Block::kWall;
				else if(c == 'T')
					m_grid[x][y] = Block::kTarget;
				else if(c == 'B')
					m_grid[x][y] = Block::kBox;
				else if(c == 'b')
					m_grid[x][y] = Block::kBoxTarget;
				else if(c == 'P' || c == 'p') {
					if(c == 'p') m_grid[x][y] = Block::kTarget;
					else m_grid[x][y] = Block::kEmpty;

					if(m_player_x != -1) ERROR_EXIT("Detected Multiple Player");

					m_player_x = i % m_width;
					m_player_y = i / m_width;
				}
				else ERROR_EXIT("Undefinded Char in Level String");
			}
			if(m_player_x == -1) ERROR_EXIT("No Player Detected");
			return true;
#undef ERROR_EXIT
		}
		inline void UpdateSize() {
			m_width = clamp(m_width, 1, kMaxLevelWidth);
			m_height = clamp(m_height, 1, kMaxLevelHeight);
			m_player_x = clamp(m_player_x, 0, m_width - 1);
			m_player_y = clamp(m_player_y, 0, m_height - 1);
		}
		bool Empty() const { return m_player_x == -1; }
		int Width() const { return m_width; }
		int &Width() { return m_width; }
		int Height() const { return m_height; }
		int &Height() { return m_height; }
		int PlayerX() const { return m_player_x; }
		int &PlayerX() { return m_player_x; }
		int PlayerY() const { return m_player_y; }
		int &PlayerY() { return m_player_y; }
		bool IsValidPos(int x, int y) const { return (0 <= x && x < m_width && 0 <= y && y < m_height); }
		uint8_t GetBlock(int x, int y) const { return m_grid[x][y]; }
		uint8_t &GetBlock(int x, int y) { return m_grid[x][y]; }
};

#endif
