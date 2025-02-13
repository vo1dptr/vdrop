# vDrop

vDrop is a simple CLI tool for fast directory search and navigation.

It searches for subdirectories in the current working directory, and when it finds the directory you want to visit, it takes you there.

My motivation was that I was too lazy to keep navigating through multiple directories, and too lazy to search for an existing tool.

### How It Works

`vdrop` performs a recursive search starting from your current directory to find the specified subdirectory. You can also add paths to a cache for quicker access later, which is stored in a file in your home directory.

### Build

To build the project, just run:

```sh
$ make
```

### Installation

After building, add the binary to your local binary directory (like `~/.local/bin` or `/usr/local/bin`) to use it from anywhere in your terminal:

```sh
$ cp vdrop /usr/local/bin/
```

If you prefer shell aliases, add this line to your shell config file (like `.bashrc` or `.zshrc`):

```sh
alias vdrop='/path/to/vdrop'
```

### Clean Build

Just run:

```sh
$ make clean
```

### Note

Searching through large directories, like `home` or `/`, might take some time. But once a directory is found, it’ll be cached, so future searches will be much faster. You can also manually add directories to the cache for quick access.

### Contribution

Contributions are open, just submit a pull request or open an issue.

For details on usage, run `vdrop --help` after building.
