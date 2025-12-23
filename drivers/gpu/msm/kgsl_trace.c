// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2011,2013,2015,2019 The Linux Foundation. All rights reserved.
 */

#include <linux/module.h>
#include <linux/tracepoint.h>

/* If the macro is missing, define it to manually export the tracepoint symbol */
#ifndef EXPORT_TRACEPOINT_SYMBOL
#define EXPORT_TRACEPOINT_SYMBOL(name) EXPORT_SYMBOL_GPL(__tracepoint_##name)
#endif

#include "kgsl_device.h"

/* Instantiate tracepoints */
#define CREATE_TRACE_POINTS
#include "kgsl_trace.h"

EXPORT_TRACEPOINT_SYMBOL(kgsl_regwrite);
EXPORT_TRACEPOINT_SYMBOL(kgsl_issueibcmds);
EXPORT_TRACEPOINT_SYMBOL(kgsl_user_pwrlevel_constraint);
EXPORT_TRACEPOINT_SYMBOL(kgsl_constraint);
