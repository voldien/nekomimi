#include "MIMIWindow.h"

using namespace nekomimi;

MIMIWindow::MIMIWindow(const GfxBackEnd backend, const WindowLibBackend window_backend)
	: WindowBackend(window_backend, backend) {}

void MIMIWindow::run() {

	/*	*/
	this->show();
	this->maximize();
	this->focus();

	/*	*/
	bool show_dockspace = true;
	bool show_viewports = false;

	/*	*/
	while (!this->requestQuit) {

		/*	*/
		this->beginRender();

		ImGui::NewFrame();

		if (getMenuBarVisable()) {
			this->displayMenuBar();
		}

		/*	User Callback.	*/
		this->renderUI();

		if (this->isEnabled()) {

			if (this->useDocking) {
				this->showDockSpace(&show_dockspace);
			}

			/*	*/
			this->showViewPorts(&show_viewports);

			/*	Draw each UI elements on screen.	*/
			for (size_t i = 0; i < components.size(); i++) {
				std::shared_ptr<UIComponent> &uiComponent = this->components[i];
				uiComponent->drawUI();
			}
		}

		ImGui::Render();
		this->endRender();
	}
}

void MIMIWindow::quit() { this->requestQuit = true; }

void MIMIWindow::addUIComponent(const std::shared_ptr<UIComponent> &component) {
	this->components.push_back(component);
}

void MIMIWindow::removeUIComponent(const std::shared_ptr<UIComponent> &component) {
	// this->components.er
}
size_t MIMIWindow::getNrUIComponents() const noexcept { return this->components.size(); }
std::shared_ptr<UIComponent> &MIMIWindow::getComponent(const size_t index) { return this->components[index]; }

void MIMIWindow::setMenuBarVisable(bool enabled) { this->showMenuBar = enabled; }
bool MIMIWindow::getMenuBarVisable() const noexcept { return this->showMenuBar; }