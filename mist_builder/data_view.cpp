#include "config.h"

#include "data_view.h"


#include <mist/draw.h>


class nofocus_button : public FXButton
{
	// Macro for class hierarchy declarations
	FXDECLARE( nofocus_button )

public:
	typedef FXButton base;

protected:
	nofocus_button( ){}
	nofocus_button( const nofocus_button& ){}

public:
	/// Construct a slider widget
	nofocus_button( FXComposite *p, const FXString &text, FXIcon *ic = NULL, FXObject *tgt = NULL, FXSelector sel = 0, FXuint opts = BUTTON_NORMAL,
					FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
					FXint pl = DEFAULT_PAD, FXint pr = DEFAULT_PAD,
					FXint pt = DEFAULT_PAD, FXint pb = DEFAULT_PAD )
					: base( p, text, ic, tgt, sel, opts, x, y, w, h, pl, pr, pt, pb ){ }

	virtual FXbool canFocus( ) const { return( false ); }
};

class nofocus_toggle_button : public FXToggleButton
{
	// Macro for class hierarchy declarations
	FXDECLARE( nofocus_toggle_button )

public:
	typedef FXToggleButton base;

protected:
	nofocus_toggle_button( ){}
	nofocus_toggle_button( const nofocus_button& ){}

public:
	/// Construct a slider widget
	nofocus_toggle_button( FXComposite *p, const FXString &text1, const FXString &text2, FXIcon *ic1 = NULL, FXIcon *ic2 = NULL,
					FXObject *tgt = NULL, FXSelector sel = 0, FXuint opts = TOGGLEBUTTON_NORMAL,
					FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
					FXint pl = DEFAULT_PAD, FXint pr = DEFAULT_PAD,
					FXint pt = DEFAULT_PAD, FXint pb = DEFAULT_PAD )
					: base( p, text1, text2, ic1, ic2, tgt, sel, opts, x, y, w, h, pl, pr, pt, pb ){ }

	virtual FXbool canFocus( ) const { return( false ); }
};


FXDEFMAP( data_view ) data_view_map[] =
	{
		//________Message_Type_____________________ID_______________Message_Handler_______
		FXMAPFUNC ( SEL_PAINT,				data_view::ID_CANVAS,			data_view::onPaint ),
		FXMAPFUNC ( SEL_LEFTBUTTONPRESS,	data_view::ID_CANVAS,			data_view::onMouseDown ),
		FXMAPFUNC ( SEL_RIGHTBUTTONPRESS,	data_view::ID_CANVAS,			data_view::onMouseDown ),
		FXMAPFUNC ( SEL_LEFTBUTTONRELEASE,	data_view::ID_CANVAS,			data_view::onMouseUp ),
		FXMAPFUNC ( SEL_RIGHTBUTTONRELEASE,	data_view::ID_CANVAS,			data_view::onMouseUp ),
		FXMAPFUNC ( SEL_MOTION,				data_view::ID_CANVAS,			data_view::onMouseMove ),
		FXMAPFUNC ( SEL_COMMAND,			data_view::ID_INTERPOLATE_MODE,	data_view::onInterpolateChange ),
		FXMAPFUNC ( SEL_COMMAND,			data_view::ID_DRAW_CROSS,		data_view::onDrawCrossChange ),
		FXMAPFUNC ( SEL_CHANGED,			data_view::ID_ZOOM_SLIDER,		data_view::onZoomChanged ),
	};


FXIMPLEMENT( data_view, data_view::base, data_view_map, ARRAYNUMBER( data_view_map ) )

FXIMPLEMENT( invert_slider, invert_slider::base, 0, 0 )

FXIMPLEMENT( nofocus_button, nofocus_button::base, 0, 0 )

FXIMPLEMENT( nofocus_toggle_button, nofocus_toggle_button::base, 0, 0 )


/* created by reswrap from file cross.gif */
const unsigned char cross_icon[]={
  0x47,0x49,0x46,0x38,0x39,0x61,0x0f,0x00,0x0f,0x00,0x91,0x00,0x00,0x00,0x12,0xff,
  0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x21,0xf9,0x04,0x01,0x00,0x00,0x02,
  0x00,0x2c,0x00,0x00,0x00,0x00,0x0f,0x00,0x0f,0x00,0x00,0x02,0x1b,0x8c,0x1f,0x80,
  0xbb,0xca,0xef,0x5e,0x93,0x2c,0x52,0x63,0x6f,0xc6,0xa0,0xfb,0xff,0x41,0x17,0xb2,
  0x49,0xa5,0x38,0x62,0xa9,0xba,0x96,0x05,0x00,0x3b
  };


// Construct a data_view
data_view::data_view( FXComposite *p, FXObject *tgt, FXSelector sel, FXuint opts, FXint x, FXint y, FXint w, FXint h, FXint pl, FXint pr, FXint pt, FXint pb, FXint hs, FXint vs )
				: base( p, opts, x, y, w, h, pl, pr, pt, pb, hs, vs ),
				interpolate_( true ),
				zoom_( 1.0 ),
				draw_cross_( false )
{
	icons_.push_back( new FXGIFIcon( getApp( ), cross_icon ) );


	// ���̃E�B���h�E���烁�b�Z�[�W�𑗂��̐ݒ�
	setTarget( tgt );
	setSelector( sel );

	// �N���C�A���g�̈�ɕ��i��z�u����
	FXVerticalFrame *client = new FXVerticalFrame( this, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0 );
	{
		// ���C���̕`�敔���Ɛݒ�_�C�A���O
		{
			glvisual_ = new FXGLVisual( getApp(), VISUAL_DOUBLEBUFFER );
			canvas_ = new FXGLCanvas( client, glvisual_, this, ID_CANVAS, LAYOUT_TOP | LAYOUT_LEFT | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0 );
		}

		// �����̐ݒ�_�C�A���O��z�u����
		{
			FXHorizontalFrame *o = new FXHorizontalFrame( client, LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X | LAYOUT_FIX_HEIGHT, 0, 0, 0, 18, 0, 0, 0, 0, 0, 0 );
			{
				FXSpring *o1 = new FXSpring( o, LAYOUT_FILL_X | LAYOUT_FILL_Y, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
				{
					FXHorizontalFrame *h = new FXHorizontalFrame( o1, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
					( new nofocus_toggle_button( h, "S", "S", 0, 0, this, ID_INTERPOLATE_MODE, TOGGLEBUTTON_KEEPSTATE | FRAME_SUNKEN | LAYOUT_TOP | LAYOUT_FILL_Y ) )->setState( interpolate_ );
					( new nofocus_toggle_button( h, "", "", icons_[ 0 ], icons_[ 0 ], this, ID_DRAW_CROSS, TOGGLEBUTTON_KEEPSTATE | FRAME_SUNKEN | LAYOUT_TOP | LAYOUT_FILL_Y ) )->setState( draw_cross_ );
				}
				FXSpring *o2 = new FXSpring( o, LAYOUT_FILL_X | LAYOUT_FILL_Y, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
				{
					FXHorizontalFrame *h = new FXHorizontalFrame( o2, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
					new FXVerticalFrame( h );
					zoom_slider_ = new invert_slider( h, this, ID_ZOOM_SLIDER, LAYOUT_CENTER_Y | LAYOUT_FIX_HEIGHT | LAYOUT_FILL_X | SLIDER_HORIZONTAL, 0, 0, 0, 10, 0, 0, 0, 0 );
					FXint lo = 0;
					FXint hi = static_cast< FXint >( ( zoom_maximum - zoom_minimum ) / zoom_step + 1.0 );
					zoom_slider_->setRange( lo, hi );
					zoom_slider_->setValue( lo );
				}
			}
		}

	}
}

data_view::~data_view( )
{
	{
		std::vector< FXIcon * >::iterator ite = icons_.begin( );
		for( ; ite != icons_.end( ) ; ite++ )
		{
			delete *ite;
		}
	}
}

void data_view::create( )
{
	base::create( );

	std::vector< FXCursor * > &cursors = getAppInstance( ).cursors;

	canvas_->setDefaultCursor( cursors[ 1 ] );
	canvas_->setDragCursor( cursors[ 2 ] );
}



// Mouse button was pressed somewhere
long data_view::onMouseDown( FXObject *obj, FXSelector sel, void *ptr )
{
	FXEvent &e = *( ( FXEvent * )ptr );

	mark_position_ = screen2point( point2( e.win_x, e.win_y ) );

	return( 1 );
}

// The mouse has moved, draw a line
long data_view::onMouseMove( FXObject *obj, FXSelector sel, void *ptr )
{
	FXEvent &e = *( ( FXEvent * )ptr );

	point2 pt = screen2point( point2( e.win_x, e.win_y ) );

	if( e.state & LEFTBUTTONMASK )
	{
		current_position_ += mark_position_ - pt;

		draw_image( );
	}

	return( 1 );
}

// The mouse button was released again
long data_view::onMouseUp( FXObject *obj, FXSelector sel, void *ptr )
{
	FXEvent &e = *( ( FXEvent * )ptr );

	return( 1 );
}


// Paint the canvas
long data_view::onPaint( FXObject *obj, FXSelector sel, void *ptr )
{
	FXEvent &e = *( ( FXEvent * )ptr );

	draw_image( );

	return( 1 );
}


long data_view::onZoomChanged( FXObject *obj, FXSelector sel, void *ptr )
{
	zoom_ = zoom_slider_->getValue( ) * zoom_step + zoom_minimum;
	draw_image( );

	return( 1 );
}


long data_view::onInterpolateChange( FXObject *obj, FXSelector sel, void *ptr )
{
	interpolate_ = !interpolate_;

	draw_image( );

	return( 1 );
}

long data_view::onDrawCrossChange( FXObject *obj, FXSelector sel, void *ptr )
{
	draw_cross_ = !draw_cross_;

	draw_image( );

	return( 1 );
}

void data_view::draw_image( )
{
	canvas_->makeCurrent( );

	mist::draw_image( buffer_, canvas_->getWidth( ), canvas_->getHeight( ),
						zoom_, current_position_.x, current_position_.y,
						background_color_.r, background_color_.g, background_color_.b,
						interpolate_ );

	if( draw_cross_ )
	{
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity( );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity( );
		glViewport( 0, 0, static_cast< GLint >( canvas_->getWidth( ) ), static_cast< GLint >( canvas_->getHeight( ) ) );

		// ��ʂ̒����Ɂ{���`�悷��
		double len = 8.0;
		double x_unit = 1.0 / static_cast< double >( canvas_->getWidth( ) );
		double y_unit = 1.0 / static_cast< double >( canvas_->getHeight( ) );
		double x_len = len * x_unit;
		double y_len = len * y_unit;
		// �J�[�\���̐F��ݒ�
		//glColor3d( 0.0, 0.0, 1.0 );
		glColor3d( 1.0, 1.0, 0.0 );

		glBegin( GL_LINES );
			glVertex2d( -x_len, 0.0 );
			glVertex2d(  x_len + x_unit / 0.5, 0.0 );
		glEnd( );
		glBegin( GL_LINES );
			glVertex2d( 0.0, -y_len );
			glVertex2d( 0.0,  y_len + y_unit / 0.5 );
		glEnd( );
	}

	canvas_->swapBuffers( );
}

void data_view::draw_image( const data_type &buf )
{
	//// �ݒ�������l�ɖ߂�
	//zoom_ = 1.0;
	//zoom_slider_->setValue( 0 );

	//current_position_ = point2( 0, 0 );

	if( !buf.mono_image_.empty( ) )
	{
		buffer_.resize( buf.mono_image_.width( ), buf.mono_image_.height( ) );
		buffer_.reso( buf.mono_image_.reso1( ), buf.mono_image_.reso2( ) );
		for( size_type i = 0 ; i < buffer_.size( ) ; i++ )
		{
			buffer_[ i ] = buf.mono_image_[ i ] == 0 ? 0 : 255;
		}
	}
	else if( !buf.gray_image_.empty( ) )
	{
		buffer_ = buf.gray_image_;
	}
	else if( !buf.color_image_.empty( ) )
	{
		buffer_ = buf.color_image_;
	}
	else
	{
		buffer_.clear( );
	}

	draw_image( );
}


void data_view::draw_image( const filter *pf )
{
	if( pf == NULL )
	{
		buffer_.fill( );
		draw_image( );
	}
	else
	{
		draw_image( pf->data( ) );
	}
}


// �o�͍��W�̓X�N���[����̓_�ŁC�P�ʂ͉�f
point2 data_view::point2screen( const point2 &pt ) const
{
	// ��ʍ��W����摜���W�ɕϊ�����
	return( mist::point2screen( point2( pt.x, pt.y ), buffer_.width( ), buffer_.height( ),
					buffer_.reso2( ) / buffer_.reso1( ), canvas_->getWidth( ), canvas_->getHeight( ), zoom_, current_position_.x, current_position_.y ) );
}

// ���͍��W�̓X�N���[����̓_�ŁC�P�ʂ͉�f
point2 data_view::screen2point( const point2 &pt ) const
{
	// ��ʍ��W����摜���W�ɕϊ�����
	return( mist::screen2point( pt, buffer_.width( ), buffer_.height( ), buffer_.reso2( ) / buffer_.reso1( ),
										canvas_->getWidth( ), canvas_->getHeight( ), zoom_, current_position_.x, current_position_.y ) );
}

