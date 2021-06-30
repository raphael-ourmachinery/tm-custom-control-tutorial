#pragma once

#include <foundation/api_types.h>

#define TM_UI_CUSTOM_CONTROLS_API_NAME "tm_ui_custom_controls_api"

struct tm_ui_o;
struct tm_ui_style_t;

typedef struct tm_ui_circular_button_data_t 
{
    const char *name;
    uint32_t frames_active;
} tm_ui_circular_button_data_t;

typedef struct tm_ui_circular_button_t
{
    uint64_t id;

    tm_vec2_t center;
    float radius;
    tm_color_srgb_t background_color;
    tm_color_srgb_t hover_color;
    tm_color_srgb_t clicked_color;

    uint32_t icon;
    const char *text;
    const struct tm_color_srgb_t text_color;
} tm_ui_circular_button_t;

struct tm_ui_custom_controls_api
{
    bool (*circular_button)(struct tm_ui_o *ui, const struct tm_ui_style_t *style, const tm_ui_circular_button_t *c);
};

extern struct tm_ui_custom_controls_api *tm_ui_custom_controls_api;
