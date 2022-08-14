#include <nuklear.h>
#include "nuk_controls.h"

void nk::TStatusBar::draw(nk_context * ctx)
{
	if (nk_begin(ctx, name.c_str(),
		nk_rectf(0.0f, (float)(window_height - status_height),
		(float)window_width, (float)status_height),
		NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR
	))
	{
		nk_layout_row_dynamic(ctx, ctx->current->bounds.h, 1);
		nk_label(ctx, text.c_str(), NK_TEXT_CENTERED);
	}
	nk_end(ctx);
}

void nk::TGroupBox::draw(nk_context * ctx)
{
	nk_group_begin_titled(ctx, name.c_str(), title.c_str(), nk_panel_flags::NK_WINDOW_BORDER);
	nk_layout_row_begin(ctx, nk_layout_format::NK_STATIC, 20, 1);
	for (auto& child : component_funcs)
	{
		child(ctx);
	}
	nk_layout_row_end(ctx);
	nk_group_end(ctx);
}
