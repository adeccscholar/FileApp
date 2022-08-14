#ifndef NUK_CONTROLS_H
#define NUK_CONTROLS_H
#include <vector>
#include <string>
#include <functional>
#include <MyStdTypes.h>
#include "nk_text_alignment.h"
#include "nk_rect.h"
struct nk_context;

namespace nk
{
	class IComponent
	{
	public:
		std::string name;
		virtual ~IComponent() = default;
	};
	struct TMemo : public IComponent
	{
		std::vector<std::string> data;
		//setText?
	};

	struct TLabel : public IComponent
	{
		std::string text;
	};

	struct TEdit : public IComponent
	{
		std::string text;
	};

	struct TCombobox : public IComponent
	{
		std::string text;
		std::vector<std::string> items;
		int itemindex = -1;
		size_t count() const { return items.size(); }
	};

	struct TListbox : public IComponent
	{
		std::vector<std::string> items;
		int itemindex = -1;
		size_t count() const { return items.size(); }
	};

	struct TCheckbox : public IComponent
	{
		std::string text;
		int checkstate = 0; //tristate?
	};

	struct TButton : public IComponent
	{
		std::string text;
		std::function<void()> onClick = nullptr;
	};

	struct TStatusBar : public IComponent
	{
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
		void draw(nk_context* ctx);
	};

	struct TGroupBox : public IComponent
	{
		std::string title;
		std::vector<std::function<void(nk_context*)>> component_funcs;
		void draw(nk_context* ctx);
	};

	struct TGrid : public IComponent
	{
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

	struct NKForm
	{
		std::string name;
		std::string title;
		std::vector<IComponent*> fields;
		IComponent* FindComponent(std::string const& strField)
		{
			for (IComponent* c : fields)
			{
				if (!c) { continue; }
				if (c->name != strField) { continue; }
				return c;
			}
			return nullptr;
		}
	};
}
#endif