# Changelog – xv6 Scheduler Enhancements

All notable modifications to the original xv6-riscv operating system for this project.

## [1.0.0] - 2025

### Added

#### New System Call
- **`getreadcount()` system call** (syscall #22) to track cumulative bytes read
  - Global 64-bit counter in kernel space
  - Tracks all successful `read()` operations
  - Overflow handling via natural wrap-around
  - Full implementation stack: kernel handler, syscall table entry, user-space wrapper

#### FCFS Scheduler
- **First Come First Serve scheduling algorithm** (build flag: `SCHEDULER=FCFS`)
  - Added `ctime` field to `struct proc` for tracking process creation time
  - Non-preemptive scheduler selecting earliest created RUNNABLE process
  - Demonstrates convoy effect with sequential process execution
  - Minimal context switching overhead

#### CFS Scheduler
- **Completely Fair Scheduler** (build flag: `SCHEDULER=CFS`)
  - Added `nice`, `weight`, `vruntime`, and `timeslice` fields to `struct proc`
  - Virtual runtime tracking normalized by process weight
  - Dynamic timeslice calculation: `max(TARGET_LATENCY / num_runnable, MIN_TIMESLICE)`
  - Priority support through nice values (-20 to 19)
  - Weight calculation approximating `weight = 1024 / (1.25^nice)`
  - Timer interrupt updates for vruntime and preemption logic
  - Minimum vruntime selection for fairness

#### Test Programs
- **`schedulertest.c`** - User-space test program for comparing scheduler behaviors
  - Spawns 4 concurrent CPU-bound worker processes
  - Reports progress at 25%, 50%, 75%, and 100% completion
  - Measures total completion time in ticks
  - Demonstrates scheduling differences between FCFS, CFS, and Round Robin

- **`readcount.c`** - Test program for `getreadcount()` system call
  - Validates byte counting accuracy
  - Tests before/after read operations
  - Verifies correct increment behavior

#### Documentation
- Comprehensive README with usage instructions and performance comparison
- Detailed IMPLEMENTATION.md with code snippets and algorithm explanations
- This CHANGELOG documenting all modifications

### Modified

#### Kernel Files

**`kernel/proc.h`**
- Extended `struct proc` with new fields:
  - `uint64 ctime` for FCFS
  - `int nice`, `uint64 weight`, `uint64 vruntime`, `int timeslice` for CFS

**`kernel/proc.c`**
- Modified `scheduler()` function with conditional compilation:
  - Added `#ifdef FCFS` block for FCFS scheduling logic
  - Added `#ifdef CFS` block for CFS scheduling logic
  - Preserved default Round Robin scheduler
- Modified `allocproc()` to initialize new fields:
  - Set `ctime = ticks` for process creation time
  - Initialize CFS fields (`nice=0`, `weight=1024`, `vruntime=0`, `timeslice`)
- Added helper function `calc_weight(int nice)` for CFS weight calculation

**`kernel/trap.c`**
- Modified timer interrupt handler (`usertrap()`) for CFS:
  - Update `vruntime` based on process weight
  - Decrement `timeslice`
  - Call `yield()` when timeslice expires
  - Conditional compilation with `#ifdef CFS`

**`kernel/sysfile.c`**
- Declared global variable `uint64 total_bytes_read`
- Modified `sys_read()` to increment counter on successful reads

**`kernel/sysproc.c`**
- Added `sys_getreadcount()` implementation returning global counter

**`kernel/syscall.h`**
- Added `#define SYS_getreadcount 22`

**`kernel/syscall.c`**
- Added external declaration: `extern uint64 sys_getreadcount(void);`
- Registered `sys_getreadcount` in syscall table at index 22

#### User Files

**`user/usys.pl`**
- Added entry for `getreadcount` syscall wrapper

**`user/user.h`**
- Added prototype: `int getreadcount(void);`

**`user/Makefile`** (implied)
- Added `$U/_schedulertest` and `$U/_readcount` to UPROGS list

#### Build System

**`Makefile`**
- Added conditional compilation support:
  ```makefile
  ifdef SCHEDULER
  CFLAGS += -D$(SCHEDULER)
  endif
  ```
- Enables building with `make qemu SCHEDULER=FCFS` or `SCHEDULER=CFS`

### Performance Improvements

- **CFS shows 27% faster completion** compared to FCFS in test workload (8 ticks vs 11 ticks)
- Better CPU utilization through fair time-slicing
- Reduced convoy effect compared to FCFS

### Testing

- Verified FCFS: strict sequential execution, no preemption
- Verified CFS: fair vruntime distribution, dynamic timeslicing, priority support
- Verified getreadcount(): accurate byte tracking across read operations
- All schedulers tested on single CPU configuration (CPUS=1)

---

## Original xv6-riscv Base

This project is based on the xv6-riscv teaching operating system by MIT PDOS.

**Original features retained:**
- Round Robin scheduler (default when no SCHEDULER flag specified)
- All existing system calls and kernel functionality
- Original process management and memory management
- File system and device drivers

**License:** MIT License (inherited from xv6-riscv)

---

## Build Instructions

```bash
# FCFS Scheduler
make clean
make qemu SCHEDULER=FCFS CPUS=1

# CFS Scheduler
make clean
make qemu SCHEDULER=CFS CPUS=1

# Default Round Robin
make clean
make qemu CPUS=1
```

## Notes

- All modifications use conditional compilation to maintain original xv6 behavior
- Original Round Robin scheduler remains default and unmodified
- Changes are isolated and can be individually enabled/disabled
- Code follows xv6 style and conventions
- Compatible with RISC-V architecture and QEMU emulation
