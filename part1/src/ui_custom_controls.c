#include "ui_custom_controls.h"
#include "ui_custom_controls_loader.h"

#include <foundation/api_registry.h>
#include <foundation/math.inl>
#include <foundation/log.h>
#include <foundation/rect.inl>
#include <foundation/unicode.h>

#include <plugins/ui/draw2d.h>
#include <plugins/ui/ui.h>
#include <plugins/ui/ui_custom.h>

static tm_strhash_t TM_UI_ACTIVE_DATA__CIRCULAR_BUTTON = TM_STATIC_HASH("tm_ui_circular_button", 0x21858d69c405701fULL);

static inline float middle_baseline(float y, float h, const tm_font_t *font, float scale)
{
    uint32_t idx = tm_font_api->glyph_set_from_scale(font, scale, &scale);
    float margin = h - font->ascent[idx] * scale - font->descent[idx] * scale;
    return y + margin / 2 + font->ascent[idx] * scale;
}

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
        TM_LOG("active data -> name: %s, frames_active: %u\n", active->name, active->frames_active);
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

    // Inscribe a quad in button circle
    const float side = c->radius * sqrtf(2);
    tm_rect_t text_rect = tm_rect_center_dim(c->center, (tm_vec2_t){ side, side });

    tm_ui_api->reserve_draw_memory(ui);
    style.clip = tm_draw2d_api->add_sub_clip_rect(uib.vbuffer, style.clip, text_rect);

    // Get glyphs from our text
    uint16_t glyphs[128];
    uint32_t n = 0;
    {
        uint32_t codepoints[128];
        n = tm_unicode_api->utf8_decode_n(codepoints, 128, tm_or(c->text, ""));
        tm_font_api->glyphs(style.font->info, glyphs, codepoints, 128);
    }
    tm_vec2_t text_pos = {
        .x = c->center.x - side / 2.f,
        .y = middle_baseline(text_rect.y, text_rect.h, style.font->info, 1.f),
    };
    style.color = c->text_color;
    tm_draw2d_api->draw_glyphs(uib.vbuffer, uib.ibuffers[uistyle->buffer], &style, text_pos, glyphs, n);

    return clicked;

}

struct tm_ui_custom_controls_api *tm_ui_custom_controls_api = &(struct tm_ui_custom_controls_api){
    .circular_button = circular_button,
};

void load_custom_controls(struct tm_api_registry_api* reg, bool load)
{
    tm_set_or_remove_api(reg, load, TM_UI_CUSTOM_CONTROLS_API_NAME, tm_ui_custom_controls_api);
}
