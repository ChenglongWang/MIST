// generated by Fast Light User Interface Designer (fluid) version 1.0104

#include "image_test_window.h"

inline void image_test_window::cb_Read_i(Fl_Button*, void*) {
  const char *filename = fl_file_chooser( "Open JPEG", "JPEG File (*.{jpg,jpeg})", "" );
if( filename == NULL ) return;

read_jpeg_test( filename );

draw_area->redraw( );
Fl::wait( 0 );
}
void image_test_window::cb_Read(Fl_Button* o, void* v) {
  ((image_test_window*)(o->parent()->user_data()))->cb_Read_i(o,v);
}

inline void image_test_window::cb_Read1_i(Fl_Button*, void*) {
  const char *filename = fl_file_chooser( "Open PNG", "PNG File (*.png)", "" );
if( filename == NULL ) return;

read_png_test( filename );

draw_area->redraw( );
Fl::wait( 0 );
}
void image_test_window::cb_Read1(Fl_Button* o, void* v) {
  ((image_test_window*)(o->parent()->user_data()))->cb_Read1_i(o,v);
}

inline void image_test_window::cb_Write_i(Fl_Button*, void*) {
  const char *filename = fl_file_chooser( "Save JPEG", "JPEG File (*.{jpg,jpeg})", "" );
if( filename == NULL ) return;

write_jpeg_test( filename );
}
void image_test_window::cb_Write(Fl_Button* o, void* v) {
  ((image_test_window*)(o->parent()->user_data()))->cb_Write_i(o,v);
}

inline void image_test_window::cb_Write1_i(Fl_Button*, void*) {
  const char *filename = fl_file_chooser( "Save PNG", "PNG File (*.png)", "" );
if( filename == NULL ) return;

write_png_test( filename );
}
void image_test_window::cb_Write1(Fl_Button* o, void* v) {
  ((image_test_window*)(o->parent()->user_data()))->cb_Write1_i(o,v);
}

inline void image_test_window::cb_Read2_i(Fl_Button*, void*) {
  const char *filename = fl_file_chooser( "Open TIFF", "TIFF File (*.{tif,tiff})", "" );
if( filename == NULL ) return;

read_tiff_test( filename );

draw_area->redraw( );
Fl::wait( 0 );
}
void image_test_window::cb_Read2(Fl_Button* o, void* v) {
  ((image_test_window*)(o->parent()->user_data()))->cb_Read2_i(o,v);
}

inline void image_test_window::cb_Write2_i(Fl_Button*, void*) {
  const char *filename = fl_file_chooser( "Save TIFF", "TIFF File (*.{tif,tiff})", "" );
if( filename == NULL ) return;

write_tiff_test( filename );
}
void image_test_window::cb_Write2(Fl_Button* o, void* v) {
  ((image_test_window*)(o->parent()->user_data()))->cb_Write2_i(o,v);
}

inline void image_test_window::cb_Read3_i(Fl_Button*, void*) {
  const char *filename = fl_file_chooser( "Open BMP", "BMP File (*.bmp)", "" );
if( filename == NULL ) return;

read_bmp_test( filename );

draw_area->redraw( );
Fl::wait( 0 );
}
void image_test_window::cb_Read3(Fl_Button* o, void* v) {
  ((image_test_window*)(o->parent()->user_data()))->cb_Read3_i(o,v);
}

inline void image_test_window::cb_Write3_i(Fl_Button*, void*) {
  const char *filename = fl_file_chooser( "Save BMP", "BMP File (*.bmp)", "" );
if( filename == NULL ) return;

write_bmp_test( filename );
}
void image_test_window::cb_Write3(Fl_Button* o, void* v) {
  ((image_test_window*)(o->parent()->user_data()))->cb_Write3_i(o,v);
}

image_test_window::image_test_window() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = main_window = new Fl_Double_Window(551, 625);
    w = o;
    o->user_data((void*)(this));
    { Fl_Group* o = new Fl_Group(5, 5, 540, 530);
      o->box(FL_DOWN_BOX);
      { image_draw_area* o = draw_area = new image_draw_area(10, 10, 530, 520, "label");
        o->box(FL_FLAT_BOX);
        o->color(40);
        o->selection_color(49);
        o->labeltype(FL_NORMAL_LABEL);
        o->labelfont(0);
        o->labelsize(14);
        o->labelcolor(56);
        o->align(FL_ALIGN_CENTER);
        o->when(FL_WHEN_RELEASE);
      }
      o->end();
    }
    { Fl_Button* o = new Fl_Button(5, 545, 100, 30, "Read JPEG");
      o->callback((Fl_Callback*)cb_Read);
    }
    { Fl_Button* o = new Fl_Button(110, 545, 100, 30, "Read PNG");
      o->callback((Fl_Callback*)cb_Read1);
    }
    { Fl_Button* o = new Fl_Button(5, 580, 100, 30, "Write JPEG");
      o->callback((Fl_Callback*)cb_Write);
    }
    { Fl_Button* o = new Fl_Button(110, 580, 100, 30, "Write PNG");
      o->callback((Fl_Callback*)cb_Write1);
    }
    { Fl_Button* o = new Fl_Button(215, 545, 100, 30, "Read TIFF");
      o->callback((Fl_Callback*)cb_Read2);
    }
    { Fl_Button* o = new Fl_Button(215, 580, 100, 30, "Write TIFF");
      o->callback((Fl_Callback*)cb_Write2);
    }
    { Fl_Button* o = new Fl_Button(320, 545, 100, 30, "Read BMP");
      o->callback((Fl_Callback*)cb_Read3);
    }
    { Fl_Button* o = new Fl_Button(320, 580, 100, 30, "Write BMP");
      o->callback((Fl_Callback*)cb_Write3);
    }
    o->end();
  }
}

void image_test_window::show( ) {
  main_window->show( );
}
