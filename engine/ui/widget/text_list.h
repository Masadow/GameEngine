#ifndef __ENGINE__UI__WIDGET__TEXT_LIST__
#define __ENGINE__UI__WIDGET__TEXT_LIST__

#include <vector>
#include <string>
#include <allegro5/allegro_font.h>
#include "../widget.h"

class TextList : public Widget {
protected:
    std::vector<std::pair<std::string, std::string> > _content;

    int _hovered_idx;
    int _selected_idx;
    int row_height;

public:
    TextList(tinyxml2::XMLElement *xml, Widget *, const UI::StyleParser &);
    ~TextList();

    ALLEGRO_COLOR text_color;
    bool selectionEnabled;
    bool unselectionEnabled;

    virtual void draw(const Camera *camera) const override;
    virtual void update(const double) override;

    void addItem(const std::string &, const std::string & = "");
    void clear();

    void clearSelection();

    inline int getSelectedIdx() const { return _selected_idx; }
    inline void setSelectedIdx(int selected_idx) { _selected_idx = selected_idx; }
    inline const std::string &getSelectedId() const { return _content[_selected_idx].second; }
    inline const std::string &getSelectedValue() const { return _content[_selected_idx].first; }
    inline const std::string &getIdByIdx(int idx) const { return _content[idx].second; }

    std::function<ALLEGRO_COLOR(int idx)> getItemColor;

    static std::unique_ptr<Widget> build_from_xml(tinyxml2::XMLElement *, Widget *, const UI::StyleParser &);
};

#endif
