#ifndef __INCLUDE_MIST_OPTIONS__
#define __INCLUDE_MIST_OPTIONS__


#ifndef __INCLUDE_MIST_CONF_H__
#include "../config/mist_conf.h"
#endif

#include <iostream>
#include <vector>
#include <map>
#include <string>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN

/// @brief �R�}���h���C����������͂���N���X
//! 
class options : public std::vector< std::string >
{
public:
	typedef std::vector< std::string > base;	///< @brief ���N���X

private:
	/// @brief �I�v�V�������L������N���X
	struct arg
	{
		std::string name;		///< @brief �I�v�V������
		std::string comment;	///< @brief �I�v�V�����̐���
		std::string value;		///< @brief �I�v�V�����̒l
		bool has_value;			///< @brief �l�Ƌ��Ɏw�肳���I�v�V�������ǂ����������t���O
		bool found;				///< @brief �I�v�V�����Ɏw�肳�ꂽ���ǂ���

		/// @brief �f�t�H���g�R���X�g���N�^
		arg( const std::string &aname = "", const std::string &text = "", const std::string &val = "", bool hasValue = false )
			: name( aname ), comment( text ), value( val ), has_value( hasValue ), found( false )
		{
		}

		/// @brief �R�s�[�R���X�g���N�^
		arg( const arg &p ) : name( p.name ), comment( p.comment ), value( p.value ), has_value( p.has_value ), found( p.found )
		{
		}
	};

	std::string                  header_text;		///< @brief �w���v�̐擪�ɕ\������镶����
	std::string                  footer_text;		///< @brief �w���v�̖����ɕ\������镶����
	std::string                  program_name;		///< @brief �v���O��������ێ����镶����
	std::vector< std::string >   option_list;		///< @brief �w�肳���\���̂���I�v�V�����̃��X�g
	std::map< std::string, arg > args;				///< @brief �w�肳���\���̂���I�v�V�����̃��X�g

public:
	/// @brief �f�t�H���g�R���X�g���N�^
	options( const std::string header = "", const std::string footer = "" ) : header_text( header ), footer_text( footer )
	{
	}

	/// @brief �R�s�[�R���X�g���N�^
	options( const options &o ) : base( o ), program_name( o.program_name ), args( o.args )
	{
	}

protected:
	/// @brief �I�v�V�����Ɏw�肳��Ă���l���擾����
	bool __isset__( const std::string &name, std::string &val ) const
	{
		std::map< std::string, arg >::const_iterator ite = args.find( name );
		if( ite != args.end( ) )
		{
			const arg &a = ite->second;
			val = a.value;
			return( true );
		}
		else
		{
			return( false );
		}
	}

public:
	/// @brief �l�����̃I�v�V������ǉ�����
	void add( const std::string &name, const std::string &comment )
	{
		option_list.push_back( name );
		args[ name ] = arg( name, comment, "", false );
	}

	/// @brief �l���܂ރI�v�V������ǉ�����i�f�t�H���g�l���w�肷��ƃI�v�V�������w�肳��Ȃ��ꍇ���l���擾�\�j
	void add( const std::string &name, const std::string &comment, const std::string &default_value )
	{
		option_list.push_back( name );
		args[ name ] = arg( name, comment, default_value, true );
	}

	/// @brief �l���܂ރI�v�V������ǉ�����i�f�t�H���g�l���w�肷��ƃI�v�V�������w�肳��Ȃ��ꍇ���l���擾�\�j
	void add( const std::string &name, const std::string &comment, int default_value )
	{
		char buff[ 50 ];
		sprintf( buff, "%d", default_value );
		option_list.push_back( name );
		args[ name ] = arg( name, comment, buff, true );
	}

	/// @brief �l���܂ރI�v�V������ǉ�����i�f�t�H���g�l���w�肷��ƃI�v�V�������w�肳��Ȃ��ꍇ���l���擾�\�j
	void add( const std::string &name, const std::string &comment, double default_value )
	{
		char buff[ 50 ];
		sprintf( buff, "%lf", default_value );
		option_list.push_back( name );
		args[ name ] = arg( name, comment, buff, true );
	}

	/// @brief �I�v�V�������R�}���h���C�����������Ɏw�肳�ꂽ���ǂ����𒲂ׂ�
	bool isset( const std::string &name ) const
	{
		std::map< std::string, arg >::const_iterator ite = args.find( name );
		if( ite != args.end( ) )
		{
			const arg &a = ite->second;
			return( a.found );
		}
		else
		{
			return( false );
		}
	}

	/// @brief �I�v�V�����Ɏw�肳�ꂽ��������擾����
	const std::string get_string( const std::string &name ) const
	{
		std::string val;
		if( __isset__( name, val ) )
		{
			return( val );
		}
		else
		{
			return( "" );
		}
	}

	/// @brief �I�v�V�����Ɏw�肳�ꂽ�l���擾����
	int get_int( const std::string &name ) const
	{
		std::string val;
		if( __isset__( name, val ) )
		{
			return( atoi( val.c_str( ) ) );
		}
		else
		{
			return( 0 );
		}
	}

	/// @brief �I�v�V�����Ɏw�肳�ꂽ�l���擾����
	double get_double( const std::string &name ) const
	{
		std::string val;
		if( __isset__( name, val ) )
		{
			return( atof( val.c_str( ) ) );
		}
		else
		{
			return( 0.0 );
		}
	}

public:
	/// @brief �I�v�V�����̈ꗗ��W���o�͂ɕ\������
	void show_help( ) const
	{
		// �w�b�_�������o�͂���
		std::cout << header_text << std::endl;

		size_t max_len = 0;
		for( size_t i = 0 ; i < option_list.size( ) ; i++ )
		{
			if( max_len < option_list[ i ].size( ) )
			{
				max_len = option_list[ i ].size( );
			}
		}

		for( size_t i = 0 ; i < option_list.size( ) ; i++ )
		{
			std::map< std::string, arg >::const_iterator ite = args.find( option_list[ i ] );
			if( ite != args.end( ) )
			{
				const arg &a = ite->second;

				std::cout << "-";
				std::cout << a.name;

				for( size_t i = a.name.size( ) ; i < max_len + 3 ; i++ )
				{
					std::cout << ' ';
				}

				std::cout << a.comment;

				if( a.has_value )
				{
					std::cout << "[" << a.value << "]";
				}

				if( a.found )
				{
					std::cout << "*";
				}
				std::cout << std::endl;
			}
			else
			{
				std::cerr << "�I�v�V�����̉�͂Ɏ��s���܂����D" << std::endl;
			}
		}

		// �t�b�^�������o�͂���
		std::cout << footer_text << std::endl;
	}

	/// @brief �v���O������������͂��āC�I�v�V�����Ƃ̑Ή��֌W�����߂�
	bool parse_args( int argc, char *argv[] )
	{
		program_name = argv[ 0 ];

		bool ret = true;

		for( int i = 1 ; i < argc ; i++ )
		{
			std::string option = argv[ i ];

			if( option[ 0 ] != '-' && option[ 0 ] != '/' )
			{
				base::push_back( option );
			}
			else
			{
				option = option.substr( 1 );

				std::map< std::string, arg >::iterator ite = args.find( option );
				if( ite != args.end( ) )
				{
					arg &a = ite->second;
					a.found = true;

					if( a.has_value )
					{
						if( i + 1 < argc && ( argv[ i + 1 ][ 0 ] != '-' && argv[ i + 1 ][ 0 ] != '/' ) )
						{
							a.value = argv[ i + 1 ];
							i++;
						}
						else
						{
							std::cerr << "�I�v�V���� \"" << option << "\" �͒l���w�肷��K�v������܂��D" << std::endl;
							ret = false;
						}
					}
				}
				else
				{
					std::cerr << "\"" << option << "\" �͕s���ȃI�v�V�����ł��D" << std::endl;
					ret = false;
				}
			}
		}

		if( !ret )
		{
			std::cerr << std::endl;
		}

		return( ret );
	}
};

// mist���O��Ԃ̏I���
_MIST_END


#endif	// __INCLUDE_MIST_OPTIONS__