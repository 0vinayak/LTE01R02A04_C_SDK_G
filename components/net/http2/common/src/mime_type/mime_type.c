/*****************************************************************************
** File Name:      mime_type.c                                               *
** Author:                                                                   *
** Date:           12/8/2022                                                *
** Copyright:      2022 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    OMA DRM v1.0: mime type                                   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/18/2008     @681             Create
** 12/08/2022     chenchen.wang    Modified
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mime_type.h"
#include "cfl_char.h"

/*---------------------------------------------------------------------------*/
/*                          STRUCT                                           */
/*---------------------------------------------------------------------------*/
typedef struct
{
  uint32 value;     // MIME_TYPE_E
  const char *mime_ptr;
  const char *ext_ptr;
} MIME_INFO_T;


/*---------------------------------------------------------------------------*/
/*                          VARIABLE                                         */
/*---------------------------------------------------------------------------*/
static const MIME_INFO_T s_mime_table[] =
{
  {MIME_TYPE_ANY_ANY,               "*/*",              ""},
  {MIME_TYPE_TEXT_ANY,              "text/*",           ""},
  {MIME_TYPE_TEXT_HTML,             "text/html",        "html"},
  {MIME_TYPE_TEXT_HTML,             "application/xhtml+xml","html"},
  {MIME_TYPE_TEXT_HTML,             "application/vnd.wap.xhtml+xml","html"},
  {MIME_TYPE_TEXT_HTML,             "application/xhtml","html"},
  {MIME_TYPE_TEXT_HTML,             "text/orangeml","html"},
  {MIME_TYPE_TEXT_WML,              "application/wml+xml","wml"},
  {MIME_TYPE_TEXT_WMLS,            "text/vnd.wap.wmlscript","wmls"},
  {MIME_TYPE_TEXT_WMLSC,          "application/vnd.wap.wmlscriptc","wmlsc"},
  {MIME_TYPE_TEXT_WMLC,            "application/vnd.wap.wmlc","wmlc"},
  {MIME_TYPE_TEXT_WMLC,            "application/vnd.wap.wbxml","wmlc"},
  {MIME_TYPE_TEXT_PLAIN,            "text/plain",       "txt"},
  {MIME_TYPE_TEXT_CSS,              "text/css","css"},
  {MIME_TYPE_TEXT_X_VCALENDAR,      "text/x-vcalendar", "vcs"},
  {MIME_TYPE_TEXT_X_VCARD,          "text/x-vcard",     "vcf"},
  {MIME_TYPE_APPLICATION_ANY,       "application/*",    ""},
  {MIME_TYPE_APPLICATION_JAVA_VM,   "application/java-vm",              "class"},
  {MIME_TYPE_APPLICATION_JAVA_JAR,  "application/java-archive",         "jar"},
  {MIME_TYPE_APPLICATION_JAVA_JAR,  "application/x-java-archive",         "jar"},
  {MIME_TYPE_APPLICATION_JAVA_JAD,  "text/vnd.sun.j2me.app-descriptor", "jad"},
  {MIME_TYPE_IMAGE_ANY,             "image/*",              ""},
  {MIME_TYPE_IMAGE_GIF,             "image/gif",            "gif"},
  {MIME_TYPE_IMAGE_JPEG,            "image/jpg",           "jpg"},
  {MIME_TYPE_IMAGE_JPEG,            "image/jpeg",           "jpeg"},
  {MIME_TYPE_IMAGE_TIFF,            "image/tiff",           "tif"},
  {MIME_TYPE_IMAGE_PNG,             "image/png",            "png"},
  {MIME_TYPE_IMAGE_VND_WAP_WBMP,    "image/vnd.wap.wbmp",   "wbmp"},
  {MIME_TYPE_IMAGE_X_UP_WPNG,       "image/x-up-wpng",      "wpng"},
  {MIME_TYPE_IMAGE_SVG,             "image/svg+xml",        "svg"},
  {MIME_TYPE_IMAGE_BMP,             "image/bmp",            "bmp"},
  {MIME_TYPE_IMAGE_IEF,               "image/ief","ief"},
  {MIME_TYPE_AUDIO_ANY,             "audio/*",              ""},
  {MIME_TYPE_AUDIO_MIDI,            "audio/midi",           "mid"},
  {MIME_TYPE_AUDIO_MID,             "audio/mid",            "mid"},
  {MIME_TYPE_AUDIO_X_MIDI,          "audio/x-midi",         "mid"},
  {MIME_TYPE_AUDIO_SP_MIDI,         "audio/sp-midi",        "mid"},
  {MIME_TYPE_AUDIO_IMELODY,         "audio/imelody",        "imy"},
  {MIME_TYPE_TEXT_X_IMELODY,        "text/x-imelody",       "imy"},
  {MIME_TYPE_AUDIO_MPEG,            "audio/mpg",            "mp3"},
  {MIME_TYPE_AUDIO_MPEG,            "audio/mpeg",           "mp3"},
  {MIME_TYPE_AUDIO_MPEG,            "audio/mp3",          "mp3"},
  {MIME_TYPE_AUDIO_MPEG,            "audio/x-mp3",          "mp3"},
  {MIME_TYPE_AUDIO_X_WAV,           "audio/wav",            "wav"},
  {MIME_TYPE_AUDIO_X_WAV,           "audio/x-wav",          "wav"},
  {MIME_TYPE_AUDIO_WMA,             "audio/x-ms-wma",       "wma"},
  {MIME_TYPE_APPLICATION_VND_SMAF,  "application/vnd.smaf", "mmf"},
  {MIME_TYPE_APPLICATION_X_SMAF,    "application/x-smaf",   "mmf"},
  {MIME_TYPE_APPLICATION_X_MMF,    "application/x-smaf.mmf",   "mmf"},
  {MIME_TYPE_AUDIO_AMR,             "audio/amr",            "amr"},
  {MIME_TYPE_AUDIO_X_AAC,             "audio/x-aac",            "acc"},
  {MIME_TYPE_AUDIO_X_AAC,             "audio/aac",            "acc"},
  {MIME_TYPE_AUDIO_MPA,                 "audio/mpa","mpa"},
  {MIME_TYPE_AUDIO_MPA_ROBUST,      "audio/mpa-robust","mpa"},
  {MIME_TYPE_AUDIO_MPA_LATM,        "audio/mp4a-latm","mpa"},
  {MIME_TYPE_AUDIO_TONE,            "audio/tone","tone"},
  {MIME_TYPE_AUDIO_RM,              "application/vnd.rn-realmedia","rm"},
  {MIME_TYPE_AUDIO_3GP,             "audio/3gp","3gp"},
  {MIME_TYPE_APPLICATION_SMIL,              "application/smil",     "smi"},
  {MIME_TYPE_VIDEO_MPEG,                    "video/mpeg",           "mpg"},
  {MIME_TYPE_VIDEO_3GPP,                    "video/3gpp",           "3gp"},
  {MIME_TYPE_VIDEO_3GPP,                    "video/3gpp2",           "3gp"},
  {MIME_TYPE_VIDEO_AVI,                    "video/x-msvideo",           "avi"},
  {MIME_TYPE_VIDEO_MP4,                    "video/mp4",           "mp4"},
  {MIME_TYPE_VIDEO_WMV,                    "video/x-ms-wmv",           "wmv"},
  {MIME_TYPE_APPLICATION_X_SHOCKWAVE_FLASH, "application/x-shockwave-flash", "swf"},
  {MIME_TYPE_APPLICATION_ZIP,               "application/zip",      "zip"},
  {MIME_TYPE_APPLICATION_VND_OMA_DRM_MESSAGE,       "application/vnd.oma.drm.message",      "dm"    },
  {MIME_TYPE_APPLICATION_VND_OMA_DRM_CONTENT,       "application/vnd.oma.drm.content",      "dcf"   },
  {MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_XML,    "application/vnd.oma.drm.rights+xml",   "dr"    },
  {MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_WBXML,  "application/vnd.oma.drm.rights+wbxml", "drc"   },
  {MIME_TYPE_APPLICATION_VND_OMA_DD,  "application/vnd.oma.dd+xml", "dd"   },
  {MIME_TYPE_APPLICATION_SDP,  "application/sdp", "sdp"   },
  {MIME_TYPE_APPLICATION_SDP,         "application/x-sdp", "sdp"   },
  {MIME_TYPE_APPLICATION_QBT,         "application/sprd.qbt","qbt"},
  {MIME_TYPE_APPLICATION_PNL,         "application/sprd.pnl","pnl"},
  {MIME_TYPE_APPLICATION_NATC,        "application/sprd.ntac","ntac"}
 };

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define TOTAL_MIME_TYPES (sizeof(s_mime_table)/sizeof(s_mime_table[0]))

/*****************************************************************************/
// Description: convert mime string to uint32
// Global resource dependence:
// Author: chenchen.wang
// Retrun: mime type
// Note:
/*****************************************************************************/
PUBLIC uint32 mime_to_uint( const char *mime_ptr )
{
   uint32 i = 0;
   uint32 ret = MIME_TYPE_UNKNOWN;
   for (i = 0; i < TOTAL_MIME_TYPES; i++)
   {
      if (CFLCHAR_StrICmp((char*)(s_mime_table[i].mime_ptr), (char*)mime_ptr ) == 0)
      {
         ret = s_mime_table[i].value;
      }
   }
   return ret;
}

/*****************************************************************************/
// Description: convert mime string to uint32
// Global resource dependence:
// Author: chenchen.wang
// Retrun: mime type
// Note:
/*****************************************************************************/
PUBLIC uint32 mime_ext_to_uint(const char *ext_ptr)
{
   uint32 i = 0;
   uint32 ret = MIME_TYPE_UNKNOWN;

   for (i = 0; i < TOTAL_MIME_TYPES; i++)
   {
      if (CFLCHAR_StrICmp((char*)(s_mime_table[i].ext_ptr), (char*)ext_ptr) == 0)
      {
         ret = s_mime_table[i].value;
      }
   }
   return ret;
}

/*****************************************************************************/
// Description: convert uint32 mime type to string
// Global resource dependence:
// Author: chenchen.wang
// Retrun: mime type string
// Note:
/*****************************************************************************/
PUBLIC const char* uint_to_mime( uint32 mime_value)
{
   uint32 i;

   for(i = 0; i < TOTAL_MIME_TYPES; i++)
   {
      if(s_mime_table[i].value == mime_value)
        return s_mime_table[i].mime_ptr;
   }

   return NULL;
}

/*****************************************************************************/
// Description: convert mime string to ext
// Global resource dependence:
// Author: chenchen.wang
// Retrun: mime type
// Note:
/*****************************************************************************/
PUBLIC const char *mime_to_mime_ext( const char *mime_ptr )
{
   uint32 i;

   for (i = 0; i < TOTAL_MIME_TYPES; i++)
   {
        if (CFLCHAR_StrICmp((char*)(s_mime_table[i].mime_ptr), (char*)mime_ptr) == 0)
        {
            return s_mime_table[i].ext_ptr;
        }
   }
   return NULL;
}

/*****************************************************************************/
// Description: get extension of a special mime type
// Global resource dependence:
// Author: chenchen.wang
// Retrun: ext name
// Note:
/*****************************************************************************/
PUBLIC const char *mime_to_ext( uint32 mime_value )
{
   uint32 i;

   for( i = 0; i < TOTAL_MIME_TYPES; i++)
   {
      if (s_mime_table[i].value == mime_value)
          return s_mime_table[i].ext_ptr;
   }

   return NULL;
}

