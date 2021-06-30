#pragma once

#include <foundation/api_types.h>

#define TM_UI_CUSTOM_CONTROLS_API_NAME "tm_ui_custom_controls_api"

struct tm_ui_o;
struct tm_ui_style_t;

typedef struct tm_ui_circular_button_t
{
    uint64_t id;
} tm_ui_circular_button_t;

struct tm_ui_custom_controls_api
{
    bool (*circular_button)(struct tm_ui_o *ui, const struct tm_ui_style_t *style, const tm_ui_circular_button_t *c);
};

extern struct tm_ui_custom_controls_api *tm_ui_custom_controls_api;
