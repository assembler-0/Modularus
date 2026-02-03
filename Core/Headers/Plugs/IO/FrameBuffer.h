#pragma once

#include <__KCONF.h>
#include <Modules/KExports.h>
#include <Errors.h>

typedef enum
{
    PixelFormat_RGB,
    PixelFormat_BGR,
    PixelFormat_RGBX,
    PixelFormat_BGRX,
    PixelFormat_XRGB,
    PixelFormat_XBGR,
    PixelFormat_RGBA,
    PixelFormat_BGRA,
    PixelFormat_ARGB,
    PixelFormat_ABGR,
    PixelFormat_RGB565,
    PixelFormat_BGR565,
    PixelFormat_RGB555,
    PixelFormat_BGR555,
    PixelFormat_RGB444,
    PixelFormat_BGR444,
    PixelFormat_RGB332,
    PixelFormat_BGR233,
    PixelFormat_GRAYSCALE8,
    PixelFormat_GRAYSCALE16,
    PixelFormat_INDEXED8,
    PixelFormat_INDEXED4,
    PixelFormat_INDEXED2,
    PixelFormat_INDEXED1,
    PixelFormat_YUV422,
    PixelFormat_YUV420,
    PixelFormat_YUV444,
    PixelFormat_UNKNOWN
} PIXEL_FORMAT;

typedef struct
{
    void*         Address;
    uint64_t      Width;
    uint64_t      Height;
    uint64_t      Pitch;
    uint16_t      BPP;
    PIXEL_FORMAT  Format;
} FRAMEBUFFER_INFORMATION;

typedef struct FRAMEBUFFER_OPERATIONS
{
    int (*Init)(FRAMEBUFFER_INFORMATION*, SYSTEM_ERROR*);
    void (*Exit)(SYSTEM_ERROR*);
} FRAMEBUFFER_OPERATIONS;

typedef struct
{
    FRAMEBUFFER_INFORMATION  Info;
    FRAMEBUFFER_OPERATIONS*  Operations;
    void*                    Private;
} FRAMEBUFFER;

extern FRAMEBUFFER* ActiveFrameBuffer;

int RegisterFrameBuffer(FRAMEBUFFER*, SYSTEM_ERROR*);
FRAMEBUFFER* GetFrameBuffer(SYSTEM_ERROR*);

int ExitFrameBuffer(FRAMEBUFFER* FrameBuffer, SYSTEM_ERROR* Error);
int InitFrameBuffer(FRAMEBUFFER* FrameBuffer, SYSTEM_ERROR* Error);

KEXPORT(RegisterFrameBuffer)
KEXPORT(GetFrameBuffer)

KEXPORT(ActiveFrameBuffer)

KEXPORT(InitFrameBuffer)
KEXPORT(ExitFrameBuffer)