#include <iostream>

#include <cmath>
#include <mist/mist.h>
#include <mist/limits.h>

#include <mist/filter/morphology.h>
#include <mist/timer.h>

#include <mist/io/jpeg.h>
#include <mist/io/bmp.h>
#include <mist/io/png.h>
#include <mist/io/tiff.h>
#include <mist/io/pnm.h>

// ��������C���[�e�B���e�B�֐��Q
inline std::string to_lower_case( const std::string &str )
{
	std::string s = "";
	for( std::string::size_type i = 0 ; i < str.size( ) ; i++ )
	{
		s += static_cast< char >( tolower( str[ i ] ) );
	}
	return( s );
}

inline std::string get_ext( const std::string &str )
{
	std::string::size_type index = str.find_last_of( "." );
	if( index == str.npos )
	{
		return( "" );
	}
	return( str.substr( index ) );
}

template < class T, class Allocator >
inline bool read_image( mist::array2< T, Allocator > &image, const std::string &filename )
{
	typedef typename mist::array2< T, Allocator >::size_type size_type;

	std::string ext = to_lower_case( get_ext( filename ) );
	if( ext == "" )
	{
		return( false );
	}

	bool ret = false;
	if( ext == ".jpeg" || ext == ".jpg" )
	{
		ret = mist::read_jpeg( image, filename );
	}
	else if( ext == ".bmp" )
	{
		ret = mist::read_bmp( image, filename );
	}
	else if( ext == ".tiff" || ext == ".tif" )
	{
		ret = mist::read_tiff( image, filename );
	}
	else if( ext == ".png" )
	{
		ret = mist::read_png( image, filename );
	}
	else if( ext == ".pbm" || ext == ".pgm" || ext == ".ppm" || ext == ".pnm" )
	{
		ret = mist::read_pnm( image, filename );
	}
	else
	{
		ret = false;
	}
	return( ret );
}


namespace orthodox
{
	template < class Array1, class Array2 >
	void erosion( const Array1 &in, Array2 &out, double radius )
	{
		typedef typename Array1::size_type  size_type;
		typedef typename Array1::value_type value_type;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array2::value_type out_value_type;

		difference_type w = in.width( );
		difference_type h = in.height( );
		difference_type d = in.depth( );

		double resoX = in.reso1( );
		double resoY = in.reso2( );
		double resoZ = in.reso3( );

		double min_reso = resoX < resoY ? resoX: resoY;
		min_reso = min_reso < resoZ ? min_reso : resoZ;
		double ax = resoX / min_reso;
		double ay = resoY / min_reso;
		double az = resoZ / min_reso;
		double xx, yy, zz, rr = radius * radius;

		difference_type rx = static_cast< size_type >( ceil( radius / ax ) );
		difference_type ry = static_cast< size_type >( ceil( radius / ay ) );
		difference_type rz = static_cast< size_type >( ceil( radius / az ) );
		difference_type i, j, k, x, y, z;

		value_type min;

		for( k = 0 ; k < d ; k++ )
		{
			for( j = 0 ; j < h ; j++ )
			{
				for( i = 0; i < w ; i++ )
				{
					min = mist::type_limits< value_type >::maximum( );
					for( z = -rz ; z <= rz ; z++ )
					{
						if( k + z < 0 || k + z >= d )
						{
							continue;
						}
						zz = z * z * az * az;
						for( y = -ry ; y <= ry ; y++ )
						{
							if( j + y < 0 || j + y >= h )
							{
								continue;
							}
							yy = y * y * ay * ay;
							for( x = -rx ; x <= rx ; x++ )
							{
								if( i + x < 0 || i + x >= w )
								{
									continue;
								}
								xx = x * x * ax * ax;
								if( xx + yy + zz <= rr )
								{
									if( in( i + x, j + y, k + z ) < min )
									{
										min = in( i + x, j + y, k + z );
									}
								}
							}
						}
					}

					out( i, j, k ) = static_cast< out_value_type >( min );
				}
			}
		}
	}

	template < class Array1, class Array2 >
	void dilation( const Array1 &in, Array2 &out, double radius )
	{
		typedef typename Array1::size_type  size_type;
		typedef typename Array1::value_type value_type;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array2::value_type out_value_type;

		difference_type w = in.width( );
		difference_type h = in.height( );
		difference_type d = in.depth( );

		double resoX = in.reso1( );
		double resoY = in.reso2( );
		double resoZ = in.reso3( );

		double min_reso = resoX < resoY ? resoX: resoY;
		min_reso = min_reso < resoZ ? min_reso : resoZ;
		double ax = resoX / min_reso;
		double ay = resoY / min_reso;
		double az = resoZ / min_reso;
		double xx, yy, zz, rr = radius * radius;

		difference_type rx = static_cast< size_type >( ceil( radius / ax ) );
		difference_type ry = static_cast< size_type >( ceil( radius / ay ) );
		difference_type rz = static_cast< size_type >( ceil( radius / az ) );
		difference_type i, j, k, x, y, z;

		value_type max;

		for( k = 0 ; k < d ; k++ )
		{
			for( j = 0 ; j < h ; j++ )
			{
				for( i = 0; i < w ; i++ )
				{
					max = mist::type_limits< value_type >::minimum( );
					for( z = -rz ; z <= rz ; z++ )
					{
						if( k + z < 0 || k + z >= d )
						{
							continue;
						}
						zz = z * z * az * az;
						for( y = -ry ; y <= ry ; y++ )
						{
							if( j + y < 0 || j + y >= h )
							{
								continue;
							}
							yy = y * y * ay * ay;
							for( x = -rx ; x <= rx ; x++ )
							{
								if( i + x < 0 || i + x >= w )
								{
									continue;
								}
								xx = x * x * ax * ax;
								if( xx + yy + zz <= rr )
								{
									if( in( i + x, j + y, k + z ) > max )
									{
										max = in( i + x, j + y, k + z );
									}
								}
							}
						}
					}

					out( i, j, k ) = static_cast< out_value_type >( max );
				}
			}
		}
	}

	template < class Array1, class Array2 >
	void opening( const Array1 &in, Array2 &out, double radius )
	{
		Array1 tmp( in );
		erosion( in, tmp, radius );
		dilation( tmp, out, radius );
	}

	template < class Array1, class Array2 >
	void closing( const Array1 &in, Array2 &out, double radius )
	{
		Array1 tmp( in );
		dilation( in, tmp, radius );
		erosion( tmp, out, radius );
	}
}



int main( int argc, char *argv[] )
{
	using namespace std;

	if( argc != 3 )
	{
		return( 1 );
	}

	mist::array2< unsigned char > a;

	read_image( a, argv[ 1 ] );

	mist::array2< unsigned char >::size_type i, loop = 10;

	mist::array2< unsigned char > tmp1( a ), tmp2( a );

	double radius = atof( argv[ 2 ] );

	{
		cout << "Orthodox Opening Filter" << endl;
		mist::timer t;
		for( i = 0 ; i < loop ; i++ )
		{
			orthodox::opening( a, tmp1, radius );
		}
		cout << "Calculation Time: " << t.elapse( ) / static_cast< double >( loop ) << " (sec)" << endl;
	}

	{
		cout << "MIST Opening Filter" << endl;
		mist::timer t;
		for( i = 0 ; i < loop ; i++ )
		{
			tmp2 = a;
			mist::opening( tmp2, radius );
		}
		cout << "Calculation Time: " << t.elapse( ) / static_cast< double >( loop ) << " (sec)" << endl;
	}
	
	{
		double err = 0.0;
		for( i = 0 ; i < tmp1.size( ) ; i++ )
		{
			err += fabs( static_cast< double >( tmp1[ i ] - tmp2[ i ] ) );
		}
		cout << "Difference: " << err << endl;
	}

	return( 0 );
}