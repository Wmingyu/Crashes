# DevGen Crash Reports

This repository records crash cases discovered during the evaluation of **DevGen**.

## Overview

The goal of this repository is to collect and organize crash cases found by **DevGen** so that they can be:

- inspected and analyzed easily,
- reproduced in a compatible environment,
- and reused for future research on Linux driver testing, virtualization, and fuzzing.

## Repository Structure

The repository is organized as follows:

```text
.
├── <crash-id-1>/
│   ├── *.c                     # modeled virtual device
│   ├── *.json                  # syzkaller run configuration
│   ├── repro.c / repro.txt     # reproducer program or reproducer description
│   └── log.txt                 # kernel output log
├── <crash-id-2>/
│   ├── *.c
│   ├── *.json
│   ├── repro.c / repro.txt
│   └── log.txt
├── 6.18.config
├── 6.18_syzbot.config
└── README.md
```

## Contents per Crash

Each crash directory may include the following files:

- **`*.c`** – the modeled virtual device code used to trigger the target driver behavior.
- **`*.json`** – the syzkaller run configuration used for the crash-triggering setup.
- **Reproducer** – a reproducer program in **C** or **syz** used to replay the crash.
- **Other files** – the kernel output log, including warnings, stack traces, or crash messages.

## Usage

1. Navigate to a crash directory.
2. Examine `dmesg.txt` or `log.txt` to inspect the crash message and failure details.
3. Run the reproducer in a compatible environment to reproduce the issue.

## Notes

- Kernel configs (`6.18.config`, `6.18_syzbot.config`) are provided for reference.
- Some crashes may correspond to confirmed bugs or CVEs; others may be under investigation.
- This repository focuses on crash artifacts and reproduction, not the full DevGen implementation.
  
