#ifndef __INCLUDE_MIST_QUATERNION__
#define __INCLUDE_MIST_QUATERNION__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif


#include <cmath>

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


template < class T >
class quaternion
{
public:
	typedef T value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

public:
	value_type w;
	value_type x;
	value_type y;
	value_type z;

	quaternion( ) : w( 0 ), x( 0 ), y( 0 ), z( 0 ){ }
	quaternion( const value_type &ww, const value_type &xx, const value_type &yy, const value_type &zz ) : w( ww ), x( xx ), y( yy ), z( zz ){ }

	template < class TT >
	quaternion( const quaternion< TT > &q ) : w( static_cast< value_type >( q.w ) ), x( static_cast< value_type >( q.x ) ), y( static_cast< value_type >( q.y ) ), z( static_cast< value_type >( q.z ) ){ }

	quaternion( const quaternion< T > &q ) : w( q.w ), x( q.x ), y( q.y ), z( q.z ){ }

	template < class TT >
	quaternion( value_type ww, const vector3< TT > &v ) : w( ww ), x( static_cast< value_type >( v.x ) ), y( static_cast< value_type >( v.y ) ), z( static_cast< value_type >( v.z ) ){ }

	// �N�H�[�^�j�I����p�����C�ӎ�����̉�]
	// �E��n�̏ꍇ�͉E�˂���]�C����n�̏ꍇ�͍��˂���]�ƂȂ�̂Œ��ӁI�I
	template < class TT >
	quaternion( const vector3< TT > &axis, value_type theta )
	{
		double t = theta * 3.1415926535897932384626433832795 / 180.0 / 2.0;
		double c = std::cos( t );
		double s = std::sin( t );
		w = static_cast< value_type >( c );
		x = static_cast< value_type >( s * axis.x );
		y = static_cast< value_type >( s * axis.y );
		z = static_cast< value_type >( s * axis.z );
	}

	// �I�y���[�^�̃I�[�o�[���C�h
	const quaternion &operator =( const quaternion &q )
	{
		if( &q != this )
		{
			w = q.w;
			x = q.x;
			y = q.y;
			z = q.z;
		}
		return ( *this );
	}

	const quaternion &operator +=( const quaternion &q )
	{
		w = w + q.w;
		x = x + q.x;
		y = y + q.y;
		z = z + q.z;
		return( *this );
	}

	const quaternion &operator +=( const value_type &a )
	{
		w += a;
		return( *this );
	}

	const quaternion &operator -=( const quaternion &q )
	{
		w = w - q.w;
		x = x - q.x;
		y = y - q.y;
		z = z - q.z;
		return( *this );
	}

	const quaternion &operator -=( const value_type &a )
	{
		w -= a;
		return( *this );
	}

	const quaternion &operator *=( const quaternion &q )
	{
		value_type ww = w * q.w - x * q.x - y * q.y - z * q.z;
		value_type xx = w * q.x + x * q.w + y * q.z - z * q.y;
		value_type yy = w * q.y + y * q.w + z * q.x - x * q.z;
		value_type zz = w * q.z + z * q.w + x * q.y - y * q.x;
		w = ww;
		x = xx;
		y = yy;
		z = zz;
		return( *this );
	}

	const quaternion &operator *=( const value_type &a )
	{
		w *= a;
		x *= a;
		y *= a;
		z *= a;
		return( *this );
	}

	const quaternion &operator /=( const quaternion &q )
	{
		return( this->operator *=( q.inv( ) ) );
	}

	const quaternion &operator /=( const value_type &a )
	{
		w /= a;
		x /= a;
		y /= a;
		z /= a;
		return( *this );
	}


	// ��r�֐�
	bool operator ==( const quaternion &q ) const { return( w == q.w && x == q.x && y == q.y && z == q.z ); }
	bool operator !=( const quaternion &q ) const { return( !( *this == q ) ); }
	bool operator < ( const quaternion &q ) const { return( !( *this >= q ) ); }
	bool operator <=( const quaternion &q ) const { return( q >= *this ); }
	bool operator > ( const quaternion &q ) const { return( q < *this ); }
	bool operator >=( const quaternion &q ) const { return( w >= q.w && x >= q.x && y >= q.y && z >= q.z ); }


public:	// ���̑��̊֐�
	const quaternion conjugate( ) const	// �����N�H�[�^�j�I��
	{
		return( quaternion( w, -x, -y, -z ) );
	}

	const quaternion inv( ) const
	{
		value_type length_ = length( );
		return( conjugate( ) / ( length_ * length_ ) );
	}

	const quaternion unit( ) const
	{
		value_type length_ = length( );
		return quaternion( w / length_, x / length_, y / length_, z / length_ );
	}

	value_type length( ) const { return ( static_cast< value_type >( sqrt( (double)( w * w + x * x + y * y + z * z ) ) ) ); }

	// �x�N�g���̉�]
	template < class TT >
	const vector3< TT > rotate( const vector3< TT > &v ) const
	{
		quaternion q = ( *this ) * quaternion( 0, static_cast< value_type >( v.x ), static_cast< value_type >( v.y ), static_cast< value_type >( v.z ) ) * inv( );
		return( vector3< TT >( static_cast< TT >( q.x ), static_cast< TT >( q.y ), static_cast< TT >( q.z ) ) );
	}
};

template < class T > inline const quaternion< T > operator +( const quaternion< T > &q1, const quaternion< T > &q2 ){ return( quaternion< T >( q1 ) += q2 ); }
template < class T > inline const quaternion< T > operator -( const quaternion< T > &q1, const quaternion< T > &q2 ){ return( quaternion< T >( q1 ) -= q2 ); }
template < class T > inline const quaternion< T > operator /( const quaternion< T > &q1, const quaternion< T > &q2 ){ return( quaternion< T >( q1 ) /= q2 ); }
template < class T > inline const quaternion< T > operator *( const quaternion< T > &q1, const quaternion< T > &q2 ){ return( q1.outer( q2 ) ); }

template < class T > inline const quaternion< T > operator *( const quaternion< T > &q, const typename quaternion< T >::value_type &a ){ return( quaternion< T >( q ) *= a ); }
template < class T > inline const quaternion< T > operator *( const typename quaternion< T >::value_type &a, const quaternion< T > &q ){ return( quaternion< T >( q ) *= a ); }
template < class T > inline const quaternion< T > operator /( const quaternion< T > &q, const typename quaternion< T >::value_type &a ){ return( quaternion< T >( q ) /= a ); }
template < class T > inline const quaternion< T > operator +( const quaternion< T > &q, const typename quaternion< T >::value_type &a ){ return( quaternion< T >( q ) += a ); }
template < class T > inline const quaternion< T > operator -( const quaternion< T > &q, const typename quaternion< T >::value_type &a ){ return( quaternion< T >( q ) -= a ); }

template < class T > inline std::ostream &operator <<( std::ostream &out, const quaternion< T > &q )
{
	out << "( ";
	out << q.w << ", ";
	out << q.x << ", ";
	out << q.y << ", ";
	out << q.z << " )";
	return( out );
}

// �N�H�[�^�j�I������s��֕ϊ�����
//template < class T >
//matrix< T > rotate_matrix( const quaternion< T > &q )
//{
//	matrix< T > mat(  4, 4  );
//	mat( 0, 0 ) = q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z;
//	mat( 0, 1 ) = 2 * ( q.x * q.y - q.w * q.z );
//	mat( 0, 2 ) = 2 * ( q.x * q.z + q.w * q.y );
//	mat( 0, 3 ) = 0;
//	mat( 1, 0 ) = 2 * ( q.x * q.y + q.w * q.z );
//	mat( 1, 1 ) = q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z;
//	mat( 1, 2 ) = 2 * ( q.y * q.z - q.w * q.x );
//	mat( 1, 3 ) = 0;
//	mat( 2, 0 ) = 2 * ( q.x * q.z - q.w * q.y );
//	mat( 2, 1 ) = 2 * ( q.y * q.z + q.w * q.x );
//	mat( 2, 2 ) = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
//	mat( 2, 3 ) = 0;
//	mat( 3, 0 ) = 0;
//	mat( 3, 1 ) = 0;
//	mat( 3, 2 ) = 0;
//	mat( 3, 3 ) = 1;
//	return( mat );
//}



// mist���O��Ԃ̏I���
_MIST_END

#endif // __INCLUDE_MIST_QUATERNION__