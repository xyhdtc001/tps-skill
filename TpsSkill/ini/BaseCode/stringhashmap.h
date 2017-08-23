#pragma once
#include <map>
#include "ref_ptr.h"
#include "basefunc.h"
#include "StringData.h"

template < class T >
class StringHashMap
{	
public:
	ref_ptr<T> find( const string& key )
	{
		uint32 hashVal = fastHash( key.c_str(), key.length() );
		typename std::map<uint32,std::map<string,ref_ptr<T > > >::iterator iter = _mapMap.find( hashVal );
		if ( iter == _mapMap.end() )
			return NULL;
		typename std::map<string,ref_ptr<T> >::iterator it = iter->second.find( key );
		if ( it == iter->second.end() )
			return NULL;
		return it->second;
	}

	void erase( const string& key )
	{
		uint32 hashVal = fastHash( key.c_str(), key.length() );
		typename std::map<uint32,std::map<string,ref_ptr<T> > >::iterator iter = _mapMap.find( hashVal );
		if ( iter == _mapMap.end() )
			return;
		typename std::map<string,ref_ptr<T> >::iterator it = iter->second.find( key );
		if ( it == iter->second.end() )
			return;
		iter->second.erase( it );
	}

	void set( const string& key, T* val )
	{
		uint32 hashVal = fastHash( key.c_str(), key.length(), 0 );
		_mapMap[hashVal][key] = val;
	}

	void clear()
	{
		_mapMap.clear();
	}

	uint32 size()
	{
		return (uint32)_mapMap.size();
	}

public:
	std::map<uint32,std::map<string,ref_ptr<T > > > _mapMap;
};


template < class T >
class DataHashMap
{	
public:
	ref_ptr<T> find( const Data& key )
	{
		uint32 hashVal = fastHash( key.c_str(), key.length() );
		typename std::map<uint32,std::map<Data,ref_ptr<T> > >::iterator iter = _mapMap.find( hashVal );
		if ( iter == _mapMap.end() )
			return NULL;
		typename std::map<Data,ref_ptr<T> >::iterator it = iter->second.find( key );
		if ( it == iter->second.end() )
			return NULL;
		return it->second;
	}

	void set( const Data& key, T* val )
	{
		uint32 hashVal = fastHash( key.c_str(), key.length(), 0 );
		_mapMap[hashVal][key] = val;
	}

	void erase( const Data& key )
	{
		uint32 hashVal = fastHash( key.c_str(), key.length() );
		typename std::map<uint32,std::map<Data,ref_ptr<T> > >::iterator iter = _mapMap.find( hashVal );
		if ( iter == _mapMap.end() )
			return;
		typename std::map<Data,ref_ptr<T> >::iterator it = iter->second.find( key );
		if ( it == iter->second.end() )
			return;
		iter->second.erase( it );
	}

	void clear()
	{
		_mapMap.clear();
	}

	uint32 size()
	{
		return _mapMap.size();
	}

public:
	std::map<uint32,std::map<Data,ref_ptr<T> > > _mapMap;
};
