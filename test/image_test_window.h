// generated by Fast Light User Interface Designer (fluid) version 1.0104

#ifndef image_test_window_h
#define image_test_window_h
#include <FL/Fl.H>
#include "image_test.h"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Progress.H>

class image_test_window {
public:
  image_test_window();
  Fl_Double_Window *main_window;
  image_draw_area *draw_area;
  static Fl_Menu_Item menu_[];
private:
  inline void cb_Open_i(Fl_Menu_*, void*);
  static void cb_Open(Fl_Menu_*, void*);
  inline void cb_Open1_i(Fl_Menu_*, void*);
  static void cb_Open1(Fl_Menu_*, void*);
  inline void cb_Save_i(Fl_Menu_*, void*);
  static void cb_Save(Fl_Menu_*, void*);
  inline void cb_Save1_i(Fl_Menu_*, void*);
  static void cb_Save1(Fl_Menu_*, void*);
  inline void cb_Exit_i(Fl_Menu_*, void*);
  static void cb_Exit(Fl_Menu_*, void*);
  inline void cb_Euclidean_i(Fl_Menu_*, void*);
  static void cb_Euclidean(Fl_Menu_*, void*);
  inline void cb_Figure_i(Fl_Menu_*, void*);
  static void cb_Figure(Fl_Menu_*, void*);
  inline void cb_Thresholding_i(Fl_Menu_*, void*);
  static void cb_Thresholding(Fl_Menu_*, void*);
  inline void cb_Labeling_i(Fl_Menu_*, void*);
  static void cb_Labeling(Fl_Menu_*, void*);
  inline void cb_Labeling1_i(Fl_Menu_*, void*);
  static void cb_Labeling1(Fl_Menu_*, void*);
  inline void cb_Thinning_i(Fl_Menu_*, void*);
  static void cb_Thinning(Fl_Menu_*, void*);
  inline void cb_Median_i(Fl_Menu_*, void*);
  static void cb_Median(Fl_Menu_*, void*);
  inline void cb_Erosion_i(Fl_Menu_*, void*);
  static void cb_Erosion(Fl_Menu_*, void*);
  inline void cb_Dilation_i(Fl_Menu_*, void*);
  static void cb_Dilation(Fl_Menu_*, void*);
  inline void cb_Opening_i(Fl_Menu_*, void*);
  static void cb_Opening(Fl_Menu_*, void*);
  inline void cb_Closing_i(Fl_Menu_*, void*);
  static void cb_Closing(Fl_Menu_*, void*);
  inline void cb_Erosion1_i(Fl_Menu_*, void*);
  static void cb_Erosion1(Fl_Menu_*, void*);
  inline void cb_Dilation1_i(Fl_Menu_*, void*);
  static void cb_Dilation1(Fl_Menu_*, void*);
  inline void cb_Opening1_i(Fl_Menu_*, void*);
  static void cb_Opening1(Fl_Menu_*, void*);
  inline void cb_Closing1_i(Fl_Menu_*, void*);
  static void cb_Closing1(Fl_Menu_*, void*);
  inline void cb_Reso_i(Fl_Menu_*, void*);
  static void cb_Reso(Fl_Menu_*, void*);
  inline void cb_Reso1_i(Fl_Menu_*, void*);
  static void cb_Reso1(Fl_Menu_*, void*);
  inline void cb_Reso2_i(Fl_Menu_*, void*);
  static void cb_Reso2(Fl_Menu_*, void*);
  inline void cb_Reso3_i(Fl_Menu_*, void*);
  static void cb_Reso3(Fl_Menu_*, void*);
  inline void cb_Reso4_i(Fl_Menu_*, void*);
  static void cb_Reso4(Fl_Menu_*, void*);
  inline void cb_Reso5_i(Fl_Menu_*, void*);
  static void cb_Reso5(Fl_Menu_*, void*);
  inline void cb_Reso6_i(Fl_Menu_*, void*);
  static void cb_Reso6(Fl_Menu_*, void*);
  inline void cb_Interlace_i(Fl_Menu_*, void*);
  static void cb_Interlace(Fl_Menu_*, void*);
  inline void cb_Interlace1_i(Fl_Menu_*, void*);
  static void cb_Interlace1(Fl_Menu_*, void*);
  inline void cb_Expand_i(Fl_Menu_*, void*);
  static void cb_Expand(Fl_Menu_*, void*);
  inline void cb_Shrink_i(Fl_Menu_*, void*);
  static void cb_Shrink(Fl_Menu_*, void*);
  inline void cb_Boundary_i(Fl_Menu_*, void*);
  static void cb_Boundary(Fl_Menu_*, void*);
  inline void cb_Boundary1_i(Fl_Menu_*, void*);
  static void cb_Boundary1(Fl_Menu_*, void*);
public:
  Fl_Progress *progress_bar;
  void show( );
};
#endif
