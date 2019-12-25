#pragma once



#ifdef _WIN32 // WINDOWS

	#include <windows.h>
	#undef min
	#undef max

	#ifndef __MINGW32__ // Visual Studio
/* using sfml 2.5.1

bin output dir		: $(SolutionDir)bin\$(Configuration)-$(Platform)\$(ProjectName)\
bin-int output dir	: $(SolutionDir)bin-int\$(Configuration)-$(Platform)\$(ProjectName)\

addnl include dirs	: $(ProjectDir)vender/SFML-2.5.1/include;$(ProjectDir)vender/ImSf;$(ProjectDir)vender/ImGui
addnl linking dirs	: $(ProjectDir)vender/SFML-2.5.1/lib;%(AdditionalLibraryDirectories)
*/
		#pragma comment(lib, "opengl32.lib")
		#pragma comment(lib, "user32.lib")
		#pragma comment(lib, "gdi32.lib")
		
		#ifdef _DEBUG
			#pragma comment(lib, "sfml-main-d.lib")
			#pragma comment(lib, "sfml-audio-d.lib")
			#pragma comment(lib, "sfml-graphics-d.lib")
			#pragma comment(lib, "sfml-network-d.lib")
			#pragma comment(lib, "sfml-window-d.lib")
			#pragma comment(lib, "sfml-system-d.lib")
		#else
			#pragma comment(lib, "sfml-main.lib")
			#pragma comment(lib, "sfml-audio.lib")
			#pragma comment(lib, "sfml-graphics.lib")
			#pragma comment(lib, "sfml-network.lib")
			#pragma comment(lib, "sfml-window.lib")
			#pragma comment(lib, "sfml-system.lib")
		#endif 
	#endif
	

#endif

#ifdef __linux__ // LINUX
#endif

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

inline void render_dock_space()
{
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	bool p_open = false;
	ImGui::Begin("DockSpace", &p_open, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiIO& io = ImGui::GetIO();
	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

	ImGui::End();
}