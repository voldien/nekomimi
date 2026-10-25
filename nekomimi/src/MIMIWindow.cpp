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

		if (this->isEnabled()) {

			if (getMenuBarVisable()) {
				this->displayMenuBar();
			}

			if (this->isDockingEnabled()) {

				this->showDockSpace(&show_dockspace);
			}

			/*	*/
			this->showViewPorts(&show_viewports);

			/*	User Callback.	*/
			this->renderUI();

			/*	Draw each UI elements on screen.	*/
			if (this->showUIComponents) {
				for (size_t com_index = 0; com_index < components.size(); com_index++) {
					std::shared_ptr<UIComponent> &uiComponent = this->components[com_index];
					ASSERT(&uiComponent, "ui component shall not be null");
					uiComponent->drawUI();
				}
			}
		}

		ImGui::Render();
		this->endRender();
	}

	this->onQuit();
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

void MIMIWindow::setMenuBarVisable(bool enabled) noexcept { this->showMenuBar = enabled; }
bool MIMIWindow::getMenuBarVisable() const noexcept { return this->showMenuBar; }

void MIMIWindow::setUIComponentEnabled(bool enabled) noexcept { this->showUIComponents = enabled; }