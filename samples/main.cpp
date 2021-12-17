#include "MIMIWindow.h"
#include "MutableLiveData.h"
#include "UIObserver.h"
#include "UIViewController.h"
#include "UIViewModel.h"
#include <imgui.h>

using namespace MIMIIMGUI;

class TextViewModel : public UIViewModel {
  public:
	MutableLiveData<std::string> text;
	observable::value<std::string> text2;
};

class SampleComponent : public UIComponent {
  private:
	TextViewModel textView;

  public:
	SampleComponent() {
		this->setName("Sample Window");
		auto a = observable::value<int>{5};
		auto b = observable::value<int>{5};
		observable::observe((a + b) / 2.0f).subscribe([](auto val) { std::cout << val << std::endl; });
		this->textView.text.getObserver([this](std::string &text) {
			text.append("Hell");
			return;
		});
	}
	virtual void draw() override {

		if (ImGui::Button("Press me")) {
			textView.text2 = "";
		}
	}
};

class SampleWindow : public MIMIWindow {
  public:
	SampleWindow(WindowBackend::GfxBackEnd gfx = WindowBackend::GfxBackEnd::ImGUI_OpenGL) : MIMIWindow(gfx) {
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

	try {
		SampleWindow *window = new SampleWindow(static_cast<WindowBackend::GfxBackEnd>(std::atoi(argv[1])));

		window->run();
		return EXIT_SUCCESS;
	} catch (const std::exception &ex) {
		std::cerr << cxxexcept::getStackMessage(ex);
		return EXIT_FAILURE;
	}
}