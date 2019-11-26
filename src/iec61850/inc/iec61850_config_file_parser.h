/*
 *  config_file_parser.h
 *
 *  Copyright 2014 Michael Zillgith
 *
 *  This file is part of libIEC61850.
 *
 *  libIEC61850 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libIEC61850 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 */

#ifndef CONFIG_FILE_PARSER_H_
#define CONFIG_FILE_PARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_filesystem.h"

/** \addtogroup server_api_group
 *  @{
 */

/**
 * @defgroup CONFIG_FILE_PARSER Create data models by configuration files
 *
 * @{
 */

/**
 * \brief Create a data model from simple text configuration file
 *
 * \param filename name or path of the configuraton file
 *
 * \return the data model to be used by \ref IedServer
 */
LIB61850_API IedModel*
ConfigFileParser_createModelFromConfigFileEx(const char* filename);

LIB61850_API IedModel*
ConfigFileParser_createModelFromConfigFile(FileHandle fileHandle);

/**@}*/

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* CONFIG_FILE_PARSER_H_ */
