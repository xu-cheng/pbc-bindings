import itertools
import subprocess
import sys
import os


def DirectoryOfThisScript():
    return os.path.dirname(os.path.abspath(__file__))


def MakeRelativePathsInFlagsAbsolute(flags, working_directory):
    if not working_directory:
        return list(flags)
    new_flags = []
    make_next_absolute = False
    path_flags = ["-isystem", "-I", "-iquote", "--sysroot="]
    for flag in flags:
        new_flag = flag

        if make_next_absolute:
            make_next_absolute = False
            if not flag.startswith('/'):
                new_flag = os.path.join(working_directory, flag)

        for path_flag in path_flags:
            if flag == path_flag:
                make_next_absolute = True
                break

            if flag.startswith(path_flag):
                path = flag[len(path_flag):]
                new_flag = path_flag + os.path.join(working_directory, path)
                break

        if new_flag:
            new_flags.append(new_flag)
    return new_flags


def SystemIncludePathasFlags():
    args = ["c++", "-E", "-x", "c++", "-", "-v"]
    out = subprocess.check_output(
        args,
        encoding="UTF-8",
        stdin=subprocess.DEVNULL,
        stderr=subprocess.STDOUT)
    out = out.split("\n")
    start = out.index("#include <...> search starts here:") + 1
    end = out.index("End of search list.")
    flags = [["-isystem", path.strip()] for path in out[start:end]
             if not path.endswith("(framework directory)")]
    return list(itertools.chain(*flags))


def FlagsForFile(filename, **kwargs):
    flags = [
        "-Wall",
        "-Wextra",
        "-Werror",
        "-Wno-long-long",
        "-Wno-variadic-macros",
        "-fexceptions",
        "-DNDEBUG",
        "-std=c++14",
        "-x",
        "c++",
        "-Iinclude/",
    ]
    flags += SystemIncludePathasFlags()
    flags = MakeRelativePathsInFlagsAbsolute(flags, DirectoryOfThisScript())
    return {"flags": flags, "do_cache": True}
