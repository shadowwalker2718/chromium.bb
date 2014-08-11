/*
 * Copyright (C) 2014 Bloomberg Finance L.P.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS," WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef INCLUDED_BLPWTK2_VIEWSDELEGATEIMPL_H
#define INCLUDED_BLPWTK2_VIEWSDELEGATEIMPL_H

#include <blpwtk2_config.h>

#include <ui/views/views_delegate.h>

namespace blpwtk2 {

// Our implementation of views::ViewsDelegate.  This is a singleton created by
// BrowserMainRunner.  Right now, all the overrides are implemented based on
// content_shell's ShellViewsDelegateAura.  We may add custom stuff in here in
// the future.
class ViewsDelegateImpl : public views::ViewsDelegate {
public:
    ViewsDelegateImpl();
    virtual ~ViewsDelegateImpl();

private:
    virtual bool UseTransparentWindows() const OVERRIDE;
    virtual void SaveWindowPlacement(
        const views::Widget* widget,
        const std::string& window_name,
        const gfx::Rect& bounds,
        ui::WindowShowState show_state) OVERRIDE {}
    virtual bool GetSavedWindowPlacement(
        const views::Widget* widget,
        const std::string& window_name,
        gfx::Rect* bounds,
        ui::WindowShowState* show_state) const OVERRIDE;
    virtual void NotifyAccessibilityEvent(
        views::View* view,
        ui::AccessibilityTypes::Event event_type) OVERRIDE {}
    virtual void NotifyMenuItemFocused(
        const string16& menu_name,
        const string16& menu_item_name,
        int item_index,
        int item_count,
        bool has_submenu) OVERRIDE {}
#if defined(OS_WIN)
    virtual HICON GetDefaultWindowIcon() const OVERRIDE;
    virtual bool IsWindowInMetro(gfx::NativeWindow window) const OVERRIDE;
#endif
    virtual views::NonClientFrameView* CreateDefaultNonClientFrameView(
        views::Widget* widget) OVERRIDE;
    virtual void AddRef() OVERRIDE { }
    virtual void ReleaseRef() OVERRIDE { }
    virtual content::WebContents* CreateWebContents(
        content::BrowserContext* browser_context,
        content::SiteInstance* site_instance) OVERRIDE;
    virtual base::TimeDelta GetDefaultTextfieldObscuredRevealDuration() OVERRIDE;
    virtual void OnBeforeWidgetInit(
        views::Widget::InitParams* params,
        views::internal::NativeWidgetDelegate* delegate) OVERRIDE;

    DISALLOW_COPY_AND_ASSIGN(ViewsDelegateImpl);
};

}  // close namespace blpwtk2

#endif  // INCLUDED_BLPWTK2_VIEWSDELEGATEIMPL_H

