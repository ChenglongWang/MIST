// 
// Copyright (c) 2003-2009, MIST Project, Intelligent Media Integration COE, Nagoya University
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
// 
// 3. Neither the name of the Nagoya University nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 

// generated by Fast Light User Interface Designer (fluid) version 1.0106

#include "volr_image_window.h"

inline void parameter_window::cb_OK_i(Fl_Return_Button*, void*) {
  val = 1;
}
void parameter_window::cb_OK(Fl_Return_Button* o, void* v) {
  ((parameter_window*)(o->parent()->user_data()))->cb_OK_i(o,v);
}

inline void parameter_window::cb_Cancel_i(Fl_Button*, void*) {
  val = 0;
}
void parameter_window::cb_Cancel(Fl_Button* o, void* v) {
  ((parameter_window*)(o->parent()->user_data()))->cb_Cancel_i(o,v);
}

parameter_window::parameter_window() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = param_window = new Fl_Double_Window(391, 105, "Open Slice Parameter");
    w = o;
    o->user_data((void*)(this));
    { Fl_Value_Input* o = width = new Fl_Value_Input(55, 10, 50, 25, "X size :");
      o->labeltype(FL_ENGRAVED_LABEL);
      o->labelfont(8);
      o->maximum(1000);
      o->value(512);
      o->when(FL_WHEN_RELEASE);
    }
    { Fl_Value_Input* o = height = new Fl_Value_Input(55, 40, 50, 25, "Y size :");
      o->labeltype(FL_ENGRAVED_LABEL);
      o->labelfont(8);
      o->maximum(1000);
      o->value(512);
    }
    { Fl_Value_Input* o = depth = new Fl_Value_Input(55, 70, 50, 25, "Z size :");
      o->labeltype(FL_ENGRAVED_LABEL);
      o->labelfont(8);
      o->maximum(1000);
      o->value(400);
    }
    { Fl_Value_Input* o = sizeX = new Fl_Value_Input(170, 10, 50, 25, "Real X :");
      o->labeltype(FL_ENGRAVED_LABEL);
      o->labelfont(8);
      o->maximum(100);
      o->value(0.625);
    }
    { Fl_Value_Input* o = sizeY = new Fl_Value_Input(170, 40, 50, 25, "Real Y :");
      o->labeltype(FL_ENGRAVED_LABEL);
      o->labelfont(8);
      o->maximum(100);
      o->value(0.625);
    }
    { Fl_Value_Input* o = sizeZ = new Fl_Value_Input(170, 70, 50, 25, "Real Z :");
      o->labeltype(FL_ENGRAVED_LABEL);
      o->labelfont(8);
      o->maximum(100);
      o->value(1);
    }
    { Fl_Value_Input* o = offset = new Fl_Value_Input(315, 10, 65, 25, "CT offset :");
      o->labeltype(FL_ENGRAVED_LABEL);
      o->labelfont(8);
      o->minimum(-2000);
      o->maximum(7999);
    }
    { Fl_Return_Button* o = OK = new Fl_Return_Button(235, 60, 70, 35, "OK");
      o->labelsize(16);
      o->callback((Fl_Callback*)cb_OK);
    }
    { Fl_Button* o = Cancel = new Fl_Button(310, 60, 70, 35, "Cancel");
      o->labelsize(16);
      o->callback((Fl_Callback*)cb_Cancel);
    }
    o->set_modal();
    o->end();
  }
  val = -1;
}

int parameter_window::value() {
  return(val);
}

void parameter_window::hide() {
  param_window->hide();
}

bool parameter_window::show() {
  param_window->show();
  while(value()<0) Fl::wait(0);
  hide( );
  Fl::wait(0);
  return( value( ) ? true : false );
}

inline void volr_image_window::cb_Open_i(Fl_Menu_*, void*) {
  volr_image->read_image( this );
}
void volr_image_window::cb_Open(Fl_Menu_* o, void* v) {
  ((volr_image_window*)(o->parent()->user_data()))->cb_Open_i(o,v);
}

inline void volr_image_window::cb_Save_i(Fl_Menu_*, void*) {
  volr_image->write_image( this );
}
void volr_image_window::cb_Save(Fl_Menu_* o, void* v) {
  ((volr_image_window*)(o->parent()->user_data()))->cb_Save_i(o,v);
}

inline void volr_image_window::cb_Render_i(Fl_Menu_*, void*) {
  volr_image->render_test( this );
}
void volr_image_window::cb_Render(Fl_Menu_* o, void* v) {
  ((volr_image_window*)(o->parent()->user_data()))->cb_Render_i(o,v);
}

Fl_Menu_Item volr_image_window::menu_[] = {
 {"&File", 0,  0, 0, 64, 0, 0, 14, 56},
 {"&Open", 0,  (Fl_Callback*)volr_image_window::cb_Open, 0, 0, 0, 0, 14, 56},
 {"&Save", 0,  (Fl_Callback*)volr_image_window::cb_Save, 0, 0, 0, 0, 14, 56},
 {0,0,0,0,0,0,0,0,0},
 {"&Edit", 0,  0, 0, 64, 0, 0, 14, 56},
 {"&Render Test", 0,  (Fl_Callback*)volr_image_window::cb_Render, 0, 0, 0, 0, 14, 56},
 {0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0}
};

inline void volr_image_window::cb_512x512_i(Fl_Menu_*, void*) {
  volr_image->high_reso( 512 );
}
void volr_image_window::cb_512x512(Fl_Menu_* o, void* v) {
  ((volr_image_window*)(o->parent()->parent()->user_data()))->cb_512x512_i(o,v);
}

inline void volr_image_window::cb_256x256_i(Fl_Menu_*, void*) {
  volr_image->high_reso( 256 );
}
void volr_image_window::cb_256x256(Fl_Menu_* o, void* v) {
  ((volr_image_window*)(o->parent()->parent()->user_data()))->cb_256x256_i(o,v);
}

inline void volr_image_window::cb_128x128_i(Fl_Menu_*, void*) {
  volr_image->high_reso( 128 );
}
void volr_image_window::cb_128x128(Fl_Menu_* o, void* v) {
  ((volr_image_window*)(o->parent()->parent()->user_data()))->cb_128x128_i(o,v);
}

Fl_Menu_Item volr_image_window::menu_high_reso[] = {
 {"512x512", 0,  (Fl_Callback*)volr_image_window::cb_512x512, 0, 0, 0, 0, 14, 56},
 {"256x256", 0,  (Fl_Callback*)volr_image_window::cb_256x256, 0, 0, 0, 0, 14, 56},
 {"128x128", 0,  (Fl_Callback*)volr_image_window::cb_128x128, 0, 0, 0, 0, 14, 56},
 {0,0,0,0,0,0,0,0,0}
};

inline void volr_image_window::cb_512x5121_i(Fl_Menu_*, void*) {
  volr_image->low_reso( 512 );
}
void volr_image_window::cb_512x5121(Fl_Menu_* o, void* v) {
  ((volr_image_window*)(o->parent()->parent()->user_data()))->cb_512x5121_i(o,v);
}

inline void volr_image_window::cb_256x2561_i(Fl_Menu_*, void*) {
  volr_image->low_reso( 256 );
}
void volr_image_window::cb_256x2561(Fl_Menu_* o, void* v) {
  ((volr_image_window*)(o->parent()->parent()->user_data()))->cb_256x2561_i(o,v);
}

inline void volr_image_window::cb_128x1281_i(Fl_Menu_*, void*) {
  volr_image->low_reso( 128 );
}
void volr_image_window::cb_128x1281(Fl_Menu_* o, void* v) {
  ((volr_image_window*)(o->parent()->parent()->user_data()))->cb_128x1281_i(o,v);
}

Fl_Menu_Item volr_image_window::menu_low_reso[] = {
 {"512x512", 0,  (Fl_Callback*)volr_image_window::cb_512x5121, 0, 0, 0, 0, 14, 56},
 {"256x256", 0,  (Fl_Callback*)volr_image_window::cb_256x2561, 0, 0, 0, 0, 14, 56},
 {"128x128", 0,  (Fl_Callback*)volr_image_window::cb_128x1281, 0, 0, 0, 0, 14, 56},
 {0,0,0,0,0,0,0,0,0}
};

inline void volr_image_window::cb_Specular_i(Fl_Check_Button* o, void*) {
  volr_image->specular( o->value( ) != 0 );
}
void volr_image_window::cb_Specular(Fl_Check_Button* o, void* v) {
  ((volr_image_window*)(o->parent()->parent()->user_data()))->cb_Specular_i(o,v);
}

inline void volr_image_window::cb_Termination_i(Fl_Value_Slider* o, void*) {
  volr_image->termination( o->value( ) );
}
void volr_image_window::cb_Termination(Fl_Value_Slider* o, void* v) {
  ((volr_image_window*)(o->parent()->parent()->user_data()))->cb_Termination_i(o,v);
}

inline void volr_image_window::cb_Sampling_i(Fl_Value_Slider* o, void*) {
  volr_image->sampling_step( o->value( ) );
}
void volr_image_window::cb_Sampling(Fl_Value_Slider* o, void* v) {
  ((volr_image_window*)(o->parent()->parent()->user_data()))->cb_Sampling_i(o,v);
}

inline void volr_image_window::cb_Light_i(Fl_Value_Slider* o, void*) {
  volr_image->light_attenuation( o->value( ) / 10000.0 );
}
void volr_image_window::cb_Light(Fl_Value_Slider* o, void* v) {
  ((volr_image_window*)(o->parent()->parent()->user_data()))->cb_Light_i(o,v);
}

inline void volr_image_window::cb_Field_i(Fl_Value_Slider* o, void*) {
  volr_image->fovy( o->value( ) );
}
void volr_image_window::cb_Field(Fl_Value_Slider* o, void* v) {
  ((volr_image_window*)(o->parent()->parent()->user_data()))->cb_Field_i(o,v);
}

inline void volr_image_window::cb_OUT_i(Fl_Button* o, void*) {
  if( o->value( ) )
{
o->label( "IN" );
volr_image->inside_mode( true );
}
else
{
o->label( "OUT" );
volr_image->inside_mode( false );
};
}
void volr_image_window::cb_OUT(Fl_Button* o, void* v) {
  ((volr_image_window*)(o->parent()->user_data()))->cb_OUT_i(o,v);
}

volr_image_window::volr_image_window( ) {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = image_window = new Fl_Double_Window(421, 561);
    w = o;
    o->user_data((void*)(this));
    { Fl_Group* o = new Fl_Group(5, 30, 410, 410, "VolumeRendering");
      o->box(FL_PLASTIC_DOWN_FRAME);
      o->labeltype(FL_NO_LABEL);
      { volr_draw_area* o = volr_image = new volr_draw_area(10, 35, 400, 400, "label");
        o->box(FL_FLAT_BOX);
        o->color(FL_DARK3);
        o->selection_color(FL_BACKGROUND_COLOR);
        o->labeltype(FL_NORMAL_LABEL);
        o->labelfont(0);
        o->labelsize(14);
        o->labelcolor(FL_BLACK);
        o->align(FL_ALIGN_CENTER);
        o->when(FL_WHEN_RELEASE);
        Fl_Group::current()->resizable(o);
      }
      o->end();
      Fl_Group::current()->resizable(o);
    }
    { Fl_Menu_Bar* o = new Fl_Menu_Bar(0, 0, 100, 25);
      o->box(FL_FLAT_BOX);
      o->menu(menu_);
    }
    { Fl_Progress* o = progress_bar = new Fl_Progress(90, 5, 295, 20);
      o->box(FL_PLASTIC_THIN_UP_BOX);
      o->selection_color((Fl_Color)1);
      o->hide();
    }
    { Fl_Group* o = new Fl_Group(120, 445, 295, 110);
      { Fl_Choice* o = high_reso = new Fl_Choice(120, 445, 85, 25, "High:");
        o->down_box(FL_BORDER_BOX);
        o->menu(menu_high_reso);
      }
      { Fl_Choice* o = low_reso = new Fl_Choice(245, 445, 85, 25, "Low:");
        o->down_box(FL_BORDER_BOX);
        o->menu(menu_low_reso);
      }
      { Fl_Check_Button* o = new Fl_Check_Button(335, 445, 25, 25, "Specular");
        o->down_box(FL_PLASTIC_DOWN_BOX);
        o->value(1);
        o->callback((Fl_Callback*)cb_Specular);
      }
      { Fl_Value_Slider* o = new Fl_Value_Slider(120, 475, 295, 20, "Termination:");
        o->type(1);
        o->box(FL_PLASTIC_DOWN_BOX);
        o->minimum(0.01);
        o->value(0.01);
        o->callback((Fl_Callback*)cb_Termination);
        o->align(FL_ALIGN_LEFT);
      }
      { Fl_Value_Slider* o = new Fl_Value_Slider(120, 495, 295, 20, "Sampling Step:");
        o->type(1);
        o->box(FL_PLASTIC_DOWN_BOX);
        o->minimum(0.1);
        o->value(1);
        o->callback((Fl_Callback*)cb_Sampling);
        o->align(FL_ALIGN_LEFT);
      }
      { Fl_Value_Slider* o = new Fl_Value_Slider(120, 515, 295, 20, "Light Attenuation:");
        o->type(1);
        o->box(FL_PLASTIC_DOWN_BOX);
        o->maximum(100);
        o->step(1);
        o->callback((Fl_Callback*)cb_Light);
        o->align(FL_ALIGN_LEFT);
      }
      { Fl_Value_Slider* o = new Fl_Value_Slider(120, 535, 295, 20, "Field of View Y:");
        o->type(1);
        o->box(FL_PLASTIC_DOWN_BOX);
        o->minimum(10);
        o->maximum(160);
        o->step(1);
        o->value(80);
        o->callback((Fl_Callback*)cb_Field);
        o->align(FL_ALIGN_LEFT);
      }
      o->end();
    }
    { Fl_Button* o = new Fl_Button(390, 5, 25, 25, "OUT");
      o->type(1);
      o->box(FL_NO_BOX);
      o->labelsize(10);
      o->labelcolor(FL_SELECTION_COLOR);
      o->callback((Fl_Callback*)cb_OUT);
    }
    o->end();
  }
  high_reso->value( 1 );
high_reso->do_callback( );
  low_reso->value( 2 );
low_reso->do_callback( );
}

void volr_image_window::show() {
  image_window->show();
}
