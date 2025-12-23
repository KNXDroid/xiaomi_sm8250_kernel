/* drivers/misc/trace_stubs.c */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/trace_events.h>

/*
 * Stubs for missing Tracing Core functions.
 * Required because CONFIG_TRACEPOINTS=y but CONFIG_EVENT_TRACING=n
 */
#ifndef CONFIG_EVENT_TRACING
int trace_define_field(struct trace_event_call *call, const char *type,
                       const char *name, int offset, int size,
                       int is_signed, int filter_type)
{
    return 0;
}
EXPORT_SYMBOL(trace_define_field);

enum event_trigger_type
event_triggers_call(struct trace_event_file *file, void *rec,
                    struct ring_buffer_event *event)
{
    return 0;
}
EXPORT_SYMBOL(event_triggers_call);

void *trace_event_buffer_reserve(struct trace_event_buffer *fbuffer,
                                 struct trace_event_file *trace_file,
                                 unsigned long len)
{
    return 0;
}
EXPORT_SYMBOL(trace_event_buffer_reserve);

void trace_event_buffer_commit(struct trace_event_buffer *fbuffer,
                               unsigned long len)
{
}
EXPORT_SYMBOL(trace_event_buffer_commit);

bool trace_event_ignore_this_pid(struct trace_event_file *trace_file)
{
    return 0;
}
EXPORT_SYMBOL(trace_event_ignore_this_pid);

/* Perf trace stubs */
void *perf_trace_buf_alloc(int size, struct pt_regs **regs, int *rctxp)
{
    return 0;
}
EXPORT_SYMBOL(perf_trace_buf_alloc);

void perf_trace_run_bpf_submit(void *raw_data, int size, int rctx,
                               struct trace_event_call *call, u64 count,
                               struct pt_regs *regs, struct hlist_head *head,
                               struct task_struct *task)
{
}
EXPORT_SYMBOL(perf_trace_run_bpf_submit);
#endif
