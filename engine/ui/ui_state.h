#ifndef __ENGINE__UI__UI_STATE__
#define __ENGINE__UI__UI_STATE__

#include <string>
#include "../core/state.h"
#include "../core/asset_mgr.h"
#include "../ui/widget/viewport.h"
#include "ui_binding.h"

class UIException : public std::exception
{
    std::string msg;
public:
    UIException(std::string msg) : msg(msg) {};
    ~UIException() {};
    virtual const char* what() const throw() { return msg.c_str(); };
};

class UIState : public State {
protected:
    std::map<std::string, std::unique_ptr<UIBinding> > _bindings;
    std::unique_ptr<Viewport> viewport;
    Widget *focus;

public:
    UIState(const std::string &);
    ~UIState();

    virtual void draw(const Camera *camera) const override;
    virtual void update(const double) override;

    template<class T>
    T *registerBinding(const char *name) { return static_cast<T*>(_bindings.emplace(name, std::make_unique<T>()).first->second.get()); }

    Widget *find(const std::string &name);
    const Widget *find(const std::string &name) const;

    void setFocus(Widget *w) { focus = w; };
    inline const Widget *getFocus() const { return focus; }
    inline Widget *getFocus() { return focus; }
    inline UIBinding *getBinding(const std::string &key) const { return _bindings.find(key)->second.get(); };
    inline bool hasBinding(const std::string &key) const { return _bindings.find(key) != _bindings.end(); };

    bool visible;
};

#endif