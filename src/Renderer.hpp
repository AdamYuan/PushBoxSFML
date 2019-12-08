#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include "Level.hpp"
#include "Resource.hpp"
#include "Config.hpp"

class Renderer {
	private:
		sf::Texture m_player_tex, m_box_tex, m_box_target_tex,
			m_target_tex, m_wall_tex, m_empty_tex, m_background_tex;
		sf::Sprite m_player_spr, m_box_spr, m_box_target_spr, 
			m_target_spr, m_wall_spr, m_empty_spr, m_background_spr;

		inline sf::Sprite &get_sprite(uint8_t data) {
			switch(data) {
				case Block::kBox:
					return m_box_spr;
				case Block::kBoxTarget:
					return m_box_target_spr;
				case Block::kTarget:
					return m_target_spr;
				case Block::kWall:
					return m_wall_spr;
				case Block::kEmpty:
					return m_empty_spr;
				default: //Player
					return m_player_spr;
			}
		}

	public:
		inline Renderer() {
			std::string data;

			data = base64_decode( kPlayerPngBase64 );
			m_player_tex.loadFromMemory(data.c_str(), data.size());
			m_player_spr.setTexture(m_player_tex);
			m_player_spr.setScale(kSpriteScale, kSpriteScale);

			data = base64_decode( kEmptyPngBase64 );
			m_empty_tex.loadFromMemory(data.c_str(), data.size());
			m_empty_spr.setTexture(m_empty_tex);
			m_empty_spr.setScale(kSpriteScale, kSpriteScale);

			data = base64_decode( kTargetPngBase64 );
			m_target_tex.loadFromMemory(data.c_str(), data.size());
			m_target_spr.setTexture(m_target_tex);
			m_target_spr.setScale(kSpriteScale, kSpriteScale);

			data = base64_decode( kBoxPngBase64 );
			m_box_tex.loadFromMemory(data.c_str(), data.size());
			m_box_spr.setTexture(m_box_tex);
			m_box_spr.setScale(kSpriteScale, kSpriteScale);

			data = base64_decode( kBoxTargetPngBase64 );
			m_box_target_tex.loadFromMemory(data.c_str(), data.size());
			m_box_target_spr.setTexture(m_box_target_tex);
			m_box_target_spr.setScale(kSpriteScale, kSpriteScale);

			data = base64_decode( kWallPngBase64 );
			m_wall_tex.loadFromMemory(data.c_str(), data.size());
			m_wall_spr.setTexture(m_wall_tex);
			m_wall_spr.setScale(kSpriteScale, kSpriteScale);

			data = base64_decode( kBackgroundPngBase64 );
			m_background_tex.loadFromMemory(data.c_str(), data.size());
			m_background_tex.setRepeated( true );
			m_background_spr.setTexture(m_background_tex);
			m_background_spr.setScale(kSpriteScale, kSpriteScale);
			m_background_spr.setTextureRect({ 0, 0, kWindowWidth, kWindowHeight });
		}
		inline const sf::Texture &GetBlockTexture(uint8_t data) const {
			switch(data) {
				case Block::kBox:
					return m_box_tex;
				case Block::kBoxTarget:
					return m_box_target_tex;
				case Block::kTarget:
					return m_target_tex;
				case Block::kWall:
					return m_wall_tex;
				case Block::kEmpty:
					return m_empty_tex;
				default: //Player
					return m_player_tex;
			}
		}
		inline void RenderBlock(sf::RenderTarget &target, int x, int y, uint8_t data) {
			auto &spr = get_sprite(data);
			spr.setPosition(x * kBlockSize, y * kBlockSize + kUiHeight);
			spr.setColor(sf::Color::White);
			target.draw(spr);
		}
		inline void RenderEditorHover(sf::RenderTarget &target, int x, int y, uint8_t data) {
			auto &spr = get_sprite(data);
			spr.setPosition(x * kBlockSize, y * kBlockSize + kUiHeight);
			spr.setColor(sf::Color(255, 255, 255, kEditorHoverAlpha));
			target.draw(spr);
		}
		inline void RenderBackground(sf::RenderTarget &target) {
			target.draw(m_background_spr);
		}
		inline void RenderLevel(sf::RenderTarget &target, const Level &level) {
			if(level.Empty()) return;

			for(int y = 0; y < level.Height(); ++y)
				for(int x = 0; x < level.Width(); ++x)
					RenderBlock(target, x, y, level.GetBlock(x, y));

			RenderBlock(target, 
						level.PlayerX(), level.PlayerY(),
						Block::kPlayer);
		}
};

#endif
