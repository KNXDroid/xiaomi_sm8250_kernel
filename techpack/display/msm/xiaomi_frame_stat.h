
#ifndef XIAOMI_FRAME_STAT_H_
#define XIAOMI_FRAME_STAT_H_

#include <linux/ktime.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/kernel.h>
#include <linux/timekeeping.h>
#include "msm_drv.h"
#include "sde_crtc.h"
#include <drm/drmP.h>
#include <drm/drm_crtc.h>
#include <drm/msm_drm.h>
#include "dsi_panel.h"
#include "dsi_display.h"

#ifndef FPS_PERIOD_1_SEC
#define FPS_PERIOD_1_SEC	(1000000)
#endif
#define IDLE_FPS					(50)
#define LONG_INTERVAL_FRAME_COUNT	(3)
#define LONG_FRAME_INTERVAL			(70000)

extern struct drm_crtc *gcrtc;
extern struct dsi_panel *g_panel;

struct frame_stat {
	ktime_t last_sampled_time_us;
	ktime_t last_frame_commit_time_us;
	u64 frame_count;
	bool start;
	/* enabled will be changed by user application, the false indicates smart dfps disabled */
	bool enabled;
	bool idle_status;
	bool skip_once;
	u64 last_fps;
	u64 skip_count;
};

extern struct frame_stat fm_stat;

static inline bool frame_stat_is_enabled(void)
{
	return g_panel && g_panel->mi_cfg.smart_fps_support &&
		READ_ONCE(fm_stat.enabled);
}

void calc_fps(bool input_event);
void frame_stat_notify(int data);
ssize_t smart_fps_value_show(struct device *device,
			struct device_attribute *attr,
			char *buf);
#endif
