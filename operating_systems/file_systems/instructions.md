---
title: Writing a novelty filesystem
---

<section>

The purpose of this exercise is to increase your familiarity with the filesystem as an *abstraction*. There's also plenty to know about the implementation of various filesystems, but concentrating on the interface itself is a useful first step. In order to do this, we'll be using FUSE.

# FUSE {-}

[FUSE](https://github.com/libfuse/libfuse) (apparently "Filesystem in Userspace") is a kernel module (`fuse`) and userspace library (`libfuse`) for implementing file system functionality outside of the kernel, by providing hooks that call in to your own code whenever certain system calls are made. For instance, once your custom filesystem is mounted, running `ls` will as usual cause system calls such as `open` to be called, which will in this case defer to the `fuse` kernel module, which defers to your own code (via `libfuse`).

::: note
There is [a version of FUSE for macOS](https://osxfuse.github.io/) which is substantially different to the Linux FUSE linked above, but should work just as well on macOS 10.9 and later. You're welcome to target either Linux or macOS.
:::

FUSE in poorly documented. Some [unofficial documentation](https://www.cs.hmc.edu/~geoff/classes/hmc.cs135.201109/homework/fuse/fuse_doc.html) is available from Geoff Kuenning. 

We suggest following the [installation instructions](https://github.com/libfuse/libfuse#installation) then making sure you can understand and run the "hello.c" (for Linux) or "hello_macfuse.c" (for macOS) from the exercise files linked above. Then, take a look at the [examples directory](https://github.com/libfuse/libfuse/tree/master/example). As you code, reading the library headers (which you can find with `locate \/fuse.h`) will likely help. You may also find [this tutorial](https://www.cs.nmsu.edu/~pfeiffer/fuse-tutorial/) helpful.

`libfuse` is a C library and the assumption is that you'll be writing your code in C, but there are also bindings available in most mainstream languages. However, this adds a level of indirection and possible version incompatibility... pick your poison.

# Instructions {-}

The exercise is simply to create some kind of novelty filesystem of your choosing with FUSE.

We encourage you to get creative! Once you're somewhat familiar with the FUSE hooks, it shouldn't take you long to come up with an idea for a novelty filesystem, particularly since it doesn't need to be useful in any way. That said, here's the kind of thing you could do in a straightforward manner with FUSE:

- [passthrough](https://github.com/libfuse/libfuse/blob/master/example/passthrough.c) (from libfuse examples): just mirror the existing file system at a different mount point by passing through all requests to the corresponding libc functions
- StatOS: mirror the existing file system, but also add a dotfile in each directory which contains stats about the directory (such as the number of files contained, permissions, or even just the contents of `stat`)
- DictFS: Just define some hierarchical structure in memory (such as nested arrays or a hashmap) and turn this into a filesystem
- WikipediaFS: each directory holds text and image files from a Wikipedia page. Links are represented as subdirectories
- s3Mirror: mount an S3 bucket and explore it in Finder/Terminal etc
- LifeOS (the reference solution): represent the tree of life as a file system. Each taxon is a directory, and you can click/cd down to a species, which contains extra information. General utilities such as `find` should work, albeit slowly.

If you had more time, you could implement something like a dungeon crawler game, with commands represented as links to rooms (directories).

For an example of a more substantial userspace filesystem, check out [TabFS](https://omar.website/tabfs/) by Omar Rizwan, a browser extension that mounts your browser tabs as a filesystem on your computer.

</section>
