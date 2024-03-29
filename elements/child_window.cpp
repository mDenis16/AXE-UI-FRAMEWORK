#include "../framework_public.h"


#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream

namespace ui {

   

	void child_window::draw_scrollbar ( ) {
		if (  this->flags & flags::scrollbar ) {

			this->renderer->AddRectFilledMultiColor ( ImVec2(this->mins.x, this->mins.y), ImVec2(this->maxs.x, this->mins.y + 25.f ), ImColor(0,0,0,25), ImColor(0,0,0, 25), ImColor(0,0,0, 15), ImColor(0,0,0,15));

			this->renderer->PushClipRect ( this->thumb_path_mins, this->thumb_path_maxs );
			this->renderer->AddRectFilled ( this->thumb_path_mins, this->thumb_path_maxs, ImColor ( 255, 0, 0, 125 ) );
			this->renderer->AddRectFilled ( this->thumb_mins, this->thumb_maxs, ImColor ( 0, 255, 255, 255 ) );
			this->renderer->PopClipRect ( );

	

			this->renderer->AddText ( ui::font_widgets, 14.f, ImVec2 ( this->maxs.x + 14, this->mins.y + 14 ), ImColor ( 255, 255, 255, 225 ), std::to_string ( this->thumb_progress ).c_str ( ) );
		}
	}
	
	void child_window::handle_scrollbar ( ) {
		if ( this->flags & flags::scrollbar  && this->children.size() > 10) {

			float windowSize = this->maxs.y - this->mins.y;

			float maxLimitSize =  this->children.size ( ) * ( this->children.back ( )->maxs.y - this->children.front ( )->mins.y ) + 55.f;

			float contentSize = ( this->children.back ( )->maxs.y - this->children.front ( )->mins.y );
			float trackSize = windowSize / 3; //80 unknown units

			float windowContentRatio = windowSize / contentSize;
			float gripSize = trackSize * windowContentRatio;


			float percentage = windowSize / contentSize;
			this->thumb_length = windowSize * percentage;

			
			this->thumb_path_mins = ImVec2( this->maxs.x - 5, this->mins.y - this->thumb_length / 2.f);
			this->thumb_path_maxs = ImVec2 ( this->maxs.x, this->maxs.y  + this->thumb_length  / 2.f);

		
	

			float s = ( contentSize / windowSize );
			float location = windowSize / 0.5f;

			float pos = ( this->maxs.y - this->mins.y ) * this->thumb_progress ;
			//m_rctThumb.Size.Height = m_rctShaft.Height * m_flThumbPerc;
			//   thumb.Location.Y += m_rctBounds.Size.Width + m_rctThumb.Location.Y;
			this->thumb_mins = ImVec2 ( this->maxs.x - 5,  this->mins.y  + pos) ;
			this->thumb_maxs = ImVec2 ( this->maxs.x, this->thumb_mins.y + this->thumb_length  );

			auto mouse_pos = ui::get_cursor ( );

			bool hovering_thumb = ( mouse_pos.x > this->thumb_path_mins.x && mouse_pos.y > this->thumb_path_mins.y && mouse_pos.x < this->thumb_path_maxs.x && mouse_pos.y < this->thumb_path_maxs.y );


			if ( ui::key_pressed ( VK_LBUTTON )  && hovering_thumb ) {
				this->modifying_thumb = true;
			}
			else if ( this->modifying_thumb && ui::key_released ( VK_LBUTTON ) ) {
				this->modifying_thumb = false;
			}

			if ( ImGui::GetIO ( ).MouseWheel < 0.0f ) {
				float old = this->thumb_progress;
				this->thumb_progress += 0.1f;

				this->thumb_progress = std::lerp ( old, this->thumb_progress, ImGui::GetIO ( ).DeltaTime * 5.5f  );

				this->max_thumb_progress = 1.f - this->thumb_length / ( this->maxs.y - this->mins.y );
				if ( this->max_thumb_progress < 0.f )
					return; /*dont handle unmodifiable scrollbar*/
				this->thumb_progress = std::clamp ( this->thumb_progress, 0.f, this->max_thumb_progress );


				float line_size = ( this->children.front ( )->maxs.y - this->children.front ( )->mins.y );

				this->scroll_progress = maximumContentSize * this->thumb_progress;

			
				for ( auto & child : this->children )
					child->update ( );

			}
			else if ( ImGui::GetIO ( ).MouseWheel > 0.0f){
				float old = this->thumb_progress;
				this->thumb_progress -= 0.1f;

				this->thumb_progress = std::lerp ( old, this->thumb_progress, ImGui::GetIO ( ).DeltaTime * 5.5f );

				this->max_thumb_progress = 1.f - this->thumb_length / ( this->maxs.y - this->mins.y );
				if ( this->max_thumb_progress < 0.f )
					return; /*dont handle unmodifiable scrollbar*/
				this->thumb_progress = std::clamp ( this->thumb_progress, 0.f, this->max_thumb_progress );


				float line_size = ( this->children.front ( )->maxs.y - this->children.front ( )->mins.y );

				this->scroll_progress = maximumContentSize * this->thumb_progress;


				for ( auto & child : this->children )
					child->update ( );

			}

			if ( this->modifying_thumb ) {

				float dist = mouse_pos.y - (this->thumb_length ) / 2.f - this->mins.y;
				dist /= ( this->maxs.y - this->mins.y );
				
				float old = this->thumb_progress;
				this->max_thumb_progress = 1.f - this->thumb_length / ( this->maxs.y - this->mins.y );
				if ( this->max_thumb_progress < 0.f )
					return; /*dont handle unmodifiable scrollbar*/

				this->thumb_progress = dist;
				this->thumb_progress = std::clamp ( this->thumb_progress, 0.f, this->max_thumb_progress );

			

				this->scroll_progress = maximumContentSize * this->thumb_progress;
				
				for ( auto & child : this->children )
					child->update ( );
			}
			else {
				maximumContentSize = ( this->children.back ( )->maxs.y - this->children.front ( )->mins.y ) + 85.f;
			}


		}
		draw_scrollbar ( );

	}

	void child_window::draw ( ) {
       

		if ( this->type_child == child_type::main_container ) {
			
			if ( !static_cast< tab * >( this->tab_assign->parrent )->selected )
				return;

			if ( !static_cast< sub_tab * >( this->tab_assign )->selected )
				return;
		}

		if ( this->type_child == child_type::tab_container ) {
			this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 0, 0, 0, 25 ), this->rounding, ImDrawCornerFlags_::ImDrawCornerFlags_Top );
			this->renderer->AddRectFilled ( this->mins, this->maxs, this->bg_color, this->rounding, ImDrawCornerFlags_::ImDrawCornerFlags_Top );
			this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 0, 0, 0, 25 ) );


			  static const std::string logo_text = "AXEUS.NET";
			  static float change_time [ 12 ];
			  static bool was_hovering [ 12 ];
			   float x = 0.f;

			   std::string str_temp = "a";
			   int i = 0;
			   auto mouse_pos = ui::get_cursor ( );

			   if ( this->should_reset || this->should_reanimate ) {
				   for ( size_t i = 0; i < 12; i++ ) {
					   change_time [ i ] = ImGui::GetTime ( ) + (i + 2) * 0.3f;
				   }
				   this->should_reset = false;
				   this->should_reanimate = false;
			   }

			   for(const auto& c : logo_text ){
				   str_temp [ 0 ] = c;

				   static float off = 130;
				   ImVec2 text_size = ImGui::CalcTextSize ( str_temp.c_str ( ), 17.f, ui::font_widgets );

				   ImVec2 char_min = ImVec2 ( this->maxs.x - off + x, this->mins.y + 10 );
				   ImVec2 char_max = ImVec2 ( this->maxs.x - off + x + text_size.x,  ( this->mins.y + this->children.front ( )->maxs.y ) / 2.f + text_size.y / 2.f );

				    bool hovering = ( mouse_pos.x > char_min.x && mouse_pos.y > char_min.y && mouse_pos.x < char_max.x && mouse_pos.y < char_max.y );

					if ( hovering != was_hovering[i] )
						change_time[i] = ImGui::GetTime ( );

					float progress = ImGui::GetTime ( ) - change_time [ i ];  progress = std::clamp ( progress * 3.5f, 0.f, 1.f );

					renderer->AddText ( ui::font_widgets, 17.f, ImVec2 ( this->maxs.x - off + x + 2.f,  this->mins.y + 10 + 2.f ), ImColor ( 0, 0, 0, ( int ) ( std::lerp ( 50, 240, hovering ? progress : 1.f - progress ) ) ), str_temp.c_str ( ) );

				   renderer->AddText ( ui::font_widgets, 17.f, ImVec2 ( this->maxs.x - off + x, this->mins.y + 10 ), ImColor ( 255, 255, 255, (int)(std::lerp(120, 255, hovering ? progress : 1.f - progress ))), str_temp.c_str() );
				  // renderer->AddRect ( char_min, char_max, ImColor ( 255, 0, 0, 255 ) );

				   float add = c == '.' ? -3.f : 3.f;
				   was_hovering [ i ] = hovering;
				   x += ImGui::CalcTextSize( str_temp.c_str(), 17.f, ui::font_widgets).y / 2.f + add;
				   i++;
			   }
			

			this->bg_color = ImColor ( 27, 27, 37, 12 );

		

			if ( !this->children.empty ( ) )
				this->renderer->AddLine ( ImVec2 ( this->mins.x, this->children.front ( )->maxs.y ), ImVec2 ( this->maxs.x, this->children.front ( )->maxs.y ), ImColor ( 0, 0, 0, 55 ));
		}
		else {
            if ( !( this->flags & flags::no_background ) ) {

				if (!( this->flags & flags::no_bg_shadow) ) {
					this->renderer->AddShadowRect ( ImVec2 ( this->mins.x + 20, this->mins.y + 20 ), ImVec2 ( this->maxs.x - 20, this->maxs.y - 20 ), ImColor ( 255, 255, 255, 255 ), 55.f, ImVec2 ( 0, 0 ), ImDrawShadowFlags_CutOutShapeBackground, 50.f, ImDrawCornerFlags_::ImDrawCornerFlags_All );
				}
                this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 0, 0, 0, 25 ), this->rounding );
				

                this->renderer->AddRectFilled ( this->mins, this->maxs, SECONDARY_COLOR, this->rounding );


            }
		//
		}
	//	this->renderer->AddShadowRect ( this->mins, this->maxs, ImColor ( 255, 255, 255, 255 ), 15.f, ImVec2 ( 0, 0 ), ImDrawShadowFlags_CutOutShapeBackground, this->rounding, ImDrawCornerFlags_::ImDrawCornerFlags_All );

		if (this->title) {
			this->renderer->AddText(ui::font_widgets, 14.f, ImVec2(this->mins.x + 14, this->mins.y + 14), ImColor(0, 0, 0, 225), this->title->data());
			this->renderer->AddText(ui::font_widgets, 14.f, ImVec2(this->mins.x + 13, this->mins.y + 13), ImColor(255, 255, 255, 225), this->title->data());
		}


		if ( flags & flags::hide_overflow ) {
			// this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 255, 255, 255, 255 ), this->rounding );
			this->renderer->PushClipRect ( this->mins, this->maxs, false );
			//  this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 255, 255,255, 255 ), this->rounding );
		}
		if ( flags & flags::render_forward ) {
			for ( auto & child : children )
				child->draw ( );
		}
		else {
			for ( size_t i = this->children.size ( ) - 1; i != ( size_t ) -1; i-- )
				this->children.at ( i )->draw ( );
		}
		if ( flags & flags::hide_overflow ) {
			this->renderer->PopClipRect ( );

		}
		handle_scrollbar ( );
	}
	void child_window::handle ( ) {

	}
	void child_window::handle_mouse_input ( ) {
		auto mouse_pos = ui::get_cursor ( );

		this->hovering = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->maxs.y );
	}
	void child_window::handle_tab_children ( ) {
		this->mins.x = this->parrent->mins.x;
		this->maxs.x = this->parrent->maxs.x;

		this->mins.y = this->parrent->mins.y;
		this->maxs.y = this->mins.y + 90;

	}
	void child_window::update ( ) {

		this->should_reanimate = false;

		this->width = ( percent_width / 100.f ) * this->parrent->width;
		this->height = ( percent_height / 100.f ) * this->parrent->height;
	//	if ( this->children.size ( ) > 2 )
		//	this->height = this->children.size ( ) * 22.9f;
	

       if ( this->parrent->type == panel_element ) {
       


            this->mins = this->parrent->mins;
            this->maxs = this->parrent->maxs;

		

           // handle_tab_children ( );

            for ( auto & child : this->children )
                child->update ( );

            return;
       
        }
		if ( this->type_child == child_type::tab_container ) {

			handle_tab_children ( );

			for ( auto & child : this->children )
				child->update ( );

			return;
		}
		if ( this->type_child == child_type::main_container ) {
			this->mins = this->parrent->mins;
			this->mins.y += 90;
			this->maxs = this->parrent->maxs;

			this->width = ( percent_width / 100.f ) * ( this->maxs.x - this->mins.x );
			this->height = ( percent_height / 100.f ) * ( this->maxs.y - this->mins.y );

 			for ( auto & child : this->children )
				child->update ( );
			return;
		}


		if ( this->_float > 0 ) {
			if ( _float == float_side::bottom ) {
				this->mins.x = this->parrent->mins.x;
				this->mins.y = this->parrent->maxs.y;

				this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );

            }
            else if ( _float == float_side::top ) {
                this->mins.x = this->parrent->mins.x;
                this->mins.y = this->parrent->mins.y;

                this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );
            }
		}
		else {
			if ( this->parrent->flex == flex_direction::automatic ) {
				auto _parrent = static_cast< child_window * >( this->parrent );

				if ( this->index > 0 ) {
					auto & last = this->parrent->children.at ( this->index - 1 );

					this->mins.x = last->mins.x;
					this->mins.y = last->maxs.y + _parrent->padding; ;

				
					this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );

					if ( this->maxs.y > this->parrent->maxs.y - 5 && !this->tab_assign ) {

						/*	auto spacing_left = ( this->parrent->width - ( this->width * 2.f + _parrent->padding ) ) / 2.f;
							this->mins.y = this->parrent->mins.x + spacing_left;
							this->mins.x = last->maxs.x + _parrent->padding;
							*/

						this->mins.x = last->maxs.x + this->parrent->padding;
						this->mins.y = this->parrent->mins.y + this->parrent->padding;


						this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );
					}


				}
				else {

					auto spacing_left = ( this->parrent->width - ( this->width * 2.f + _parrent->padding ) ) / 2.f;
					this->mins.x = this->parrent->mins.x + spacing_left;
					this->mins.y = this->parrent->mins.y + _parrent->padding;
				}

				this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );

			}
			else if ( this->parrent->flex == flex_direction::block ) {
				auto _parrent = static_cast< child_window * >( this->parrent );

				if ( this->index > 0 ) {
					try {
						auto & back = this->parrent->children.at ( this->index - 1 );
						this->mins.x = _parrent->mins.x + this->padding;
						this->maxs.x = _parrent->maxs.x - this->padding;


						this->mins.y = back->maxs.y + _parrent->padding;
						this->maxs.y = this->mins.y + this->height;
					}
					catch ( int ex ) {

					}
				}
				else {
					this->mins.x = _parrent->mins.x + this->padding;
					this->maxs.x = _parrent->maxs.x - this->padding;


					this->mins.y = _parrent->mins.y + _parrent->padding;
					this->maxs.y = this->mins.y + this->height;
				}
			}
			else {
				this->mins = this->parrent->mins;

				if ( this->index > 0 ) {
					try {
						auto & back = this->parrent->children.at ( this->index - 1 );
						this->mins.y = back->maxs.y;
					}
					catch ( int ex ) {

					}
				}

				this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );
			}
		}

		
		for ( auto & child : this->children )
			child->update ( );

	}
}
/*
auto _parrent = static_cast< child_window * >( this->parrent );

				if ( this->index > 0 ) {
					auto & last = this->parrent->children.at ( this->index - 1 );

					this->mins.x = last->maxs.x + _parrent->padding;
					this->mins.y = last->mins.y;

					this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );

					if ( this->maxs.x > this->parrent->maxs.x ) {

						auto spacing_left = ( this->parrent->width - ( this->width * 2.f + _parrent->padding ) ) / 2.f;
						this->mins.x = this->parrent->mins.x + spacing_left;
						this->mins.y = last->maxs.y + _parrent->padding;


					}


				}
				else {

					auto spacing_left = ( this->parrent->width - ( this->width * 2.f + _parrent->padding ) ) / 2.f;
					this->mins.x = this->parrent->mins.x + spacing_left;
					this->mins.y = this->parrent->mins.y + _parrent->padding;
				}

				this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );
*/