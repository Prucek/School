
Demonstration of perf (linux tool)

Program perf uses "performance counters" in all modern CPU to get statistical
information about running code (including OS kernel code).  It can do
statistical profiling, too.  See perf documentation for details.

For unprivileged user we need setting of permissions (as root):
# sysctl kernel.perf_event_paranoid=-1

We use matrix multiplication test program

Run:
    ./01-perf-stat-runs.sh
    ./02-perf-run.sh
    ./03-perf-report.sh
