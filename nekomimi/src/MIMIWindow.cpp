#include "MIMIWindow.h"
using namespace MIMIIMGUI;

MIMIWindow::MIMIWindow(GfxBackEnd backend)
	: WindowBackend(WindowBackend::WindowLibBackend::WindowBackendSDL2, backend) {}

void MIMIWindow::run() {

	this->maximize();
	this->focus();

	bool show_dockspace = true;
	bool show_viewports = false;

	while (!this->requestQuit) {

		/*	*/
		this->beginRender();

		ImGui::NewFrame();

		this->displayMenuBar();

		/*	User Callback.	*/
		this->renderUI();
		
		if(this->useDocking){
			this->showDockSpace(&show_dockspace);
		}
		this->showViewPorts(&show_viewports);

		/*	Draw each UI elements on screen.	*/
		for (unsigned int i = 0; i < components.size(); i++) {
			std::shared_ptr<UIComponent> &uiComponent = this->components[i];
			uiComponent->drawUI();
		}

		ImGui::Render();

		this->endRender();
	}
}

void MIMIWindow::quit() { this->requestQuit = true; }

void MIMIWindow::addUIComponent(const std::shared_ptr<UIComponent> &component) {
	this->components.push_back(component);
}

void MIMIWindow::removeUIComponent(const std::shared_ptr<UIComponent> &component) {}
unsigned int MIMIWindow::getNrUIComponents() const { return this->components.size(); }
std::shared_ptr<UIComponent> &MIMIWindow::getComponent(unsigned int index) { return this->components[index]; }
