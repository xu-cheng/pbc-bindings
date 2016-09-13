import os
import subprocess

def SystemIncludePathasFlags():
    cmd = "c++ -E -x c++ - -v </dev/null 2>&1"
    out = subprocess.check_output(cmd, shell=True).decode("utf-8")
    out = out.split("\n")
    out = out[out.index("#include <...> search starts here:") + 1:-1]
    out = out[0:out.index("End of search list.")]
    out = [p.strip() for p in out if not p.endswith("(framework directory)")]
    flags = [["-isystem", p] for p in out]
    return [f for ff in flags for f in ff]


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
            "-I./include",
            ]
    flags += SystemIncludePathasFlags()
    return { "flags": flags, "do_cache": True}
