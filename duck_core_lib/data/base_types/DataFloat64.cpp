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
* @file DataFloat64.cpp
* @brief DataFloat64 Implementation File 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "DataFloat64.h"
#include "DataFactory.h"
#include "DuckLog.h"

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif

using namespace std;

DUCK_REGISTER_DATA_CLASS(DataFloat64, DATA_FLOAT64_ID);

DataFloat64::DataFloat64() 
	: DataTemplate<DATA_FLOAT64_TYPE>(DataConfig(DATA_FLOAT64_ID))
{ 
	DUCK_DBG("DataFloat64: CHILD Construct Void");
}

DataFloat64::DataFloat64(const string &n, unsigned len, DATA_FLOAT64_TYPE *bf)
: DataTemplate<DATA_FLOAT64_TYPE>(DataConfig(DATA_FLOAT64_ID, n, len), bf)
{
	DUCK_DBG("DataFloat64: CHILD Construct Name-Len-InitData");
}

DataFloat64::DataFloat64(const DataConfig& c) 
	: DataTemplate<DATA_FLOAT64_TYPE>(c)
{ 
	DUCK_DBG("DataFloat64: CHILD Construct Config");
}

DataFloat64::DataFloat64(const DataConfig& c, DATA_FLOAT64_TYPE *bf) 
	: DataTemplate<DATA_FLOAT64_TYPE>(c, bf)
{ 
	DUCK_DBG("DataFloat64: CHILD Construct Config + InitData");
}

DataFloat64::~DataFloat64()
{
	DUCK_DBG("DataFloat64: CHILD Destruct");
}
