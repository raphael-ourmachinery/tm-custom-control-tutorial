#include "ui_custom_controls.h"
#include "ui_custom_controls_loader.h"

#include <foundation/api_registry.h>
#include <foundation/math.inl>
#include <foundation/log.h>

#include <plugins/ui/draw2d.h>
#include <plugins/ui/ui.h>
#include <plugins/ui/ui_custom.h>

static tm_strhash_t TM_UI_ACTIVE_DATA__CIRCULAR_BUTTON = TM_STATIC_HASH("tm_ui_circular_button", 0x21858d69c405701fULL);

bool tm_vec2_in_circle(tm_vec2_t pos, tm_vec2_t center, float radius)
{
    float d = tm_vec2_length(tm_vec2_sub(pos, center));

    if (d < radius)
        return true;

    return false;
}

bool circular_button(struct tm_ui_o *ui, const struct tm_ui_style_t *uistyle, const tm_ui_circular_button_t *c)
{
    // tm_ui_buffer_t contains information needed when creating a custom control
    tm_ui_buffers_t uib = tm_ui_api->buffers(ui);
    const uint64_t id = c->id ? c->id : tm_ui_api->make_id(ui);
    // is_active will return a pointer for user defined data up to 16KB
    tm_ui_circular_button_data_t *active = (tm_ui_circular_button_data_t *)tm_ui_api->is_active(ui, id, TM_UI_ACTIVE_DATA__CIRCULAR_BUTTON);
    if (active) {
        active->frames_active++;
    }

    // convert tm_ui_style_t to tm_draw2d_style_t
    tm_draw2d_style_t style;
    tm_ui_api->to_draw_style(ui, &style, uistyle);
    style.color = c->background_color;

    bool clicked = false;
    bool inside = tm_vec2_in_circle(uib.input->mouse_pos, c->center, c->radius);
    if (inside)
        uib.activation->next_hover = id;

    if (uib.activation->hover == id && uib.input->left_mouse_pressed) {
        active = tm_ui_api->set_active(ui, id, TM_UI_ACTIVE_DATA__CIRCULAR_BUTTON);
        if (active)
            *active = (tm_ui_circular_button_data_t){ .name = "circular_button", .frames_active = 0 };
        tm_ui_api->set_responder_chain(ui, 0);
    }

    if (active && uib.input->left_mouse_released) {
        clicked = inside;
        tm_ui_api->clear_active(ui);
    }

    if (inside) {
        if (active)
            style.color = c->clicked_color;
        else if (uib.activation->hover == id)
            style.color = c->hover_color;
    }

    tm_ui_api->reserve_draw_memory(ui);
    tm_draw2d_api->fill_circle(uib.vbuffer, uib.ibuffers[uistyle->buffer], &style, c->center, c->radius);
    tm_draw2d_api->stroke_circle(uib.vbuffer, uib.ibuffers[uistyle->buffer], &style, c->center, c->radius);

    return clicked;

}

struct tm_ui_custom_controls_api *tm_ui_custom_controls_api = &(struct tm_ui_custom_controls_api){
    .circular_button = circular_button,
};

void load_custom_controls(struct tm_api_registry_api* reg, bool load)
{
    tm_set_or_remove_api(reg, load, TM_UI_CUSTOM_CONTROLS_API_NAME, tm_ui_custom_controls_api);
}
