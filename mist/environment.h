/// @file mist/environment.h
//!
//! @brief 計算機の環境情報を扱うためのライブラリ
//!
#ifndef __INCLUDE_MIST_ENVIRONMENT__
#define __INCLUDE_MIST_ENVIRONMENT__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_SINGLETON__
#include "singleton.h"
#endif

#include <string>

#ifdef WIN32

	#include <windows.h>
	#include <shlwapi.h>
	#pragma comment( lib, "shlwapi.lib" )

#else

	//#include <pwd.h>
	#include <iostream>
	#include <fstream>

#endif


// mist名前空間の始まり
_MIST_BEGIN


namespace __environment__
{
#ifdef WIN32
	static std::string cpu( size_t &num_processors )
	{
		SYSTEM_INFO info;
		memset( &info, 0, sizeof( info ) );
		GetSystemInfo( &info );

		char value[ 1024 ];
		DWORD type = REG_SZ;
		DWORD length = 1024;
		std::string cpu = "unknown";
		std::string machine = "unknown";

		if( SHGetValue( HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString", &type, value, &length ) == ERROR_SUCCESS )
		{
			cpu = value;
			size_t index = 0;

			for( size_t i = 0 ; i < cpu.size( ) ; i++ )
			{
				if( cpu[ i ] == ' ' )
				{
					index++;
				}
				else
				{
					break;
				}
			}

			cpu = cpu.substr( index );

			num_processors = info.dwNumberOfProcessors;
		}

		return( cpu );
	}

	static std::string user( )
	{
		char value[ 1024 ];
		DWORD length = 1024;

		if( GetUserName( value, &length ) )
		{
			return ( std::string( value ) );
		}
		else
		{
			return ( "unknown" );
		}
	}

	static std::string machine( )
	{
		char value[ 1024 ];
		DWORD length = 1024;

		if( GetComputerName( value, &length ) )
		{
			return ( std::string( value ) );
		}
		else
		{
			return ( "unknown" );
		}
	}

	static std::string os( )
	{
		std::string os = "unknown";
		OSVERSIONINFO osinfo;
		memset( &osinfo, 0, sizeof( osinfo ) );
		osinfo.dwOSVersionInfoSize = sizeof( osinfo );

		if( GetVersionEx( &osinfo ) )
		{
			switch( osinfo.dwPlatformId )
			{
			case VER_PLATFORM_WIN32s:
				os = "Win32s on Windows 3.1";
				break;

			case VER_PLATFORM_WIN32_WINDOWS:
				switch( osinfo.dwMinorVersion )
				{
				case 0:
					os = "Windows 95";
					break;

				case 10:
					os = "Windows 98";
					break;

				case 90:
					os = "Windows Me";
					break;
				}
				break;

			case VER_PLATFORM_WIN32_NT:
				if( osinfo.dwMajorVersion == 5 )
				{
					switch( osinfo.dwMinorVersion )
					{
					case 0:
						os = "Windows 2000";
						break;

					case 1:
						os = "Windows XP";
						break;

					case 2:
						os = "Windows Server 2003 family";
						break;
					}
				}
				else if( osinfo.dwMajorVersion == 3 )
				{
					os = "Windows NT 3.51";
				}
				else if( osinfo.dwMajorVersion == 4 )
				{
					os = "Windows NT 4.0";
				}
				break;

			default:
				break;
			}
		}

		return( os );
	}

#else

	// LINUX 専用

	static std::string cpu( size_t &num_processors )
	{
		typedef std::string::size_type size_type;
		std::ifstream fi;

		fi.open( "/proc/cpuinfo" );

		if( !fi )
		{
			std::cerr << "CPU情報が見つからないよ！！" << std::endl;
			return( "unknown" );
		}

		std::string cpuinfo = "";

		char buff[ 4096 ];
		while( fi )
		{
			fi.getline( buff, 4094 );
			cpuinfo += buff;
			cpuinfo += "\n";
		}

		fi.close( );

		// CPU数を取得する
		size_type indx = cpuinfo.find( "processor", 0 );
		size_type num_cpu = 0;
		while( indx != cpuinfo.npos )
		{
			num_cpu++;
			indx = cpuinfo.find( "processor", indx + 1 );
		}

		num_processors = num_cpu;

		// CPUの種類を取得する
		std::string cpuname = "";
		indx = cpuinfo.find( "model name", 0 );
		if( indx != cpuinfo.npos )
		{
			size_type sindx = cpuinfo.find( ':', indx );
			size_type eindx = cpuinfo.find( '\n', indx );
			if( sindx != cpuinfo.npos && eindx != cpuinfo.npos )
			{
				cpuname = cpuinfo.substr( sindx + 2, eindx - sindx - 2 );
				return( cpuname );
			}
		}

		return( "unknown" );
	}

	static std::string chomp( const std::string &str )
	{
		if( str.empty( ) )
		{
			return( "" );
		}
		else if( str.length( ) > 1 )
		{
			if( str[ str.length( ) - 1 ] == '\n' )
			{
				if( str[ str.length( ) - 2 ] == '\r' )
				{
					return( str.substr( 0, str.length( ) - 2 ) );
				}
				else
				{
					return( str.substr( 0, str.length( ) - 1 ) );
				}
			}
		}
		else if( str[ 0 ] == '\n' )
		{
			return( "" );
		}

		return( str );
	}

	static std::string user( )
	{
		FILE *gid = popen( "whoami", "r" );

		if( gid == NULL )
		{
			return( "unknown" );
		}

		char buff[ 4096 ];

		fgets( buff, 4096, gid );

		pclose( gid );

		return( chomp( std::string( buff ) ) );
	}

	static std::string machine( )
	{
		FILE *gid = popen( "uname -n", "r" );

		if( gid == NULL )
		{
			return( "unknown" );
		}

		char buff[ 4096 ];

		fgets( buff, 4096, gid );

		pclose( gid );

		return( chomp( std::string( buff ) ) );
	}

	static std::string os( )
	{
		typedef std::string::size_type size_type;
		std::ifstream fi;

		fi.open( "/proc/version" );

		if( !fi )
		{
			std::cerr << "OS情報が見つからないよ！！" << std::endl;
			return( "unknown, a debian linux?" );
		}

		std::string osinfo = "";

		char buff[ 4096 ];
		while( fi )
		{
			fi.getline( buff, 4094 );
			osinfo += buff;
			osinfo += "\n";
		}

		fi.close( );

		// OSの情報だけを取得する
		size_type indx = osinfo.find( '(', 0 );

		if( indx != osinfo.npos )
		{
			return( osinfo.substr( 0, indx - 1 ) );
		}

		return( osinfo );
	}

#endif

	struct environment_info
	{
		std::string	os_name;
		std::string	cpu_name;
		size_t		cpu_num;
		std::string	machine_name;
		std::string	user_name;

		environment_info( ) :
			os_name( os( ) ),
			machine_name( machine( ) ),
			user_name( user( ) )
		{
			cpu_name = cpu( cpu_num );
		}
	};
}

/// @brief OS情報を返す関数
//! 
//! @attention WindowsおよびLinuxのみでの動作保証
//! 
inline std::string os( ){ return( singleton< __environment__::environment_info >::get_instance( ).os_name ); }

/// @brief CPU情報を返す関数
//! 
//! @attention WindowsおよびLinuxのみでの動作保証
//! 
inline std::string cpu( ){ return( singleton< __environment__::environment_info >::get_instance( ).cpu_name ); }

/// @brief CPU情報を返す関数
//! 
//! @attention WindowsおよびLinuxのみでの動作保証
//! 
inline size_t cpu_num( ){ return( singleton< __environment__::environment_info >::get_instance( ).cpu_num ); }

/// @brief 計算機名を返す関数
//! 
//! @attention WindowsおよびLinuxのみでの動作保証
//! 
inline std::string machine( ){ return( singleton< __environment__::environment_info >::get_instance( ).machine_name ); }

/// @brief 現在のユーザー情報を返す関数
//! 
//! @attention WindowsおよびLinuxのみでの動作保証
//! 
inline std::string user( ){ return( singleton< __environment__::environment_info >::get_instance( ).user_name ); }


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_ENVIRONMENT__
