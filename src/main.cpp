#include <iostream>

#include "Renderer.hpp"
#include "Logic.hpp"
#include "DataBase.hpp"
#include "LevelSwitch.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui-SFML.h"

sf::RenderWindow window;

Renderer renderer;
Level level;
Logic logic;
DataBase database{kDataBaseFilename};
LevelSwitch levelswitch;

//空: '.'
//玩家: 'P'
//踩目标点的玩家: 'p'
//墙: '#'
//箱子: 'B'
//放在目标点上的箱子: 'b'
//目标点: 'T'

inline void initialize() {
	window.create(sf::VideoMode(kWindowWidth, kWindowHeight), 
				  "PushBox", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);
	ImGui::SFML::Init(window);
	//Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDarcula();

	levelswitch.Goto(level, database, 1);
}

inline void ui_push_disable()
{
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
}

inline void ui_pop_disable()
{
	ImGui::PopItemFlag();
	ImGui::PopStyleVar();
}

inline void ui_error_notification() {
	static sf::Clock s_clk;
	static bool s_show = false;
	if(!pberr::empty() && !s_show) {
		s_clk.restart();
		s_show = true;
	}

	if(s_clk.getElapsedTime() > sf::seconds(kErrorNotificationLifespan)) {
		pberr::clear();
		s_show = false;
	}

	if(s_show) {
		ImGui::SetNextWindowPos(ImGui::GetIO().DisplaySize,
								ImGuiCond_Always, ImVec2(1, 1));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.4f)); // Transparent background
		if (ImGui::Begin("INFO", nullptr,
						 ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize
						 |ImGuiWindowFlags_NoFocusOnAppearing
						 |ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoMove
						 |ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoBringToFrontOnFocus))
		{
			ImGui::TextEx(pberr::str.c_str());
			ImGui::End();
		}
		ImGui::PopStyleColor();
	}
}

inline void ui_editor() {
	ImGui::Text("#%d", levelswitch.GetCurrent());
	if(ImGui::BeginMenu("Levels")) {

		ImGui::ListBoxHeader("##2");
		for(int i = 1; i <= database.Size(); ++i) {
			static char level_text[64];
			sprintf(level_text, "#%d", i);
			if(ImGui::Selectable(level_text, levelswitch.GetCurrent() == i))
				levelswitch.Goto(level, database, i);
		}
		ImGui::ListBoxFooter();

		{
			bool can_move_up = levelswitch.GetCurrent() > 1;
			if(!can_move_up) ui_push_disable();
			if(ImGui::Button("Move Up"))
			{
				database.Swap(levelswitch.GetCurrent(), levelswitch.GetCurrent() - 1);
				levelswitch.Goto(level, database, levelswitch.GetCurrent() - 1);
			}
			if(!can_move_up) ui_pop_disable();
		}
		ImGui::SameLine();

		{
			bool can_move_down = levelswitch.GetCurrent() < database.Size();
			if(!can_move_down) ui_push_disable();
			if(ImGui::Button("Move Down"))
			{
				database.Swap(levelswitch.GetCurrent(), levelswitch.GetCurrent() + 1);
				levelswitch.Goto(level, database, levelswitch.GetCurrent() + 1);
			}
			if(!can_move_down) ui_pop_disable();
		}

		ImGui::SameLine();

		if(ImGui::Button("New")) {
			database.PushBack(kNewLevel.width, kNewLevel.height, kNewLevel.str);
			levelswitch.Goto(level, database, database.Size());
		}

		ImGui::SameLine();

		{
			bool can_delete = database.Size() > 1;
			if(!can_delete) ui_push_disable();
			if(ImGui::Button("Delete"))
			{
				database.Delete(levelswitch.GetCurrent());
				levelswitch.Goto(level, database, std::min(levelswitch.GetCurrent(), database.Size()));
			}
			if(!can_delete) ui_pop_disable();
		}

		bool level_size_changed = false;
		ImGui::SetNextItemWidth(kUiItemWidth);
		level_size_changed |= ImGui::DragInt("Width", &level.Width(), 1, 1, kMaxLevelWidth);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(kUiItemWidth);
		level_size_changed |= ImGui::DragInt("Height", &level.Height(), 1, 1, kMaxLevelHeight);

		if(level_size_changed) {
			level.UpdateSize();
			levelswitch.Save(level, database);
		}

		ImGui::EndMenu();
	}

	ImGui::Image(renderer.GetBlockTexture(levelswitch.EditorCurrent()), {kSmallBlockSize, kSmallBlockSize}, 
				 sf::Color(255, 255, 255, kEditorHoverAlpha));

	if(ImGui::BeginMenu("Blocks")) {
		ImGui::Image( renderer.GetBlockTexture(Block::kBox), {kSmallBlockSize, kSmallBlockSize} );
		ImGui::SameLine();
		if(ImGui::MenuItem("Box", nullptr, levelswitch.EditorCurrent() == Block::kBox))
			levelswitch.EditorCurrent() = Block::kBox;

		ImGui::Image( renderer.GetBlockTexture(Block::kBoxTarget), {kSmallBlockSize, kSmallBlockSize} );
		ImGui::SameLine();
		if(ImGui::MenuItem("Box with Target", nullptr, levelswitch.EditorCurrent() == Block::kBoxTarget))
			levelswitch.EditorCurrent() = Block::kBoxTarget;

		ImGui::Image( renderer.GetBlockTexture(Block::kTarget), {kSmallBlockSize, kSmallBlockSize} );
		ImGui::SameLine();
		if(ImGui::MenuItem("Target", nullptr, levelswitch.EditorCurrent() == Block::kTarget))
			levelswitch.EditorCurrent() = Block::kTarget;

		ImGui::Image( renderer.GetBlockTexture(Block::kWall), {kSmallBlockSize, kSmallBlockSize} );
		ImGui::SameLine();
		if(ImGui::MenuItem("Wall", nullptr, levelswitch.EditorCurrent() == Block::kWall))
			levelswitch.EditorCurrent() = Block::kWall;

		ImGui::Image( renderer.GetBlockTexture(Block::kEmpty), {kSmallBlockSize, kSmallBlockSize} );
		ImGui::SameLine();
		if(ImGui::MenuItem("Empty", nullptr, levelswitch.EditorCurrent() == Block::kEmpty))
			levelswitch.EditorCurrent() = Block::kEmpty;

		ImGui::Image( renderer.GetBlockTexture(Block::kPlayer), {kSmallBlockSize, kSmallBlockSize} );
		ImGui::SameLine();
		if(ImGui::MenuItem("Player", nullptr, levelswitch.EditorCurrent() == Block::kPlayer))
			levelswitch.EditorCurrent() = Block::kPlayer;

		ImGui::EndMenu();
	}

	if(!ImGui::GetCurrentContext()->NavWindow
	   || (ImGui::GetCurrentContext()->NavWindow->Flags & ImGuiWindowFlags_NoBringToFrontOnFocus)) //no windows focus
	{
		if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			if(levelswitch.EditorPut(level)) levelswitch.Save(level, database);
	}
}

inline void ui_game() {
	ImGui::Text("#%d", levelswitch.GetCurrent());
	if(ImGui::BeginMenu("Levels")) {
		for(int i = 1; i <= database.Size(); ++i) {
			static char level_text[64];
			sprintf(level_text, "#%d", i);
			if(ImGui::MenuItem(level_text, nullptr, levelswitch.GetCurrent() == i))
				if(levelswitch.GetCurrent() != i) levelswitch.Goto(level, database, i);
		}
		ImGui::EndMenu();
	}

	if(ImGui::Button("Restart"))
		levelswitch.Reset(level, database);

	if(level.CheckVictory())
		ImGui::OpenPopup("Victory");

	if(ImGui::BeginPopupModal("Victory", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) {
		ImGui::TextColored({1.0f, 0.5f, 0.0f, 1.0f}, "YOU WIN!!");

		bool have_next = levelswitch.GetCurrent() < database.Size();
		if(!have_next) ui_push_disable();
		if(ImGui::Button("Next Level", ImVec2(kUiItemWidth, 0))) {
			levelswitch.Goto(level, database, levelswitch.GetCurrent() + 1);
			ImGui::CloseCurrentPopup();
		}
		if(!have_next) ui_pop_disable();

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Replay", ImVec2(kUiItemWidth, 0))) {
			levelswitch.Reset(level, database);
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

inline void ui_main_menu() {
	ImGui::BeginMainMenuBar();

	if(levelswitch.InEditor())
		ui_editor();
	else
		ui_game();

	ImGui::Separator();
	if(ImGui::Checkbox("Editor", &levelswitch.InEditor()))
		levelswitch.Reset(level, database);

	ImGui::EndMainMenuBar();
}

inline void ui_main() {
	ui_main_menu();
	ui_error_notification();
}

inline void render() {
	renderer.RenderBackground(window);
	renderer.RenderLevel(window, level);

	if(levelswitch.InEditor() && levelswitch.EditorPosValid(level)) {
		renderer.RenderEditorHover(window, levelswitch.EditorX(), levelswitch.EditorY(), 
								   levelswitch.EditorCurrent());
	}
	ui_main();
}

inline void control() {
	sf::Event event;
	while(window.pollEvent(event)) {
		ImGui::SFML::ProcessEvent(event);

		if(event.type == sf::Event::Closed)
			window.close();

		if(levelswitch.InEditor() == false && event.type == sf::Event::KeyPressed) {
			switch(event.key.code) {
				case sf::Keyboard::Left:
				case sf::Keyboard::A:
				case sf::Keyboard::H:
					logic.Update(level, Movement::kLeft);
					break;
				case sf::Keyboard::Right:
				case sf::Keyboard::D:
				case sf::Keyboard::L:
					logic.Update(level, Movement::kRight);
					break;
				case sf::Keyboard::Up:
				case sf::Keyboard::W:
				case sf::Keyboard::K:
					logic.Update(level, Movement::kUp);
					break;
				case sf::Keyboard::Down:
				case sf::Keyboard::S:
				case sf::Keyboard::J:
					logic.Update(level, Movement::kDown);
					break;
				default: break;
			}
		}
	}
	//update selection position
	auto mpos = sf::Mouse::getPosition(window);
	levelswitch.EditorX() = mpos.x / kBlockSize;
	levelswitch.EditorY() = mpos.y / kBlockSize - 1;
}

inline void loop() {
	sf::Clock delta_clock;
	while(window.isOpen()) {
		control();
		ImGui::SFML::Update(window, delta_clock.restart());
		render();
		ImGui::SFML::Render(window);
		window.display();
	}
}

int main() {
	initialize();
	loop();
	return 0;
}
