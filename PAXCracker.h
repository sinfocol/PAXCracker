/*
 *  Copyright (C) 2013 Daniel Correa, http://www.sinfocol.org/
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */  

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

EXTERN __declspec(dllexport) void    __stdcall _is5_Initialize(const char *pKey);
EXTERN __declspec(dllexport) void    __stdcall _is5_Exit();
EXTERN __declspec(dllexport) HANDLE  __stdcall _is5_OpenFileSource(const char *pFileName);
EXTERN __declspec(dllexport) UINT32  __stdcall _is5_CloseSource(HANDLE hSource);
EXTERN __declspec(dllexport) BOOL    __stdcall _is5_Seek(HANDLE hMgr, __int32 uOffset, UINT32 uOrigin);
EXTERN __declspec(dllexport) HGLOBAL __stdcall _is5_ReadPAX(HANDLE hSource, UINT32 *puWidth, UINT32 *puHeight, UINT32 uBitsPerPixel, RGBQUAD *pPal, const char *pPassword, UINT32 uFlags);
EXTERN __declspec(dllexport) UINT32  __stdcall _is5_GuessFileType(HANDLE hSource);

#ifdef __cplusplus
}
#endif