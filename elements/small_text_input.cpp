
#include "../framework_public.h"


#include <Windows.h>
#include <iostream>
#include <cmath>
#include <sstream>
#include <iomanip>


namespace ui {
	float InvLerp2 ( float a, float b, float v ) {
		return ( v - a ) / ( b - a );
	}

	void small_text_input::draw ( ) {

		this->handle ( );


		//this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 255, 255, 255, 255 ), 0.f );
		//	this->renderer->AddText ( ui::font_widgets, 13.f, ImVec2 ( this->mins.x, middle.y ), ImColor ( 255, 0, 255, 225 ), this->title.c_str() );
		ImVec2 middle = ImVec2 ( ( this->mins.x + this->maxs.x ) / 2.f, ( this->maxs.y + this->mins.y ) / 2.f );

		middle.y -= ImGui::CalcTextSize ( this->text_val.c_str ( ), 13.f, ui::font_widgets ).y / 2.f;

	
		this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 46, 49, 52, ui::focused_item == this->_id ? 211 : 45 ), 4.5f );
		this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 255, 255, 255, 15 ), 4.5f );


		ImVec2 text_pos = ImVec2 ( );

		auto text_size = ImGui::CalcTextSize ( this->text_val.c_str ( ), 11.5f, ui::font_widgets );

		text_pos.x = ( this->maxs.x + this->mins.x ) / 2.f;
		text_pos.y = ( this->maxs.y + this->mins.y ) / 2.f;
		
		text_pos.x -= text_size.x / 2.f;
		text_pos.y -= text_size.y / 2.f;

		this->renderer->AddText ( ui::font_widgets, 11.5f, text_pos, ImColor ( 255, 255, 255, 225 ), this->text_val.c_str ( ) );
	}
	void small_text_input::handle_mouse_input ( ) {
		
		if ( !this->can_focus ( ) )
			return;

		auto mouse_pos = ui::get_cursor ( );

		this->hovering = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->maxs.y );
		
	}
	void small_text_input::handle ( ) {


		handle_mouse_input ( );

		if ( this->hovering && key_down ( VK_LBUTTON ) ) {
			this->focus_it ( );
		}


		if ( this->is_focused ( ) ) {


			if ( key_pressed ( VK_ESCAPE ) || key_pressed ( VK_RETURN ) || ( !this->hovering && key_pressed ( VK_LBUTTON ) ) ) {
				this->out_of_focus ( );
			}
			else if ( key_pressed ( VK_BACK ) && !this->text_val.empty ( ) ) {
				this->text_val.pop_back ( );
			}
			else if ( this->text_val.length ( ) < 12 ) {
				if (this->input_type == text_type::floats && key_pressed ( 0xBE ) ) {
					this->text_val += ".";
					return;
				}
				for ( int i = 0x30; i <= 0x39; i++ ) {

					if ( key_pressed ( i ) )
						this->text_val += static_cast< char >( i );

				}
			}
			if ( key_pressed ( VK_RETURN ) ) {
				auto parent = static_cast< slider * >( this->parrent );
				try {
					if (this->input_type == text_type::floats) {
						auto parsed = ::atof(this->text_val.c_str());

						*(float*)parent->value = (float)parsed;
					}
					else
					{
						auto parsed = std::atoi(this->text_val.c_str());

						*(float*)parent->value = (int)parsed;
						
					}

					//std::memmove ( parent->old_value, parent->value, sizeof ( parent->value ) );
				}
				catch ( int e ) {

				}
				this->out_of_focus ( );
			}

		}
	}

	void small_text_input::update ( ) {

		if ( this->parrent->type == object_type::slider_element ) {
			


			this->maxs.x = this->parrent->maxs.x;
			this->maxs.y = this->parrent->maxs.y;

			this->mins.x = this->maxs.x - 30;
			this->mins.y = this->parrent->mins.y + 3;

				auto s = (float)*(float*)(static_cast<slider*>(this->parrent)->value);

				std::stringstream stream;
				if (s < 10.0f && this->input_type == text_type::floats)
					stream << std::fixed << std::setprecision(1) << s;
				else
					stream << std::fixed << std::setprecision(0) << s;
				this->text_val = stream.str();
			
		}
	
	}
}