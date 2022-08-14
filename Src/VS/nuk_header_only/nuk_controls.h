#ifndef NUK_CONTROLS_H
#define NUK_CONTROLS_H
#include <vector>
#include <string>
#include <functional>

namespace nk
{

	struct TMemo
	{
		std::string name;
		std::vector<std::string> data;
	};

	struct TLabel
	{
		std::string name;
		std::string text;
	};

	struct TEdit
	{
		std::string name;
		std::string text;
	};

	struct TCombobox
	{
		std::string name;
		std::string text;
		std::vector<std::string> items;
		size_t count() const { return items.size(); }
	};

	struct TListbox
	{
		std::string name;
		std::vector<std::string> items;
		size_t count() const { return items.size(); }
	};

	struct TCheckbox
	{
		std::string name;
		std::string text;
		int checkstate = 0; //tristate?
	};

	struct TButton
	{
		std::string name;
		std::string text;
		std::function<void()> onClick = nullptr;
	};

	struct TStatusBar
	{
		std::string name;
		std::string text;
		int& window_height;
		int& window_width;
		int status_height;
		TStatusBar() = delete;
		TStatusBar(const TStatusBar&) = delete;
		TStatusBar(int& h, int& w)
			:
			window_height(h),
			window_width(w),
			status_height(30)
		{}
		void draw(nk_context* ctx)
		{
			if (nk_begin(ctx, name.c_str(),
				nk_rect(0.0f, (float)(window_height - status_height),
						(float)window_width, (float)status_height),
				NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR
			))
			{
				nk_layout_row_dynamic(ctx, ctx->current->bounds.h, 1);
				nk_label(ctx, text.c_str(), NK_TEXT_CENTERED);
			}
			nk_end(ctx);
		}
	};

	struct TGroupBox
	{
		std::string name;
		std::string title;
		std::vector<std::function<void(nk_context*)>> component_funcs;
		void draw(nk_context* ctx)
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
	};

	struct TGrid {
		struct THeadItem {
			std::string caption;
			int width = 10;
			nk_text_alignment nk_alignment = nk_text_alignment::NK_TEXT_LEFT; //translated, ref in MyForm.h
			EMyAlignmentType alignment = EMyAlignmentType::left; //<-framework
		};
		std::vector<THeadItem> Columns;
		std::vector<std::vector<std::string>> Rows;
		void clear()
		{
			Columns.clear();
			Rows.clear();
		}
		int rowCount() const
		{
			return static_cast<int>( Rows.size() );
		}
		int colCount() const
		{
			return static_cast<int>( Columns.size() );
		}
	};

}
#endif