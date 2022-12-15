#include "MIMIWindow.h"
#include "MutableLiveData.h"
#include "UIObserver.h"
#include "UIViewController.h"
#include "UIViewModel.h"
#include <Util/imgui-ext.h>
#include <imgui.h>

using namespace nekomimi;

class TextViewModel : public UIViewModel {
  public:
	MutableLiveData<std::string> text;
	observable::value<std::string> text2;
};

class SampleComponent : public UIComponent {
  private:
	TextViewModel textView;
	observable::value<int> a;

  public:
	SampleComponent() {
		this->setName("Sample Window");
		a = observable::value<int>{0};
		auto b = observable::value<int>{0};

		//		auto c = observable::observe((a + b) / 2.0f).subscribe([](auto val) { std::cout << val << std::endl; });

		this->textView.text.subscribe([this] { textView.text.setValue(textView.text.getValue() + "Was"); });

		//a.subscribe([this] {
		//	std::string text = "Hello Thre ";
		//	text += (a.get() + '0');
		//	textView.text.setValue(text);
		//});
	}
	virtual void draw() override {

		if (ImGui::Button("Press me - ")) {
			textView.text.set("Added some text");
			// a = a.get() + 1;
		}
		
		ImGui::TextUnformatted(textView.text.getValue().c_str());
	}
};

class SampleWindow : public MIMIWindow {
  public:
	SampleWindow(WindowBackend::GfxBackEnd gfx = WindowBackend::GfxBackEnd::ImGUI_OpenGL) : MIMIWindow(gfx) {
		this->setTitle(fmt::format("Sample Main Window: {}", this->getRenderInterface()->getName()));

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

	size_t defaultGFX = static_cast<size_t>(WindowBackend::GfxBackEnd::ImGUI_Default);
	if(argc > 1){
		defaultGFX = std::atoi(argv[1]);
	} 

	try {
		SampleWindow *window = new SampleWindow(static_cast<WindowBackend::GfxBackEnd>(defaultGFX));

		window->run();
		return EXIT_SUCCESS;
	} catch (const std::exception &ex) {
		std::cerr << cxxexcept::getStackMessage(ex) << std::endl;
		return EXIT_FAILURE;
	}
}