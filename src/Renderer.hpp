#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <cmath>
#include "Level.hpp"
#include "Resource.hpp"
#include "Config.hpp"

class Renderer {
	private:
		sf::RenderTexture m_front_buffer, m_back_buffer, m_light_buffer, m_hbuffer, m_vbuffer;
		sf::Shader m_shadow_blur_shader, m_shadow_final_shader;
		sf::VertexArray m_light_mesh;

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

		inline void set_sprite_pos(sf::Sprite &spr, int x, int y) {
			spr.setPosition(x * kBlockSize, y * kBlockSize + kUiHeight);
		}

		inline void render_light(const sf::Color &color, float radius, int x, int y) {
			m_light_mesh[0].color = color;
			sf::RenderStates state;
			state.transform.translate(x * kBlockSize + kBlockSize / 2, y * kBlockSize + kBlockSize / 2 + kUiHeight);
			state.transform.scale(radius, radius);
			state.blendMode = sf::BlendAdd;
			m_light_buffer.draw(m_light_mesh, state);
		}

		inline void render_to_textures(const Level &level) {
			m_front_buffer.clear(sf::Color::Transparent);
			m_back_buffer.clear(sf::Color::Transparent);
			m_light_buffer.clear(sf::Color{128, 128, 128, 255});

			if(level.Empty()) return;

			for(int y = 0; y < level.Height(); ++y)
				for(int x = 0; x < level.Width(); ++x) {
					uint8_t data = level.GetBlock(x, y);

					sf::Sprite spr = get_sprite(data);
					set_sprite_pos(spr, x, y);
					spr.setColor(sf::Color::White);

					if((data & Block::kBox) || (data & Block::kWall))
						m_front_buffer.draw(spr);
					else
						m_back_buffer.draw(spr);

					if(data == Block::kTarget)
						render_light({255, 0, 0, 255}, kBlockSize, x, y);
					else if(data == Block::kBoxTarget)
						render_light({128, 128, 0, 255}, kBlockSize * 2, x, y);
				}

			set_sprite_pos(m_player_spr, level.PlayerX(), level.PlayerY());
			m_player_spr.setColor(sf::Color::White);
			m_front_buffer.draw(m_player_spr);

			render_light({128, 128, 64, 255}, kLightRadius, level.PlayerX(), level.PlayerY());

			m_front_buffer.display();
			m_back_buffer.display();
			m_light_buffer.display();
		}

		inline void init_meshes() {
			m_light_mesh.clear();
			m_light_mesh.setPrimitiveType(sf::TriangleFan);

			sf::Vertex vert;
			vert.position = {0, 0};
			m_light_mesh.append(vert);

			vert.color = {0, 0, 0, 0};
			vert.position = { 1, 0 };
			m_light_mesh.append(vert);

			for(int i = 0; i < kLightCircleDivision; ++i) {
				float angle = kLightAngle * (float) (i + 1);

				vert.position = { cosf(angle), sinf(angle) };
				m_light_mesh.append(vert);
			}
		}

		inline void init_shaders() {
			m_shadow_blur_shader.loadFromMemory(kShadowBlurGlsl, sf::Shader::Fragment);
			m_shadow_blur_shader.setUniform("uTexture", sf::Shader::CurrentTexture);
			m_shadow_blur_shader.setUniform("uResolution", sf::Vector2f{kWindowWidth, kWindowHeight});

			m_shadow_final_shader.loadFromMemory(kShadowFinalGlsl, sf::Shader::Fragment);
			m_shadow_final_shader.setUniform("uTexture", sf::Shader::CurrentTexture);
		}

		inline void init_buffers() {
			m_front_buffer.create(kWindowWidth, kWindowHeight);
			m_back_buffer.create(kWindowWidth, kWindowHeight);
			m_light_buffer.create(kWindowWidth, kWindowHeight);
			m_hbuffer.create(kWindowWidth, kWindowHeight);
			m_vbuffer.create(kWindowWidth, kWindowHeight);
		}

		inline void init_textures() {
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

	public:
		inline Renderer() {
			init_meshes();
			init_shaders();
			init_buffers();
			init_textures();
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
		inline void RenderEditorHover(sf::RenderTarget &target, int x, int y, uint8_t data) {
			auto &spr = get_sprite(data);
			set_sprite_pos(spr, x, y);
			spr.setColor(sf::Color(255, 255, 255, kEditorHoverAlpha));
			target.draw(spr);
		}
		inline void RenderBackground(sf::RenderTarget &target) {
			target.draw(m_background_spr);
		}
		inline void RenderLevel(sf::RenderTarget &target, const Level &level) {
			if(level.Empty()) return;
			render_to_textures(level);
			target.draw(sf::Sprite{ m_back_buffer.getTexture() });

			m_hbuffer.clear(sf::Color::Transparent);
			m_vbuffer.clear(sf::Color::Transparent);

			sf::Shader::bind( &m_shadow_blur_shader );

			m_shadow_blur_shader.setUniform("uDirection", sf::Vector2f{1, 0});
			m_hbuffer.draw( sf::Sprite{ m_front_buffer.getTexture() } );
			m_hbuffer.display();

			m_shadow_blur_shader.setUniform("uDirection", sf::Vector2f{0, 1});
			m_vbuffer.draw( sf::Sprite{ m_hbuffer.getTexture() } );
			m_vbuffer.display();

			m_shadow_blur_shader.setUniform("uDirection", sf::Vector2f{1, 0});
			m_hbuffer.draw( sf::Sprite{ m_vbuffer.getTexture() } );
			m_hbuffer.display();

			m_shadow_blur_shader.setUniform("uDirection", sf::Vector2f{0, 1});
			m_vbuffer.draw( sf::Sprite{ m_hbuffer.getTexture() } );
			m_vbuffer.display();

			sf::Shader::bind( nullptr );

			target.draw( sf::Sprite{ m_vbuffer.getTexture() } );

			target.draw( sf::Sprite{ m_front_buffer.getTexture() } );
			target.draw( sf::Sprite{ m_light_buffer.getTexture() }, sf::BlendMultiply );
		}
};

#endif
