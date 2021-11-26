#include "MIMIWindow.h"
using namespace MIMIIMGUI;

MIMIWindow::MIMIWindow(GfxBackEnd backend)
	: WindowBackend(WindowBackend::WindowLibBackend::WindowBackendSDL2, backend) {}

void MIMIWindow::run() {
	this->show();
	this->maximize();
	this->focus();

	bool show_dockspace = true;

	while (!requestQuit) {

		/*	*/
		beginRender();

		ImGui::NewFrame();

		showDockSpace(&show_dockspace);

		displayMenuBar();

		/*	Draw each UI elements on screen.	*/
		for (unsigned int i = 0; i < components.size(); i++) {
			std::shared_ptr<UIComponent> &uiComponent = this->components[i];
			uiComponent->drawUI();
		}

		/*	User Callback.	*/
		renderUI();

		ImGui::Render();

		endRender();
	}
}

void MIMIWindow::quit() { this->requestQuit = true; }

void MIMIWindow::addUIComponent(const std::shared_ptr<UIComponent> &component) {
	this->components.push_back(component);
}

void MIMIWindow::removeUIComponent(const std::shared_ptr<UIComponent> &component) {}
unsigned int MIMIWindow::getNrUIComponents() const { return this->components.size(); }
std::shared_ptr<UIComponent> &MIMIWindow::getComponent(unsigned int index) { return this->components[index]; }
