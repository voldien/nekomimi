#include "MIMIWindow.h"

using namespace MIMIIMGUI;

class SampleComponent : public UIComponent {
  public:
  SampleComponent(){
	this->setName("Sample Window");
  }
	virtual void draw() override {}
};

class SampleWindow : public MIMIWindow {
  public:
	SampleWindow() : MIMIWindow(WindowBackend::GfxBackEnd::ImGUI_OpenGL) {
		std::shared_ptr<SampleComponent> com = std::make_shared<SampleComponent>();
        this->setTitle("Sample Main Window");
	
		this->addUIComponent(com);
	}
	virtual void displayMenuBar() override {}
	virtual void renderUI() override {}
};

int main(int argc, const char **argv) {

	SampleWindow *window = new SampleWindow();

	window->run();
}