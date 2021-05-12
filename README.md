# C Logger

C library logger by using `LD_PRELOAD`

## Environment

- `gcc`
- `make`

## Usage

- `make` for compile source
- `./logger [-h|--help] [-o file] [-p sofile] [--] cmd [cmd args ...]` for execute command
    - Or using `LD_PRELOAD=./logger.so cmd [cmd args ...]`

