#include "config.h"

#include "filter_graph.h"


FXDEFMAP( filter_graph ) filter_graph_map[] =
	{
		//________Message_Type_____________________ID_________________________________Message_Handler_______
		FXMAPFUNC ( SEL_PAINT,					filter_graph::ID_CANVAS,			filter_graph::onPaint ),
		FXMAPFUNC ( SEL_LEFTBUTTONPRESS,		filter_graph::ID_CANVAS,			filter_graph::onMouseDown ),
		FXMAPFUNC ( SEL_RIGHTBUTTONPRESS,		filter_graph::ID_CANVAS,			filter_graph::onMouseDown ),
		FXMAPFUNC ( SEL_LEFTBUTTONRELEASE,		filter_graph::ID_CANVAS,			filter_graph::onMouseUp ),
		FXMAPFUNC ( SEL_RIGHTBUTTONRELEASE,		filter_graph::ID_CANVAS,			filter_graph::onMouseUp ),
		FXMAPFUNC ( SEL_MOTION,					filter_graph::ID_CANVAS,			filter_graph::onMouseMove ),
		FXMAPFUNC ( SEL_KEYPRESS,				filter_graph::ID_CANVAS,			filter_graph::onKeyDown ),
		FXMAPFUNC ( SEL_KEYRELEASE,				filter_graph::ID_CANVAS,			filter_graph::onKeyUp ),
	};


FXIMPLEMENT( filter_graph, filter_graph::base, filter_graph_map, ARRAYNUMBER( filter_graph_map ) )


// Construct a filter_graph
filter_graph::filter_graph( FXComposite *p, FXObject *tgt, FXSelector sel, FXuint opts, FXint x, FXint y, FXint w, FXint h )
				: base( p, opts | SCROLLERS_NORMAL | SCROLLERS_TRACK, x, y, w, h, 0, 0, 0, 0, 0, 0 ),
				font_( NULL ), mem_image_( NULL ), damage_( true ), current_filter_( NULL ), current_pin_( NULL ),
				posX_( 0 ), posY_( 0 )
{
	// このウィンドウからメッセージを送る先の設定
	setTarget( tgt );
	setSelector( sel );

	canvas_ = new FXCanvas( this, this, ID_CANVAS, LAYOUT_FILL_X | LAYOUT_FILL_Y );

	font_ = new FXFont( getApp( ), "helvetica", 12 );
	mem_image_ = new FXImage( getApp( ), NULL, IMAGE_OWNED, 1024, 768 );
}

filter_graph::~filter_graph( )
{
	for( size_type i = 0 ; i < filters_.size( ) ; i++ )
	{
		delete filters_[ i ];
	}
}

void filter_graph::create( )
{
	font_->create( );

	mem_image_->create( );

	base::create( );
}

void filter_graph::set_cursors( bool is_drag )
{
	std::vector< FXCursor * > &cursors = getAppInstance( ).cursors;
	if( is_drag )
	{
		canvas_->setDefaultCursor( cursors[ 1 ] );
		canvas_->setDragCursor( cursors[ 2 ] );
	}
	else
	{
		canvas_->setDefaultCursor( cursors[ 0 ] );
		canvas_->setDragCursor( cursors[ 0 ] );
	}
}

void filter_graph::append_filter( const filter &f )
{
	filter *ff = f.clone( );
	initialize_filter( *ff );
	filters_.push_back( ff );

	if( current_filter_ != NULL )
	{
		current_filter_->active = false;
	}

	current_filter_ = ff;
	current_filter_->active = true;

	// メインウィンドウへ，選択フィルタが変更されたことを通知する
	SendUserMessage( MIST_FILTER_CHANGED, static_cast< void * >( current_filter_ ) );

	// 全体を再描画する
	damage_ = true;
	canvas_->update( );
	canvas_->repaint( );
}


// Mouse button was pressed somewhere
long filter_graph::onMouseDown( FXObject *obj, FXSelector sel, void *ptr )
{
	FXEvent &e = *( ( FXEvent * )ptr );

	set_cursors( e.state & ALTMASK && e.state & LEFTBUTTONMASK );

	//canvas_->setFocus( );

	filter *old_filter = current_filter_;

	current_filter_ = NULL;
	current_pin_ = NULL;

	FXint x = e.win_x - getXPosition( );
	FXint y = e.win_y - getYPosition( );

	// まずフィルタがクリックされていないかを検索する
	size_type i, index = filters_.size( );
	for( i = 0 ; i < filters_.size( ) ; i++ )
	{
		filter &f = *( filters_[ i ] );
		if( f.hittest( x, y ) )
		{
			f.active = true;
			current_filter_ = &f;
			index = i;
			i++;
			break;
		}
		else
		{
			f.active = false;
		}
	}
	for( ; i < filters_.size( ) ; i++ )
	{
		filter &f = *( filters_[ i ] );
		f.active = false;
	}

	// リストの中で，描画される順番を変更し，選択フィルタが最前面に来るようにする
	if( current_filter_ != NULL && index != filters_.size( ) )
	{
		filters_.erase( filters_.begin( ) + index );
		filters_.push_back( current_filter_ );

	}

	if( old_filter != current_filter_ )
	{
		// メインウィンドウへ，選択フィルタが変更されたことを通知する
		SendUserMessage( MIST_FILTER_CHANGED, static_cast< void * >( current_filter_ ) );
	}

	// ピンがクリックされたかどうかを判定する
	if( current_filter_ == NULL )
	{
		for( i = 0 ; i < filters_.size( ) ; i++ )
		{
			filter &f = *( filters_[ i ] );
			pin_list &output_pins = f.output_pins( );
			for( size_type j = 0 ; j < output_pins.size( ) ; j++ )
			{
				pin &p = output_pins[ j ];
				if( p.hittest( x, y ) )
				{
					current_filter_ = &f;
					current_pin_    = &p;
					f.active = true;
					break;
				}
			}
			if( current_pin_ != NULL )
			{
				break;
			}
		}

		// すでに接続されているピンがある場合は，接続を解除する
		if( current_pin_ != NULL && current_pin_->connected_pin( ) != NULL )
		{
			pin *p = current_pin_->connected_pin( );
			p->connected_pin( NULL );
			current_pin_->connected_pin( NULL );
		}
	}

	damage_ = true;
	canvas_->update( );
	canvas_->repaint( );

	return( 1 );
}

// The mouse has moved, draw a line
long filter_graph::onMouseMove( FXObject *obj, FXSelector sel, void *ptr )
{
	FXEvent &e = *( ( FXEvent * )ptr );

	set_cursors( e.state & ALTMASK && e.state & LEFTBUTTONMASK );

	if( e.state & ALTMASK && e.state & LEFTBUTTONMASK )
	{
		set_cursors( true );
		posX_ += e.win_x - e.last_x;
		posY_ += e.win_y - e.last_y;
		canvas_->update( );
		canvas_->repaint( );
	}
	else if( current_filter_ != NULL && current_pin_ == NULL && e.state & LEFTBUTTONMASK )
	{
		current_filter_->move( e.win_x - e.last_x, e.win_y - e.last_y );

		damage_ = true;
		canvas_->update( );
		canvas_->repaint( );
	}
	else if( current_pin_ != NULL )
	{
		canvas_->update( );
		canvas_->repaint( );
	}

	return( 1 );
}

// The mouse button was released again
long filter_graph::onMouseUp( FXObject *obj, FXSelector sel, void *ptr )
{
	FXEvent &e = *( ( FXEvent * )ptr );

	set_cursors( e.state & ALTMASK && e.state & LEFTBUTTONMASK );

	if( current_filter_ != NULL && current_pin_ != NULL )
	{
		filter	*target_filter_ = NULL;
		pin		*target_pin_ = NULL;

		FXint x = e.win_x - getXPosition( );
		FXint y = e.win_y - getYPosition( );

		// 接続先のインプットピンを探す
		for( size_type i = 0 ; i < filters_.size( ) ; i++ )
		{
			filter &f = *( filters_[ i ] );
			pin_list &input_pins = f.input_pins( );

			// 同じフィルタの入力と出力を接続することはできない
			if( &f == current_filter_ )
			{
				continue;
			}

			for( size_type j = 0 ; j < input_pins.size( ) ; j++ )
			{
				pin &p = input_pins[ j ];
				if( p.hittest( x, y ) )
				{
					target_filter_ = &f;
					target_pin_    = &p;
					break;
				}
			}
			if( target_pin_ != NULL )
			{
				break;
			}
		}

		if( target_pin_ == NULL || target_pin_->pin_type( ) != current_pin_->pin_type( ) )
		{
			current_filter_ = NULL;
			current_pin_    = NULL;
		}
		else
		{
			// ピンを接続することで，グラフ上にループができないかを確認する
			filter_set fset;
			filter_list flist;
			flist.push_back( target_filter_ );

			// ターゲットピンからいくことができるフィルタを全て列挙する
			while( !flist.empty( ) )
			{
				filter &f = *( flist.front( ) );
				pin_list &output_pins = f.output_pins( );
				flist.pop_front( );

				for( size_type i = 0 ; i < output_pins.size( ) ; i++ )
				{
					pin &p = output_pins[ i ];
					if( p.connected_pin( ) != NULL )
					{
						filter *pf = p.connected_pin( )->related_filter( );
						std::pair< filter_set::iterator, bool > ret = fset.insert( pf );
						if( ret.second )
						{
							flist.push_back( pf );
						}
					}
				}
			}

			// ターゲットピンから現在のフィルタへたどり着けるかをチェックする
			if( fset.find( current_filter_ ) != fset.end( ) )
			{
				// フィルタグラフ内にループが存在する
				FXMessageBox::warning( this, MBOX_OK, "Warning", "Filter Graph contains self loop." );
				current_pin_    = NULL;
			}
			else
			{
				// 接続先のピンは，すでに他のピンと接続されている場合は，接続を解除する
				if( target_pin_->connected_pin( ) != NULL )
				{
					pin *p = target_pin_->connected_pin( );
					p->connected_pin( NULL );
					target_pin_->connected_pin( NULL );
				}

				current_pin_->connected_pin( target_pin_ );
				target_pin_->connected_pin( current_pin_ );
				current_pin_    = NULL;
				damage_ = true;

				// フィルタを連結したので，データの再計算を行う
				apply_filters( *current_filter_ );
			}
		}
		canvas_->update( );
		canvas_->repaint( );
	}

	return( 1 );
}

// The mouse button was released again
long filter_graph::onKeyDown( FXObject *obj, FXSelector sel, void *ptr )
{
	FXEvent &e = *( ( FXEvent * )ptr );

	set_cursors( e.state & ALTMASK );

	if( e.code == KEY_Delete && current_filter_ != NULL )
	{
		pin_list &input_pins = current_filter_->input_pins( );
		pin_list &output_pins = current_filter_->output_pins( );

		// 入力ピン側の接続を解除する
		for( size_type i = 0 ; i < input_pins.size( ) ; i++ )
		{
			pin &p = input_pins[ i ];
			if( p.connected_pin( ) != NULL )
			{
				pin *tpin = p.connected_pin( );
				p.connected_pin( NULL );
				tpin->connected_pin( NULL );
			}
		}

		// 出力ピン側の接続を解除する
		for( size_type i = 0 ; i < output_pins.size( ) ; i++ )
		{
			pin &p = output_pins[ i ];
			if( p.connected_pin( ) != NULL )
			{
				pin *tpin = p.connected_pin( );
				p.connected_pin( NULL );
				tpin->connected_pin( NULL );
			}
		}

		// 選択されているフィルタを，リストから除外する
		filter_list::iterator ite = filters_.begin( );
		for( ; ite != filters_.end( ) ; ++ite )
		{
			if( *ite == current_filter_ )
			{
				break;
			}
		}
		if( ite != filters_.end( ) )
		{
			delete current_filter_;
			filters_.erase( ite );
		}

		// フィルタの接続が解除されたので，フィルタ全体の再計算を行う

		// メインウィンドウへ，選択フィルタが解除されたことを通知する
		SendUserMessage( MIST_FILTER_CHANGED, NULL );

		// 変更されたフィルタを再描画する
		damage_ = true;
		canvas_->update( );
		canvas_->repaint( );
	}

	return( 1 );
}

// The mouse button was released again
long filter_graph::onKeyUp( FXObject *obj, FXSelector sel, void *ptr )
{
	FXEvent &e = *( ( FXEvent * )ptr );

	set_cursors( e.state & ALTMASK );

	return( 1 );
}


// Paint the canvas
long filter_graph::onPaint( FXObject *obj, FXSelector sel, void *ptr )
{
	FXEvent &e = *( ( FXEvent * )ptr );

	if( damage_ )
	{
		FXDCWindow memdc( mem_image_ );

		memdc.setFont( font_ );

		memdc.setForeground( RGB( 255, 255, 255 ) );
		memdc.fillRectangle( 0, 0, mem_image_->getWidth( ), mem_image_->getHeight( ) );

		for( size_type i = 0 ; i < filters_.size( ) ; i++ )
		{
			filter &f = *( filters_[ i ] );
			pin_list &output_pins = f.output_pins( );
			draw_filter( memdc, f );

			// 接続されているピンを描画する
			for( size_type j = 0 ; j < output_pins.size( ) ; j++ )
			{
				pin &p1 = output_pins[ j ];
				if( p1.connected_pin( ) != NULL )
				{
					pin &p2 = *( p1.connected_pin( ) );
					FXint sx = p1.x + p1.width / 2;
					FXint sy = p1.y + p1.height / 2;
					FXint dx = p2.x + p2.width / 2;
					FXint dy = p2.y + p2.height / 2;

					// 接続されているピンの種類によって色分けする
					if( !f.active )
					{
						switch( p1.pin_type( ) )
						{
						case pin::MONO:
							memdc.setForeground( FXRGB( 50, 50, 128 ) );
							break;

						case pin::GRAY:
							memdc.setForeground( FXRGB( 50, 128, 50 ) );
							break;

						case pin::COLOR:
							memdc.setForeground( FXRGB( 128, 50, 50 ) );
							break;

						default:
							memdc.setForeground( FXRGB( 50, 50, 50 ) );
							break;
						}
					}
					else
					{
						switch( p1.pin_type( ) )
						{
						case pin::MONO:
							memdc.setForeground( FXRGB( 130, 130, 255 ) );
							break;

						case pin::GRAY:
							memdc.setForeground( FXRGB( 130, 255, 130 ) );
							break;

						case pin::COLOR:
							memdc.setForeground( FXRGB( 255, 130, 130 ) );
							break;

						default:
							memdc.setForeground( FXRGB( 128, 128, 128 ) );
							break;
						}
					}
					draw_arrow( memdc, sx, sy, dx, dy, 10 );
				}
			}
		}

		damage_ = false;
	}

	FXDCWindow dc( canvas_, &e );
	dc.drawImage( mem_image_, getXPosition( ), getYPosition( ) );

	FXint w = getWidth( );
	FXint h = getHeight( );
	FXint l = getXPosition( );
	FXint r = getXPosition( ) + mem_image_->getWidth( );
	FXint t = getYPosition( );
	FXint b = getYPosition( ) + mem_image_->getHeight( );

	// キャンバスの範囲外にある領域を塗りつぶす
	dc.setForeground( FXRGB( 128, 128, 128 ) );
	// 左端
	dc.fillRectangle( 0, 0, l, h );
	// 右端
	dc.fillRectangle( r, 0, w - r, h );
	// 上端
	dc.fillRectangle( l, 0, r - l, t );
	// 下端
	dc.fillRectangle( l, b, r - l, h - b );

	// 接続先のピンを探している時は，マウスの位置までの矢印を描画する
	if( current_pin_ != NULL )
	{
		FXint sx = current_pin_->x + current_pin_->width / 2 + getXPosition( );
		FXint sy = current_pin_->y + current_pin_->height / 2 + getYPosition( );
		FXint dx = e.win_x;
		FXint dy = e.win_y;

		dc.setForeground( FXRGB( 50, 50, 50 ) );
		draw_arrow( dc, sx, sy, dx, dy, 10 );
	}

	return( 1 );
}

void filter_graph::initialize_filter( filter &f )
{
	FXString text( f.name( ).c_str( ) );
	FXint fw = font_->getTextWidth( text );
	FXint fh = font_->getTextHeight( text );

	pin_list &input_pins = f.input_pins( );
	pin_list &output_pins = f.output_pins( );
	FXint ipins = static_cast< FXint >( input_pins.size( ) );
	FXint opins = static_cast< FXint >( output_pins.size( ) );

	FXint margin = layout_parameter::margin;
	FXint box_size = layout_parameter::box_size;

	// フォントの高さを決定し，描画するボックスの最低の高さを決定する
	FXint w = fw + margin * 2;
	FXint h = fh + margin * 2;

	FXint pin_num = ipins > opins ? ipins : opins;
	FXint hh = ( pin_num * 2 + 1 ) * box_size;

	f.x      = canvas_->getWidth( ) / 2 - getXPosition( );
	f.y      = canvas_->getHeight( ) / 2 - getYPosition( );
	f.width  = w;
	f.height = h > hh ? h : hh;

	FXint i = 0;
	FXint ioffset = ( f.height - ( ipins * 2 + 1 ) * box_size ) / 2;
	FXint ooffset = ( f.height - ( opins * 2 + 1 ) * box_size ) / 2;
	for( i = 0 ; i < ipins ; i++ )
	{
		// フィルターの描画位置を指定する
		input_pins[ i ].x      = f.x - box_size;
		input_pins[ i ].y      = f.y + ( 2 * i + 1 ) * box_size + ioffset;
		input_pins[ i ].width  = box_size;
		input_pins[ i ].height = box_size;

		// フィルターとピンの関係を設定する
		input_pins[ i ].related_filter( &f );
	}

	for( i = 0 ; i < opins ; i++ )
	{
		// フィルターの描画位置を指定する
		output_pins[ i ].x      = f.x + f.width;
		output_pins[ i ].y      = f.y + ( 2 * i + 1 ) * box_size + ooffset;
		output_pins[ i ].width  = box_size;
		output_pins[ i ].height = box_size;

		// フィルターとピンの関係を設定する
		output_pins[ i ].related_filter( &f );
	}
}

void filter_graph::draw_arrow( FXDC &dc, const FXint x1, const FXint y1, const FXint x2, const FXint y2, double r )
{
	if( x1 == x2 && y1 == y2 )
	{
		return;
	}

	r = r < 1 ? 1 : r;

	dc.drawLine( x1, y1, x2, y2 );
	point2 sp( x1, y1 ), dp( x2, y2 );
	point2 v1 = ( dp - sp ).unit( );
	point2 v2( -v1.y, v1.x );
	double rr = r * 0.57735026918962576450914878050196;
	point2 p1 = dp - v1 * r + v2 * rr;
	point2 p2 = dp - v1 * r - v2 * rr;

	FXPoint p[ 3 ];
	p[ 0 ] = FXPoint( x2, y2 );
	p[ 1 ] = FXPoint( static_cast< FXint >( p1.x + 0.5 ), static_cast< FXint >( p1.y + 0.5 ) );
	p[ 2 ] = FXPoint( static_cast< FXint >( p2.x + 0.5 ), static_cast< FXint >( p2.y + 0.5 ) );

	dc.fillPolygon( p, 3 );
}

void filter_graph::draw_box( FXDC &dc, const FXint x, const FXint y, const FXint w, const FXint h, bool active )
{
	FXPoint p[ 9 ];

	p[ 0 ] = FXPoint( x     + 2, y         );
	p[ 1 ] = FXPoint( x + w - 2, y         );
	p[ 2 ] = FXPoint( x + w    , y     + 2 );
	p[ 3 ] = FXPoint( x + w    , y + h - 2 );
	p[ 4 ] = FXPoint( x + w - 2, y + h     );
	p[ 5 ] = FXPoint( x     + 2, y + h     );
	p[ 6 ] = FXPoint( x        , y + h - 2 );
	p[ 7 ] = FXPoint( x        , y     + 2 );
	p[ 8 ] = p[ 0 ];

	dc.setForeground( active ? FXRGB( 253, 131, 102 ) : FXRGB( 255, 251, 233 ) );
	dc.fillPolygon( p, 8 );

	dc.setForeground( FXRGB( 0, 0, 0 ) );
	dc.drawLines( p, 9 );
}

// Paint the canvas
bool filter_graph::draw_filter( FXDC &dc, const filter &f )
{
	const layout_parameter &p = f;
	const pin_list &input_pins = f.input_pins( );
	const pin_list &output_pins = f.output_pins( );
	FXint ipins = static_cast< FXint >( input_pins.size( ) );
	FXint opins = static_cast< FXint >( output_pins.size( ) );

	FXint margin = layout_parameter::margin;

	FXint x = p.x;
	FXint y = p.y;

	// フォントの高さを決定し，描画するボックスの最低の高さを決定する
	FXString text( f.name( ).c_str( ) );
	FXint fh = font_->getTextHeight( text );

	FXint w = p.width;
	FXint h = p.height;

	// フィルタのボックス描画する
	draw_box( dc, x, y, w, h, p.active );
	dc.drawText( x + margin, y + fh + ( h - fh - 2 * margin ) / 2, text.text( ), text.length( ) );

	FXint i = 0;
	for( i = 0 ; i < ipins ; i++ )
	{
		const pin &pn = input_pins[ i ];
		switch( pn.pin_type( ) )
		{
		case pin::MONO:
			dc.setForeground( FXRGB( 0, 0, 255 ) );
			break;

		case pin::GRAY:
			dc.setForeground( FXRGB( 0, 255, 0 ) );
			break;

		case pin::COLOR:
			dc.setForeground( FXRGB( 255, 0, 0 ) );
			break;

		default:
			dc.setForeground( FXRGB( 128, 128, 128 ) );
			break;
		}

		dc.fillRectangle( pn.x, pn.y, pn.width, pn.height );
	}

	for( i = 0 ; i < opins ; i++ )
	{
		const pin &pn = output_pins[ i ];
		switch( pn.pin_type( ) )
		{
		case pin::MONO:
			dc.setForeground( FXRGB( 0, 0, 255 ) );
			break;

		case pin::GRAY:
			dc.setForeground( FXRGB( 0, 255, 0 ) );
			break;

		case pin::COLOR:
			dc.setForeground( FXRGB( 255, 0, 0 ) );
			break;

		default:
			dc.setForeground( FXRGB( 128, 128, 128 ) );
			break;
		}

		dc.fillRectangle( pn.x, pn.y, pn.width, pn.height );
	}

	return( true );
}

bool filter_graph::apply_filters( filter &f )
{
	// あるフィルタの出力を他のフィルタの入力につないだ際の，データの伝播を実現する
	filter_list cur, next;
	size_type i;

	cur.push_back( &f );

	do
	{
		while( !cur.empty( ) )
		{
			filter &f = *( cur.front( ) );
			pin_list &opins = f.output_pins( );
			cur.pop_front( );

			for( i = 0 ; i < opins.size( ) ; i++ )
			{
				pin *p = opins[ i ].connected_pin( );
				if( p != NULL )
				{
					filter *pf = p->related_filter( );
					if( pf->is_input_pins_connected( ) )
					{
						pf->filtering( );
						// 次に伝播するように設定する
						next.push_back( pf );
					}
					else
					{
						// 接続されているフィルタは，フィルタリングの準備ができていないので，データをクリアする
						//pf->data( ).clear( );
					}
				}
			}
		}
		cur = next;
		next.clear( );
	}
	while( !cur.empty( ) );

	return( true );
}

void filter_graph::recompute_current_filter( )
{
	if( current_filter_ == NULL )
	{
		return;
	}

	if( current_filter_->filtering( ) )
	{
		apply_filters( *current_filter_ );
	}

	// メインウィンドウへ，選択フィルタが変更されたことを通知する
	SendUserMessage( MIST_FILTER_IMAGE_CHANGED, static_cast< void * >( current_filter_ ) );
}






