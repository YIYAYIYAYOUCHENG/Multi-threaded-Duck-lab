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
* @file DataUInt32.cpp
* @brief DataUInt32 Implementation File 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "DataUInt32.h"
#include "DataFactory.h"
#include "DuckLog.h"

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif

using namespace std;

DUCK_REGISTER_DATA_CLASS(DataUInt32, DATA_UINT32_ID);

DataUInt32::DataUInt32() 
	: DataTemplate<DATA_UINT32_TYPE>(DataConfig(DATA_UINT32_ID))
{ 
	DUCK_DBG("DataUInt32: CHILD Construct Void");
}

DataUInt32::DataUInt32(const string &n, unsigned len, DATA_UINT32_TYPE *bf)
: DataTemplate<DATA_UINT32_TYPE>(DataConfig(DATA_UINT32_ID, n, len), bf)
{
	DUCK_DBG("DataUInt32: CHILD Construct Name-Len-InitData");
}

DataUInt32::DataUInt32(const DataConfig& c) 
	: DataTemplate<DATA_UINT32_TYPE>(c)
{ 
	DUCK_DBG("DataUInt32: CHILD Construct Config");
}

DataUInt32::DataUInt32(const DataConfig& c, DATA_UINT32_TYPE *bf) 
	: DataTemplate<DATA_UINT32_TYPE>(c, bf)
{ 
	DUCK_DBG("DataUInt32: CHILD Construct Config + InitData");
}

DataUInt32::~DataUInt32()
{
	DUCK_DBG("DataUInt32: CHILD Destruct");
}
