# xv6 Scheduler Enhancements

Custom scheduling algorithm implementations and system call extensions for xv6, the teaching operating system based on Unix V6. Implemented FCFS and CFS schedulers with priority support and virtual runtime tracking in C.

## Overview

xv6 is a re-implementation of Dennis Ritchie's and Ken Thompson's Unix Version 6 (V6) for RISC-V architecture, used for teaching operating systems concepts. This project extends xv6 with:

- Custom `getreadcount()` system call for tracking read operations
- **FCFS (First Come First Serve)** scheduler implementation
- **CFS (Completely Fair Scheduler)** with priority support and dynamic timeslicing
- Comprehensive testing framework comparing scheduler behaviors

## 🎯 Key Features

- **System Call Implementation** - Added kernel-level read tracking with 64-bit overflow handling
- **FCFS Scheduler** - Non-preemptive scheduling based on process creation time
- **CFS Scheduler** - Fair scheduling with virtual runtime tracking and priority weights
- **Dynamic Timeslicing** - Adaptive quantum calculation based on system load
- **Scheduler Comparison Tests** - Benchmarking framework demonstrating behavioral differences

## Technical Highlights

- Implemented two CPU scheduling algorithms from scratch in kernel space
- Modified xv6 kernel trap handler for CFS virtual runtime tracking
- Designed weight calculation approximating exponential decay: `weight = 1024 / (1.25^nice)`
- Built non-preemptive FCFS showing convoy effect vs. fair CFS timeslicing
- Created userspace test program demonstrating 27% performance improvement with CFS

## Implementation Details

### FCFS (First Come First Serve)
- Non-preemptive scheduler selecting process with earliest creation time
- Added `ctime` field to `struct proc` for tracking process creation
- Demonstrates convoy effect with sequential process completion
- Minimal context switching overhead

### CFS (Completely Fair Scheduler)
- Virtual runtime tracking normalized by process weight
- Dynamic timeslice: `max(TARGET_LATENCY / num_runnable, MIN_TIMESLICE)`
- Priority support through nice values affecting CPU time distribution
- Mimics Linux CFS behavior with simplified weight calculation
- Fair CPU distribution across processes of different priorities

### getreadcount() System Call
- Tracks total bytes read across all file operations
- 64-bit counter with natural overflow handling
- Demonstrates full system call implementation stack

[View Full Implementation Details](IMPLEMENTATION.md)

## Performance Results

Tested with 4 CPU-bound processes running identical workloads:

| Scheduler | Total Time | Behavior | Context Switches | Fairness |
|-----------|------------|----------|------------------|----------|
| FCFS | 11 ticks | Sequential | Minimal | Poor |
| CFS | 8 ticks | Interleaved | Moderate | Excellent |
| Round Robin | ~10 ticks | Time-sliced | High | Good |

### Key Findings
- **CFS achieved 27% faster completion** due to better CPU utilization
- FCFS showed convoy effect with strict sequential execution
- CFS virtual runtime correctly balanced CPU time across processes
- Dynamic timeslicing adapted to system load effectively

## Quick Start

```bash
# Clone the repository
git clone https://github.com/<your-username>/xv6-riscv.git
cd xv6-riscv

# Build and run with FCFS
make qemu SCHEDULER=FCFS CPUS=1

# Build and run with CFS
make qemu SCHEDULER=CFS CPUS=1

# In xv6 shell, run tests
$ schedulertest
$ readcount
```

**Requirements:** RISC-V toolchain, QEMU

## Testing

The project includes two test programs:

- **schedulertest** - Creates 4 concurrent processes to demonstrate scheduling behavior
- **readcount** - Tests the getreadcount() system call functionality

```bash
# Test FCFS
make clean
make qemu SCHEDULER=FCFS CPUS=1
$ schedulertest

# Test CFS  
make clean
make qemu SCHEDULER=CFS CPUS=1
$ schedulertest

# Test system call
$ readcount
```

## Learning Outcomes

- Deep dive into kernel-level process scheduling mechanisms
- Hands-on experience with OS kernel modification and compilation
- Understanding of fairness algorithms and priority-based scheduling
- System call implementation from kernel to user space
- Performance analysis of different scheduling policies
- Working with RISC-V architecture and assembly

## Project Structure

```
xv6-riscv/
├── kernel/
│   ├── proc.c          # Scheduler implementations
│   ├── trap.c          # Timer interrupt handling for CFS
│   ├── sysfile.c       # Read tracking for getreadcount
│   ├── sysproc.c       # System call implementation
│   └── syscall.c       # System call table
├── user/
│   ├── schedulertest.c # Scheduler comparison test
│   └── readcount.c     # System call test
└── Makefile            # Build with SCHEDULER flag
```

## Modifications from Original xv6

This fork adds:
- FCFS scheduling algorithm with creation time tracking
- CFS scheduling algorithm with virtual runtime
- getreadcount() system call (syscall #22)
- Test programs for verification
- Conditional compilation support for scheduler selection

See [CHANGELOG.md](CHANGELOG.md) for detailed modifications.

## License

This project is based on xv6-riscv, which is licensed under the MIT License.

## Acknowledgments

- Original xv6 by MIT PDOS
- Inspired by Linux CFS scheduler design
- Course: Operating Systems, IIIT Hyderabad

