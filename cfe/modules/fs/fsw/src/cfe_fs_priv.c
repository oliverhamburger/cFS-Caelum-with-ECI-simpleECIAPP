/*
**  GSC-18128-1, "Core Flight Executive Version 6.7"
**
**  Copyright (c) 2006-2019 United States Government as represented by
**  the Administrator of the National Aeronautics and Space Administration.
**  All Rights Reserved.
**
**  Licensed under the Apache License, Version 2.0 (the "License");
**  you may not use this file except in compliance with the License.
**  You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software
**  distributed under the License is distributed on an "AS IS" BASIS,
**  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**  See the License for the specific language governing permissions and
**  limitations under the License.
*/

/*
** File: cfe_fs_priv.c
**
** Purpose:  cFE File Services (FS) library API Initialization
**
** Author:  Alan Cudmore/NASA GSFC
**
** Notes:
**
*/

/*
** Required header files
*/
#include "cfe_fs_module_all.h"

#include <string.h>

/*
** Global data
**
*/
CFE_FS_Global_t CFE_FS_Global;

/*----------------------------------------------------------------
 *
 * Function: CFE_FS_EarlyInit
 *
 * Implemented per public API
 * See description in header file for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 CFE_FS_EarlyInit(void)
{
    int32 Stat;

    memset(&CFE_FS_Global, 0, sizeof(CFE_FS_Global));

    Stat = OS_MutSemCreate(&CFE_FS_Global.SharedDataMutexId, "CFE_FS_SharedMutex", 0);
    if (Stat != OS_SUCCESS)
    {
        CFE_ES_WriteToSysLog("%s: Shared Data Mutex creation failed! RC=0x%08x\n", __func__, (unsigned int)Stat);
        return Stat;
    } /* end if */

    return Stat;
}

/*----------------------------------------------------------------
 *
 * Function: CFE_FS_LockSharedData
 *
 * Application-scope internal function
 * See description in header file for argument/return detail
 *
 *-----------------------------------------------------------------*/
void CFE_FS_LockSharedData(const char *FunctionName)
{
    int32          Status;
    CFE_ES_AppId_t AppId;

    Status = OS_MutSemTake(CFE_FS_Global.SharedDataMutexId);
    if (Status != OS_SUCCESS)
    {
        CFE_ES_GetAppID(&AppId);

        CFE_ES_WriteToSysLog("%s: SharedData Mutex Take Err Stat=0x%x,App=%lu,Function=%s\n", __func__,
                             (unsigned int)Status, CFE_RESOURCEID_TO_ULONG(AppId), FunctionName);

    } /* end if */

    return;
}

/*----------------------------------------------------------------
 *
 * Function: CFE_FS_UnlockSharedData
 *
 * Application-scope internal function
 * See description in header file for argument/return detail
 *
 *-----------------------------------------------------------------*/
void CFE_FS_UnlockSharedData(const char *FunctionName)
{
    int32          Status;
    CFE_ES_AppId_t AppId;

    Status = OS_MutSemGive(CFE_FS_Global.SharedDataMutexId);
    if (Status != OS_SUCCESS)
    {
        CFE_ES_GetAppID(&AppId);
        CFE_ES_WriteToSysLog("%s: SharedData Mutex Give Err Stat=0x%x,App=%lu,Function=%s\n", __func__,
                             (unsigned int)Status, CFE_RESOURCEID_TO_ULONG(AppId), FunctionName);

    } /* end if */
    return;
}
