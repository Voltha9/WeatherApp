#include "Application.h"

#include <imgui/imgui-SFML.h>
#include <imgui/imgui.h>

Application::Application() :
	window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Weather App"),
	weatherAPI(GetApiKey()),
	weatherGUI(weatherAPI)
{
	window.setFramerateLimit(60);

	ImGui::SFML::Init(window, false);

	SetupGuiStyle();
	SetupWindowIcon();
}

Application::~Application()
{
	ImGui::SFML::Shutdown();
}

bool Application::IsRunning() const
{
	return window.isOpen();
}

void Application::Events()
{
	sf::Event event;

	while (window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);

		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
	}
}

void Application::Update()
{
	sf::Time deltaTime = deltaClock.restart();

	ImGui::SFML::Update(window, deltaTime);
}

void Application::Render()
{
	window.clear();

	weatherGUI.Render();
	ImGui::SFML::Render(window);

	window.display();
}

void Application::SetupGuiStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 5.0f;
	style.WindowRounding = 7.0f;
	style.GrabRounding = 5.0f;
	style.ScrollbarRounding = 10.0f;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.5f, 0.8f, 1.0f);
	colors[ImGuiCol_Button] = ImVec4(0.0f, 0.7f, 0.3f, 1.0f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.0f, 0.8f, 0.4f, 1.0f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.0f, 0.6f, 0.3f, 1.0f);

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("Resources/Fonts/Roboto-Regular.ttf", 24);
	ImGui::SFML::UpdateFontTexture();
}

void Application::SetupWindowIcon()
{
	sf::Image icon;
	icon.loadFromFile("Resources/Textures/icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}