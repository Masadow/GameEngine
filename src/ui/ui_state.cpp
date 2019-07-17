#include "ui/ui_state.h"
#include "ui/style_parser.h"

using namespace UI;

UIState::UIState(const std::string &file) : focus(nullptr) {
    XMLDocumentPtr doc = AssetMgr::xml(file);

    tinyxml2::XMLElement *root = nullptr;
    StyleParser style;
    tinyxml2::XMLElement *e = doc->RootElement();
    while (e) {
        if (!std::strcmp(e->Name(), "viewport")) {
            root = e;
        } else if (!std::strcmp(e->Name(), "style")) {
            style.parse(e->GetText());
        }
        e = e->NextSiblingElement();
    }

    if (root && root->Name())
    {
        viewport = std::unique_ptr<Viewport>((Viewport*)Viewport::build_from_xml(this, root, nullptr, style).release());
    }
    else
    {
        throw UIException("Invalid root node in " + file);
    }

    visible = true;
}

UIState::~UIState() {

}

void UIState::draw(const Camera *camera) const
{
    if (visible) {
        State::draw(camera);

        if (viewport->enabled())
        {
            viewport->draw(camera);
        }
    }
}

void UIState::update(const double elapsed)
{
    State::update(elapsed);

    if (visible && viewport->enabled())
    {
        viewport->update(elapsed);
    }
}

Widget *UIState::find(const std::string &name)
{
    return viewport->find(name);
}

const Widget *UIState::find(const std::string &name) const
{
    return viewport->find(name);
}
