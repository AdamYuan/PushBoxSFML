#ifndef LOGIC_HPP
#define LOGIC_HPP

#include "Level.hpp"

enum Movement {
	kLeft = 0,
	kRight,
	kUp,
	kDown
};

class Logic {
	public:
		inline void Update(Level &level, Movement mov) {
			if(level.Empty()) return;
			constexpr static int kDx[4] = {-1, 1, 0, 0};
			constexpr static int kDy[4] = {0, 0, -1, 1};

			int px = level.PlayerX(), py = level.PlayerY();
			int nx = px + kDx[int(mov)], ny = py + kDy[int(mov)];
			if( !level.IsValidPos(nx, ny) ) return;
			if( level.GetBlock(nx, ny) & Block::kWall ) return;
			if( level.GetBlock(nx, ny) & Block::kBox ) {
				int nnx = nx + kDx[int(mov)], nny = ny + kDy[int(mov)];
				if( !level.IsValidPos(nnx, nny) ) return;
				if( level.GetBlock(nnx, nny) & Block::kWall ) return;
				if( level.GetBlock(nnx, nny) & Block::kBox ) return;

				level.GetBlock(nx, ny) ^= Block::kBox;
				level.GetBlock(nnx, nny) ^= Block::kBox;

				level.PlayerX() = nx;
				level.PlayerY() = ny;
			}
			else {
				level.PlayerX() = nx;
				level.PlayerY() = ny;
			}
		}
};

#endif
