#include "Globals.h"
#include "PanelConsole.h"
#include "Application.h"
#include "Imgui/imgui.h"

#include "mmgr/mmgr.h"

PanelConsole::PanelConsole(char * name) : Panel(name)
{
}

PanelConsole::~PanelConsole()
{
	Clear();
}

bool PanelConsole::Draw()
{

	ImGuiWindowFlags consoleFlags = 0;
	consoleFlags |= ImGuiWindowFlags_NoFocusOnAppearing;

	if (ImGui::Begin(name, &enabled, consoleFlags))
	{
		if (ImGui::SmallButton("Clear"))
		{
			Clear();
		}

		ImGui::Separator();

		ImGuiWindowFlags scrollFlags = 0;
		scrollFlags |= ImGuiWindowFlags_HorizontalScrollbar;
		scrollFlags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;

		if (ImGui::BeginChild("Scrollbar", ImVec2(0, 0), false, scrollFlags))
		{
			// --- Print logs to console ---

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 1)); // Tighten spacing

			for (uint i = 0; i < App->GetLogs().size(); ++i)
			{
				ImGui::Text(App->GetLogs().at(i).data());
			}

			ImGui::PopStyleVar();
		}

		if(ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();

	}

	ImGui::End();

	return true;
}

void PanelConsole::Clear()
{
	App->ClearLogsFromConsole();
}
