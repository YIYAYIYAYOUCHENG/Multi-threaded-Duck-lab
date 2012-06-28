/*
 * This file is part of duck-lab.
 * 
 * Copyright (c) 
 * 2012 - Christian Nastasi, Claudio Salvadori 
 *
 * duck-lab is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * duck-lab is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */
/** 
* @file DataUInt8.cpp
* @brief DataUInt8 Implementation File 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "DataUInt8.h"
#include "DataFactory.h"
#include "DuckLog.h"

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif

using namespace std;

DUCK_REGISTER_DATA_CLASS(DataUInt8, DATA_UINT8_ID);

DataUInt8::DataUInt8() 
	: DataTemplate<DATA_UINT8_TYPE>(DataConfig(DATA_UINT8_ID))
{ 
	DUCK_DBG("DataUInt8: CHILD Construct Void");
}

DataUInt8::DataUInt8(const string &n, unsigned len, DATA_UINT8_TYPE *bf)
: DataTemplate<DATA_UINT8_TYPE>(DataConfig(DATA_UINT8_ID, n, len), bf)
{
	DUCK_DBG("DataUInt8: CHILD Construct Name-Len-InitData");
}

DataUInt8::DataUInt8(const DataConfig& c) 
	: DataTemplate<DATA_UINT8_TYPE>(c)
{ 
	DUCK_DBG("DataUInt8: CHILD Construct Config");
}

DataUInt8::DataUInt8(const DataConfig& c, DATA_UINT8_TYPE *bf) 
	: DataTemplate<DATA_UINT8_TYPE>(c, bf)
{ 
	DUCK_DBG("DataUInt8: CHILD Construct Config + InitData");
}

DataUInt8::~DataUInt8()
{
	DUCK_DBG("DataUInt8: CHILD Destruct");
}
