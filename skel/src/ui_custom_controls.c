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

bool circular_button(struct tm_ui_o *ui, const struct tm_ui_style_t *uistyle, const tm_ui_circular_button_t *c)
{
    return false;
}

struct tm_ui_custom_controls_api *tm_ui_custom_controls_api = &(struct tm_ui_custom_controls_api){
    .circular_button = circular_button,
};

void load_custom_controls(struct tm_api_registry_api* reg, bool load)
{
    tm_set_or_remove_api(reg, load, TM_UI_CUSTOM_CONTROLS_API_NAME, tm_ui_custom_controls_api);
}
