#include "xiaomi_frame_stat.h"

struct frame_stat fm_stat;

ssize_t smart_fps_value_show(struct device *device,
			   struct device_attribute *attr,
			   char *buf)
{
	if (!g_panel)
		return snprintf(buf, PAGE_SIZE, "%d\n", 0);

	return snprintf(buf, PAGE_SIZE, "%d\n", g_panel->mi_cfg.smart_fps_value);
}

void frame_stat_notify(int data)
{
	struct dsi_display *display = NULL;
	struct mipi_dsi_host *host = NULL;

	if (g_panel)
		host = g_panel->host;

	if (host)
		display = container_of(host, struct dsi_display, host);

	if (!display) {
		pr_err("%s: invalid param.\n");
		return;
	}

	g_panel->mi_cfg.smart_fps_value = data;

	sysfs_notify(&display->drm_conn->kdev->kobj, NULL, "smart_fps_value");

	fm_stat.skip_count = 0;
	fm_stat.last_fps = data;
}

void calc_fps(bool input_event)
{
	ktime_t current_time_us = 0;
	u64 fps, diff_us, diff, curr_fps, idle_fps;

	if (!frame_stat_is_enabled())
		return;

	if (input_event) {
		frame_stat_notify(0xFF); //0xFF used as a symbol of input event.
		fm_stat.last_fps = g_panel->mi_cfg.smart_fps_max_framerate;
		fm_stat.skip_once = true;
		goto exit;
	}

	idle_fps = g_panel->mi_cfg.idle_fps ? (u64)g_panel->mi_cfg.idle_fps : IDLE_FPS;
	current_time_us = ktime_get();
	if (fm_stat.idle_status) {
		if (fm_stat.last_fps != (u64)idle_fps) {
			if (g_panel->panel_mode == DSI_OP_CMD_MODE)
				/* video panel will directly notify SDM */
				frame_stat_notify((int)idle_fps);

			fm_stat.last_fps = (u64)idle_fps;
		}
		goto exit;
	}

	if (!fm_stat.start) {
		fm_stat.last_sampled_time_us = current_time_us;
		fm_stat.start = true;
	}
	diff_us = (u64)ktime_us_delta(current_time_us, fm_stat.last_sampled_time_us);

	fm_stat.frame_count++;

	if (fm_stat.last_frame_commit_time_us > 0) {
		diff = (u64)ktime_us_delta(current_time_us, fm_stat.last_frame_commit_time_us);
		fm_stat.last_frame_commit_time_us = current_time_us;
		if (diff > LONG_FRAME_INTERVAL) {
			fm_stat.skip_count++;
			if (fm_stat.skip_count > LONG_INTERVAL_FRAME_COUNT) {
				/* Sometime  app refresh in low fps, here set 50hz */
				frame_stat_notify((u64)idle_fps);
				fm_stat.last_fps = (u64)idle_fps;
			}
			goto exit;
		} else
			fm_stat.skip_count = 0;
	}

	if (diff_us >= FPS_PERIOD_1_SEC) {
		/* skip once after input events */
		if (fm_stat.skip_once) {
			fm_stat.skip_once = false;
			goto exit;
		}

		 /* Multiplying with 10 to get fps in floating point */
		fps = fm_stat.frame_count * FPS_PERIOD_1_SEC * 10;
		do_div(fps, diff_us);
		curr_fps = (unsigned int)fps/10;
		if (curr_fps != fm_stat.last_fps)
			frame_stat_notify(curr_fps);

		goto exit;
	} else {
		fm_stat.last_frame_commit_time_us = current_time_us;
		goto final;
	}

exit:
	fm_stat.last_sampled_time_us = current_time_us;
	fm_stat.frame_count = 0;
	fm_stat.start = false;
	fm_stat.last_frame_commit_time_us = current_time_us;
final:
	return;
}
