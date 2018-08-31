# Netflix 5 reference local gcc/cmake build

How to convert this to document to html:

```
grip --export readme.md
```


## Toolchain

Make sure the following file was extracted in `/` (root):

```
netflix-toolchain-desktop-5.0.11-4443.tar.bz2
```

md5sum:

```
0a659688ab2f4d8ac90fc47fb9ad496f
```


## Bound key message

Make sure somewhere the following file is available:

```
boundKeyMsg.bin
```

Set variable `BOUND_KEY_MESSAGE` in `netflix5-run.sh` to its location


## Initial downloading of netflix sources

Run `./netflix5-fetch.sh`


## Building

Run `./fullrun.sh`

To speed up building, export `MAKE_PROC_COUNT` variable, for example:

```
$ export MAKE_PROC_COUNT=4
$ ./fullrun.sh
```

## Running

Run `./netflix5-run.sh`


