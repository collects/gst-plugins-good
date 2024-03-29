/* GStreamer
 * Copyright (C) <1999> Erik Walthinsen <omega@cse.ogi.edu>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */


#ifndef __GST_QTDEMUX_H__
#define __GST_QTDEMUX_H__

#include <gst/gst.h>
#include <gst/base/gstadapter.h>

G_BEGIN_DECLS

GST_DEBUG_CATEGORY_EXTERN (qtdemux_debug);
#define GST_CAT_DEFAULT qtdemux_debug

#define GST_TYPE_QTDEMUX \
  (gst_qtdemux_get_type())
#define GST_QTDEMUX(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_QTDEMUX,GstQTDemux))
#define GST_QTDEMUX_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_QTDEMUX,GstQTDemuxClass))
#define GST_IS_QTDEMUX(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_QTDEMUX))
#define GST_IS_QTDEMUX_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_QTDEMUX))

#define GST_QTDEMUX_CAST(obj) ((GstQTDemux *)(obj))

/* qtdemux produces these for atoms it cannot parse */
#define GST_QT_DEMUX_PRIVATE_TAG "private-qt-tag"
#define GST_QT_DEMUX_CLASSIFICATION_TAG "classification"

#define GST_QTDEMUX_MAX_STREAMS         32

typedef struct _GstQTDemux GstQTDemux;
typedef struct _GstQTDemuxClass GstQTDemuxClass;
typedef struct _QtDemuxStream QtDemuxStream;

struct _GstQTDemux {
  GstElement element;

  /* pads */
  GstPad *sinkpad;

  QtDemuxStream *streams[GST_QTDEMUX_MAX_STREAMS];
  gint     n_streams;
  gint     n_video_streams;
  gint     n_audio_streams;
  gint     n_sub_streams;

  gboolean have_group_id;
  guint group_id;

  guint  major_brand;
  GstBuffer *comp_brands;
  GNode *moov_node;
  GNode *moov_node_compressed;

  guint32 timescale;
  guint64 duration;

  gboolean fragmented;
  /* offset of the mfra atom */
  guint64 mfra_offset;
  guint64 moof_offset;

  gint state;

  gboolean pullbased;
  gboolean posted_redirect;

  /* push based variables */
  guint neededbytes;
  guint todrop;
  GstAdapter *adapter;
  GstBuffer *mdatbuffer;
  guint64 mdatleft;

  guint64 offset;
  /* offset of the mdat atom */
  guint64 mdatoffset;
  guint64 first_mdat;
  gboolean got_moov;
  guint header_size;

  GstTagList *tag_list;

  /* configured playback region */
  GstSegment segment;
  GstEvent *pending_newsegment;
  gboolean upstream_newsegment;

#if 0
  /* gst index support */
  GstIndex *element_index;
  gint index_id;
#endif

  gint64 requested_seek_time;
  guint64 seek_offset;

  gboolean upstream_seekable;
  gint64 upstream_size;

  /* MSS streams have a single media that is unspecified at the atoms, so
   * upstream provides it at the caps */
  GstCaps *media_caps;
  gboolean exposed;
  gboolean mss_mode; /* flag to indicate that we're working with a smoothstreaming fragment */
  guint64 fragment_start;
    
  gint64 chapters_track_id;

  GstClockTime min_elst_offset;
};

struct _GstQTDemuxClass {
  GstElementClass parent_class;
};

GType gst_qtdemux_get_type (void);

G_END_DECLS

#endif /* __GST_QTDEMUX_H__ */
