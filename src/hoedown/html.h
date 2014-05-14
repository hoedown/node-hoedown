/* html.h - HTML renderer */

#ifndef HOEDOWN_HTML_H
#define HOEDOWN_HTML_H

#include "document.h"
#include "buffer.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	HOEDOWN_HTML_SKIP_HTML = (1 << 0),
	HOEDOWN_HTML_ESCAPE = (1 << 1),
	HOEDOWN_HTML_EXPAND_TABS = (1 << 2),
	HOEDOWN_HTML_SAFELINK = (1 << 3),
	HOEDOWN_HTML_HARD_WRAP = (1 << 4),
	HOEDOWN_HTML_USE_XHTML = (1 << 5),
} hoedown_html_flags;

typedef enum {
	HOEDOWN_HTML_TAG_NONE = 0,
	HOEDOWN_HTML_TAG_OPEN,
	HOEDOWN_HTML_TAG_CLOSE
} hoedown_html_tag;

struct hoedown_html_renderer_state {
	void *opaque;

	struct {
		int header_count;
		int current_level;
		int level_offset;
		int nesting_level;
	} toc_data;

	unsigned int flags;

	/* extra callbacks */
	void (*link_attributes)(hoedown_buffer *ob, const hoedown_buffer *url, void *self);
};

typedef struct hoedown_html_renderer_state hoedown_html_renderer_state;

int
hoedown_html_is_tag(const uint8_t *tag_data, size_t tag_size, const char *tagname);

extern hoedown_renderer *
hoedown_html_renderer_new(unsigned int render_flags, int nesting_level);

extern hoedown_renderer *
hoedown_html_toc_renderer_new(int nesting_level);

extern void
hoedown_html_renderer_free(hoedown_renderer *renderer);

extern void
hoedown_html_smartypants(hoedown_buffer *ob, const uint8_t *text, size_t size);

#ifdef __cplusplus
}
#endif

#endif /** HOEDOWN_HTML_H **/
