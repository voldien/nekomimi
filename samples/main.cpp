#include "MIMIWindow.h"
#include <imgui.h>

using namespace MIMIIMGUI;

class SampleComponent : public UIComponent {
  public:
	SampleComponent() { this->setName("Sample Window"); }
	virtual void draw() override {}
};

class SampleWindow : public MIMIWindow {
  public:
	SampleWindow() : MIMIWindow(WindowBackend::GfxBackEnd::ImGUI_OpenGL) {
		this->setTitle("Sample Main Window");

		/*	*/
		std::shared_ptr<SampleComponent> com = std::make_shared<SampleComponent>();
		this->addUIComponent(com);
	}
	virtual void displayMenuBar() override {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New")) {
			}
			if (ImGui::MenuItem("Open", "Ctrl+O")) {
			}
			if (ImGui::BeginMenu("Open Recent")) {
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {
			}
			if (ImGui::MenuItem("Save As..", "Ctrl+Alt+S")) {
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Quit", "Alt+F4")) {
				this->quit();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Preference", nullptr)) {
			}
			ImGui::EndMenu();
		}
	}
	virtual void renderUI() override {}
};

int main(int argc, const char **argv) {

	SampleWindow *window = new SampleWindow();

	window->run();
}