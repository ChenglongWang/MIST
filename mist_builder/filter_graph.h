#ifndef __INCLUDE_FILTER_GRAPH__
#define __INCLUDE_FILTER_GRAPH__


#include "scroll_area.h"


// Event Handler Object
class filter_graph : public FXPacker
{
	// Macro for class hierarchy declarations
	FXDECLARE( filter_graph )

public:
	typedef FXPacker base;

protected:
	FXCanvas		*canvas_;
	FXFont			*font_;
	FXImage			*mem_image_;

	bool			damage_;
	filter			*current_filter_;
	pin				*current_pin_;
	filter_list		filters_;

	FXint			posX_;
	FXint			posY_;

	// アプリケーションオブジェクトを取得する
	mist_builder *getApp( ) const { return( reinterpret_cast< mist_builder * >( base::getApp( ) ) ); }
	mist_builder &getAppInstance( ) const { return( *getApp( ) ); }

	void initialize_filter( filter &filter );

	bool draw_filter( FXDC &dc, const filter &f );
	void draw_box( FXDC &dc, const FXint x, const FXint y, const FXint w, const FXint h, bool active );
	void draw_arrow( FXDC &dc, const FXint x1, const FXint y1, const FXint x2, const FXint y2, double r );

	bool apply_filters( filter &f );

	void SendUserMessage( UserMessage msg, void *ptr )
	{
		if( getTarget( ) != NULL )
		{
			getTarget( )->handle( this, FXSEL( msg, getSelector( ) ), ptr );
		}
	}

public:
	FXint getContentWidth( ){ return( mem_image_ ? mem_image_->getWidth( ) : 1 ); }
	FXint getContentHeight( ){ return( mem_image_ ? mem_image_->getHeight( ) : 1 ); }

	virtual FXbool canFocus( ) const { return( true ); }
	void set_cursors( bool is_drag );

	FXint getXPosition( ) const { return( posX_ ); }
	FXint getYPosition( ) const { return( posY_ ); }

public:
	void append_filter( const filter &f );
	void recompute_current_filter( );
	filter *get_current_Filter( ){ return( current_filter_ ); }

private:
	filter_graph( ){}
	filter_graph( const filter_graph& ){}
	filter_graph( FXComposite *p, FXuint opts = 0,
				FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0 ){}


public:
	long onPaint( FXObject *obj, FXSelector sel, void *ptr );
	long onMouseDown( FXObject *obj, FXSelector sel, void *ptr );
	long onMouseUp( FXObject *obj, FXSelector sel, void *ptr );
	long onMouseMove( FXObject *obj, FXSelector sel, void *ptr );
	long onKeyDown( FXObject *obj, FXSelector sel, void *ptr );
	long onKeyUp( FXObject *obj, FXSelector sel, void *ptr );

	long onDataChanged( FXObject *obj, FXSelector sel, void *ptr );

	virtual void create( );

public:
	enum
	{
		ID_CANVAS = base::ID_LAST,
		ID_LAST
	};

public:
	filter_graph( FXComposite *p, FXObject *tgt = NULL, FXSelector sel = 0, FXuint opts = 0,
				FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0 );

	virtual ~filter_graph( );
};



#endif	// __INCLUDE_FILTER_GRAPH__
