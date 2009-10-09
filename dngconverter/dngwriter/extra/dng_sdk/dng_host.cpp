/*****************************************************************************/
// Copyright 2006-2008 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in
// accordance with the terms of the Adobe license agreement accompanying it.
/*****************************************************************************/

/* $Id: //mondo/dng_sdk_1_2/dng_sdk/source/dng_host.cpp#1 $ */ 
/* $DateTime: 2008/03/09 14:29:54 $ */
/* $Change: 431850 $ */
/* $Author: tknoll $ */

/*****************************************************************************/

#include "dng_host.h"
#include "dng_abort_sniffer.h"
#include "dng_area_task.h"
#include "dng_exceptions.h"
#include "dng_exif.h"
#include "dng_ifd.h"
#include "dng_memory.h"
#include "dng_negative.h"
#include "dng_shared.h"
#include "dng_simple_image.h"

/*****************************************************************************/

dng_host::dng_host (dng_memory_allocator *allocator,
					dng_abort_sniffer *sniffer)

	:	fAllocator	(allocator)
	,	fSniffer	(sniffer  )
	
	,	fNeedsMeta  		(true )
	,	fNeedsImage 		(true )
	,	fForPreview 		(false)
	,	fMinimumSize 		(0    )
	,	fPreferredSize      (0    )
	,	fMaximumSize    	(0    )
	,	fCropFactor			(1.0  )
	,	fKeepStage1 		(false)
	,	fKeepStage2 		(false)
	,	fKeepDNGPrivate 	(false)
	,	fKeepOriginalFile	(false)
	
	{
	
	}
	
/*****************************************************************************/

dng_host::~dng_host ()
	{
	
	}
	
/*****************************************************************************/

dng_memory_allocator & dng_host::Allocator ()
	{
	
	if (fAllocator)
		{
		
		return *fAllocator;
		
		}
		
	else
		{
		
		return gDefaultDNGMemoryAllocator;
		
		}
	
	}

/*****************************************************************************/

dng_memory_block * dng_host::Allocate (uint32 logicalSize)
	{
	
	return Allocator ().Allocate (logicalSize);
		
	}
		
/*****************************************************************************/

void dng_host::SniffForAbort ()
	{
	
	dng_abort_sniffer::SniffForAbort (Sniffer ());
	
	}
		
/*****************************************************************************/

void dng_host::ValidateSizes ()
	{
	
	// The maximum size limits the other two sizes.
	
	if (MaximumSize ())
		{
		SetMinimumSize   (Min_uint32 (MinimumSize   (), MaximumSize ()));
		SetPreferredSize (Min_uint32 (PreferredSize (), MaximumSize ()));
		}
		
	// If we have a preferred size, it limits the minimum size.
	
	if (PreferredSize ())
		{
		SetMinimumSize (Min_uint32 (MinimumSize (), PreferredSize ()));
		}
		
	// Else find default value for preferred size.
	
	else
		{
		
		// If preferred size is zero, then we want the maximim
		// size image.
		
		if (MaximumSize ())
			{
			SetPreferredSize (MaximumSize ());
			}
		
		}
		
	// If we don't have a minimum size, find default.
	
	if (!MinimumSize ())
		{
	
		// A common size for embedded thumbnails is 120 by 160 pixels,
		// So allow 120 by 160 pixels to be used for thumbnails when the
		// preferred size is 256 pixel.
		
		if (PreferredSize () >= 160 && PreferredSize () <= 256)
			{
			SetMinimumSize (160);
			}
			
		// Many sensors are near a multiple of 1024 pixels in size, but after
		// the default crop, they are a just under.  We can get an extra factor
		// of size reduction if we allow a slight undershoot in the final size
		// when computing large previews.
		
		else if (PreferredSize () >= 490 && PreferredSize () <= 512)
			{
			SetMinimumSize (490);
			}

		else if (PreferredSize () >= 980 && PreferredSize () <= 1024)
			{
			SetMinimumSize (980);
			}

		else if (PreferredSize () >= 1470 && PreferredSize () <= 1536)
			{
			SetMinimumSize (1470);
			}

		else if (PreferredSize () >= 1960 && PreferredSize () <= 2048)
			{
			SetMinimumSize (1960);
			}

		// Else minimum size is same as preferred size.
			
		else
			{
			SetMinimumSize (PreferredSize ());
			}
			
		}
	
	}
			
/*****************************************************************************/

bool dng_host::IsTransientError (dng_error_code code)
	{
	
	switch (code)
		{
		
		case dng_error_memory:
		case dng_error_user_canceled:
			{
			return true;
			}
			
		default:
			break;
			
		}
		
	return false;
	
	}
		
/*****************************************************************************/

void dng_host::PerformAreaTask (dng_area_task &task,
								const dng_rect &area)
	{
	
	dng_area_task::Perform (task,
							area,
							&Allocator (),
							Sniffer ());
	
	}
		
/*****************************************************************************/

dng_exif * dng_host::Make_dng_exif ()
	{
	
	dng_exif *result = new dng_exif ();
	
	if (!result)
		{
		
		ThrowMemoryFull ();

		}
	
	return result;
	
	}

/*****************************************************************************/

dng_shared * dng_host::Make_dng_shared ()
	{
	
	dng_shared *result = new dng_shared ();
	
	if (!result)
		{
		
		ThrowMemoryFull ();

		}
	
	return result;
	
	}

/*****************************************************************************/

dng_ifd * dng_host::Make_dng_ifd ()
	{
	
	dng_ifd *result = new dng_ifd ();
	
	if (!result)
		{
		
		ThrowMemoryFull ();

		}
	
	return result;
	
	}

/*****************************************************************************/

dng_negative * dng_host::Make_dng_negative ()
	{
	
	return dng_negative::Make (Allocator ());
	
	}

/*****************************************************************************/

dng_image * dng_host::Make_dng_image (const dng_rect &bounds,
									  uint32 planes,
									  uint32 pixelType)
	{
	
	dng_image *result = new dng_simple_image (bounds,
											  planes,
											  pixelType,
											  0,
											  Allocator ());
	
	if (!result)
		{
		
		ThrowMemoryFull ();

		}
	
	return result;
	
	}
					      		 
/*****************************************************************************/
