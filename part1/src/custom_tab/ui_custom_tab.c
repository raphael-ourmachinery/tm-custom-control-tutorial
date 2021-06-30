#include "ui_custom_tab.h"
#include "../ui_custom_controls_loader.h"
#include "../ui_custom_controls.h"


#include <foundation/api_registry.h>
#include <foundation/carray.inl>
#include <foundation/log.h>
#include <foundation/profiler.h>

#include <plugins/ui/docking.h>
#include <plugins/ui/ui.h>

#include <the_machinery/the_machinery_tab.h>


const float metrics_item_h = 25.f;

struct tm_tab_o
{
    tm_tab_i tm_tab_i;
    tm_allocator_i *allocator;
    //scroll
    float scroll_y;
    float max_x;
    float max_y;
};

#pragma region tab

static void tab__ui(tm_tab_o *data, tm_ui_o *ui, const struct tm_ui_style_t *uistyle_in, tm_rect_t content_r)
{
    TM_PROFILER_BEGIN_FUNC_SCOPE();
    tm_ui_style_t *uistyle = (tm_ui_style_t[]){ *uistyle_in };
    tm_rect_t canvas_r = { .w = content_r.w, .h = data->max_y };
    tm_ui_scrollview_t scrollview = {
        .rect = content_r,
        .canvas = canvas_r,
        .visibility_y = TM_UI_SCROLLBAR_VISIBILITY_WHEN_NEEDED,
    };
    tm_rect_t scroll_content_r;
    tm_rect_t rect = { content_r.x, content_r.y, 100.f, metrics_item_h};
    tm_ui_api->begin_scrollview(ui, uistyle, &scrollview, NULL, &data->scroll_y, &scroll_content_r);

    tm_ui_api->label(ui, uistyle, &(tm_ui_label_t){ .rect = rect, .text = "UI Custom Controls" });
    rect.y += metrics_item_h;
    tm_ui_circular_button_t button = {
        .text = "click me",
        .text_color = (tm_color_srgb_t){ .r = 255, .g = 255, .b = 255, .a = 255 },
        .center = (tm_vec2_t){ content_r.w / 2.f, rect.y + 85.f},
        .radius = 40.f,
        .background_color = (tm_color_srgb_t){ .r = 128, .g = 200, .b = 50, .a = 255 },
        .hover_color = (tm_color_srgb_t){ .r = 128, .g = 100, .b = 50, .a = 255 },
        .clicked_color = (tm_color_srgb_t){ .r = 128, .g = 100, .b = 150, .a = 255 }
    };
    tm_ui_custom_controls_api->circular_button(ui, uistyle, &button);

    tm_ui_api->end_scrollview(ui, NULL, &data->scroll_y, true);
    TM_PROFILER_END_FUNC_SCOPE();
}

static void tab__destroy(tm_tab_o *tab)
{
    tm_free(tab->allocator, tab, sizeof(*tab));
}

static const char *tab__title(tm_tab_o *tab, tm_ui_o *ui)
{
    return "UI Custom Controls";
}
static const char *tab__create_menu_name(void)
{
    return "UI Custom Controls";
}

static const char *tab__create_menu_category(void)
{
    return "Samples";
}

static tm_tab_i *tab__create(tm_tab_create_context_t *context, tm_ui_o *ui)
{
    tm_allocator_i *allocator = context->allocator;
    uint64_t *id = context->id;

    static tm_the_machinery_tab_vt *vt = 0;
    if (!vt)
        vt = tm_global_api_registry->get(TM_UI_CUSTOM_CONTROLS_TAB_VT_NAME);

    tm_tab_o *tab = tm_alloc(allocator, sizeof(tm_tab_o));
    *tab = (tm_tab_o){
        .tm_tab_i = {
            .vt = (tm_tab_vt *)vt,
            .inst = (tm_tab_o *)tab,
            .root_id = *id,
        },
        .allocator = allocator,
        .scroll_y = 0.f,
        .max_x = 0.f,
        .max_y = 0.f,
    };
    *id += 100000;

    return &tab->tm_tab_i;
}


tm_the_machinery_tab_vt *ui_custom_tab_vt = &(tm_the_machinery_tab_vt){
    .name = TM_UI_CUSTOM_CONTROLS_TAB_VT_NAME,
    .name_hash = TM_UI_CUSTOM_CONTROLS_TAB_VT_NAME_HASH,
    .create_menu_name = tab__create_menu_name,
    .create_menu_category = tab__create_menu_category,
    .title = tab__title,
    .ui = tab__ui,
    .create = tab__create,
    .destroy = tab__destroy,
    .run_as_job = true,
};

#pragma endregion


void load_custom_tab(struct tm_api_registry_api *reg, bool load)
{
    tm_set_or_remove_api(reg, load, TM_UI_CUSTOM_CONTROLS_TAB_VT_NAME, ui_custom_tab_vt);
    tm_add_or_remove_implementation(reg, load, TM_TAB_VT_INTERFACE_NAME, ui_custom_tab_vt);
}
